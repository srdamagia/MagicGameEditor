/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2016 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2016
Thiago C. Moraes
http://www.magictech.com.br

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "skeletoninfo.h"
#include "ui_skeletoninfo.h"
#include "modelanimationsequences.h"
#include "AnimationTreeItem.h"
#include "magic3d/magic3d.h"
#include "utils.h"

SkeletonInfo::SkeletonInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::SkeletonInfo)
{
    ui->setupUi(this);

    animationTreeModel = new AnimationTreeModel("default", ui->treeAnimations, this);

    ui->treeAnimations->setModel(animationTreeModel);
    ui->treeAnimations->setColumnWidth(0, 75);
    ui->treeAnimations->setColumnWidth(1, 50);
    ui->treeAnimations->setColumnWidth(2, 50);
    ui->treeAnimations->setColumnWidth(3, 50);
    ui->treeAnimations->setColumnWidth(4, 50);

    QItemSelectionModel *selectionModel= ui->treeAnimations->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,           SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
}

SkeletonInfo::~SkeletonInfo()
{
    if (animationTreeModel)
    {
        ui->treeAnimations->setModel(NULL);
        delete animationTreeModel;
        animationTreeModel = NULL;
    }

    delete ui;
}

Magic3D::Model* SkeletonInfo::getModel()
{
    Magic3D::Object* object = getObject();
    Magic3D::Model* model = NULL;

    if (object && object->getType() == Magic3D::eOBJECT_MODEL)
    {
        model = static_cast<Magic3D::Model*>(object);
    }
    return model;
}

void SkeletonInfo::update()
{
    updating = true;
    Magic3D::Model* model = getModel();

    animationTreeModel->removeAll();

    if (model)
    {
        if (model->getSkeleton())
        {
            ui->lblBones->setText(tr("%0").arg(model->getSkeleton()->getBoneCount()));

            ui->sldAnimation->setMaximum(model->getSkeleton()->getAnimation()->getFrames()->size() - 1);
            if (ui->sldAnimation->maximum() < 0)
            {
                ui->sldAnimation->setMaximum(0);
            }
            ui->sldAnimation->setMinimum(0);
            ui->sldAnimation->setValue(0);

            Magic3D::AnimationFrame* frame = model->getSkeleton()->getAnimation()->getFrame(ui->sldAnimation->value());
            if (frame)
            {
                ui->lblTime->setText(tr("%0").arg(frame->time, 0, 'f', 3));
            }


            ui->btnAdd->setEnabled(true);
            ui->btnRemove->setEnabled(model->getSkeleton()->getAnimation()->getFrameCount() > 0);

            std::string current;

            if (model->getSkeleton()->getAnimation()->getCurrentSequence())
            {
                current = model->getSkeleton()->getAnimation()->getCurrentSequence()->getName();
            }


            AnimationTreeItem* root = animationTreeModel->getRoot();
            animationTreeModel->removeAll();

            std::vector<Magic3D::AnimationSequence*>* animations = model->getSkeleton()->getAnimation()->getSequences();
            if (animations)
            {
                AnimationTreeItem* selected = NULL;

                std::vector<Magic3D::AnimationSequence*>::const_iterator it_s = animations->begin();
                while (it_s != animations->end())
                {
                    Magic3D::AnimationSequence* seq = *it_s++;

                    QList<QVariant> itemData;
                    itemData << QString::fromStdString(seq->getName());
                    itemData << tr("%0").arg(seq->getStartFrame());
                    itemData << tr("%0").arg(seq->getEndFrame());
                    itemData << tr("%0").arg(seq->getSpeed(), 0, 'f', 3);
                    itemData << tr("%0").arg(seq->isLooping() ? "true" : "false");

                    AnimationTreeItem* first = new AnimationTreeItem(itemData, ANIMATION, root);
                    root->appendChild(first);

                    if (seq->getName().compare(current) == 0)
                    {
                        selected = first;
                    }
                }

                if (selected)
                {
                    QModelIndex index = animationTreeModel->index(selected->row(), 0);
                    ui->treeAnimations->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
                }

                ui->treeAnimations->collapseAll();
            }


        }
        else
        {
            ui->lblBones->setText(tr("0"));
            ui->sldAnimation->setMaximum(0);
            ui->sldAnimation->setMinimum(0);
            ui->sldAnimation->setValue(0);
            ui->lblTime->setText(tr("0"));
        }
    }
    else
    {
        ui->lblBones->setText(tr("0"));
        ui->sldAnimation->setMaximum(0);
        ui->sldAnimation->setMinimum(0);
        ui->sldAnimation->setValue(0);
        ui->lblTime->setText(tr("0"));
    }

    ui->btnAdd->setEnabled(model && model->getSkeleton() && model->getSkeleton()->getAnimation()->getFrameCount() > 0);
    ui->btnRemove->setEnabled(model && model->getSkeleton() && model->getSkeleton()->getAnimation()->getFrameCount() > 0);

    ui->lblKey->setText(tr("%0").arg(ui->sldAnimation->value()));

    updating = false;
}

void SkeletonInfo::updateObject()
{

}

void SkeletonInfo::updateSimulation()
{
    Magic3D::Model* model = getModel();
    if (model && model->getSkeleton())
    {
        ui->lblCurrentFrame->setText(tr("%0").arg(model->getSkeleton()->getAnimation()->getCurrentFrame()));
    }
}


void SkeletonInfo::on_sldAnimation_valueChanged(int value)
{
    Magic3D::Model* model = getModel();
    if (model)
    {
        if (model->getSkeleton())
        {
            if (model->getSkeleton()->getAnimation()->getCurrentSequence())
            {
                model->getSkeleton()->getAnimation()->stop();
                model->getSkeleton()->getAnimation()->setCurrentSequence(-1);
            }
            model->getSkeleton()->getAnimation()->setCurrentFrame(value);

            Magic3D::AnimationFrame* frame = model->getSkeleton()->getAnimation()->getFrame(value);
            if (frame)
            {
                ui->lblTime->setText(tr("%0").arg(frame->time, 0, 'f', 3));
                ui->lblCurrentFrame->setText(tr("%0").arg(model->getSkeleton()->getAnimation()->getCurrentFrame()));
            }
        }
    }

    ui->lblKey->setText(tr("%0").arg(value));
}

void SkeletonInfo::on_btnAdd_clicked()
{
    ModelAnimationSequences sequenceWindow;

    Magic3D::Model* model = getModel();
    if (model && model->getSkeleton())
    {
        sequenceWindow.setAnimation(model->getSkeleton()->getAnimation());
    }

    sequenceWindow.setFixedSize(320, 240);
    if (sequenceWindow.exec() == QDialog::Accepted)
    {
        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void SkeletonInfo::selectionChangedSlot(const QItemSelection& newSelection, const QItemSelection& oldSelection)
{
    if (oldSelection.isEmpty())
    {

    }
    if (!newSelection.isEmpty())
    {
        QModelIndex index = newSelection.indexes().front();
        if (index.isValid())
        {
            Magic3D::Model* model = getModel();
            if (model && model->getSkeleton())
            {
                AnimationTreeItem* item = static_cast<AnimationTreeItem*>(index.internalPointer());
                if (item && item->getType() == ANIMATION)
                {
                    if (item->data(0).toString().compare(QString::fromStdString(model->getSkeleton()->getAnimation()->getCurrentSequenceName())) != 0)
                    {
                        model->getSkeleton()->getAnimation()->playAnimation(item->data(0).toString().toStdString(), 0.25f, false);

                        mainWindow->setProjectChanged(true);
                        mainWindow->update();
                    }
                }
            }
        }
    }
}

void SkeletonInfo::on_treeAnimations_doubleClicked(QModelIndex index)
{
    selectionChangedSlot(QItemSelection(index, index), QItemSelection(index, index));
}

void SkeletonInfo::on_btnRemove_clicked()
{
    Magic3D::Model* model = getModel();
    if (model && model->getSkeleton())
    {
        if (ui->treeAnimations->currentIndex().isValid())
        {
            AnimationTreeItem* item = static_cast<AnimationTreeItem*>(ui->treeAnimations->currentIndex().internalPointer());

            if (item->getType() != ANIMATION)
            {
                item = item->parent();
            }

            if (model->getSkeleton()->getAnimation()->getCurrentSequenceName().compare(item->data(0).toString().toStdString()) == 0)
            {
                model->getSkeleton()->getAnimation()->setCurrentSequence(-1);
            }
            model->getSkeleton()->getAnimation()->removeSequence(item->data(0).toString().toStdString());
            update();
            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
    }
}
