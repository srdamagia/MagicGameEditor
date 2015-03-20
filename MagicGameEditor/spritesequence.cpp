/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2015 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2015
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

#include "spritesequence.h"
#include "ui_spritesequence.h"
#include "SequenceTreeItem.h"
#include "sequencewindow.h"

#include <magic3d/magic3d.h>

SpriteSequence::SpriteSequence(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::SpriteSequence)
{
    ui->setupUi(this);

    sequenceTreeModel = new SequenceTreeModel("default", ui->treeSequences, this);

    ui->treeSequences->setModel(sequenceTreeModel);
    ui->treeSequences->setColumnWidth(0, 75);
    ui->treeSequences->setColumnWidth(1, 50);
    ui->treeSequences->setColumnWidth(2, 50);
    ui->treeSequences->setColumnWidth(3, 50);
    ui->treeSequences->setColumnWidth(4, 50);

    QItemSelectionModel *selectionModel= ui->treeSequences->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,           SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

}

SpriteSequence::~SpriteSequence()
{
    if (sequenceTreeModel)
    {
        ui->treeSequences->setModel(NULL);
        delete sequenceTreeModel;
        sequenceTreeModel = NULL;
    }

    delete ui;
}

void SpriteSequence::update()
{
    Magic3D::Object* object = getObject();
    if (object)
    {
        SequenceTreeItem* root = sequenceTreeModel->getRoot();
        sequenceTreeModel->removeAll();

        Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;

        ui->btnAdd->setEnabled(true);
        ui->btnRemove->setEnabled(sprite->getSequences() && sprite->getSequences()->size() > 0);

        std::string current;

        if (sprite->getCurrentSequence())
        {
            current = sprite->getCurrentSequence()->getName();
        }

        if (sprite)
        {
            std::vector<Magic3D::SpriteSequence*>* sequences = sprite->getSequences();
            if (sequences)
            {
                SequenceTreeItem* selected = NULL;

                std::vector<Magic3D::SpriteSequence*>::const_iterator it_s = sequences->begin();
                while (it_s != sequences->end())
                {
                    Magic3D::SpriteSequence* seq = *it_s++;

                    QList<QVariant> itemData;
                    itemData << QString::fromStdString(seq->getName());
                    itemData << tr(" ");
                    itemData << tr(" ");
                    itemData << tr(" ");
                    itemData << tr(" ");

                    SequenceTreeItem* first = new SequenceTreeItem(itemData, SEQUENCE, root);
                    root->appendChild(first);

                    int count = seq->getFrameCount();

                    for (int i = 0; i < count; i++)
                    {
                        Magic3D::Vector4 frame = seq->getFrame(i);

                        itemData.clear();
                        itemData << QString("%1").arg(i+1);
                        itemData << QString("%1").arg(frame.getX());
                        itemData << QString("%1").arg(frame.getY());
                        itemData << QString("%1").arg(frame.getZ());
                        itemData << QString("%1").arg(frame.getW());

                        SequenceTreeItem* child = new SequenceTreeItem(itemData, FRAME, first);
                        first->appendChild(child);
                    }

                    if (seq->getName().compare(current) == 0)
                    {
                        selected = first;
                    }
                }

                if (selected)
                {
                    QModelIndex index = sequenceTreeModel->index(selected->row(), 0);
                    ui->treeSequences->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
                }

                ui->treeSequences->collapseAll();
            }
        }
    }
    else
    {
        ui->btnAdd->setEnabled(false);
        ui->btnRemove->setEnabled(false);
    }
}

void SpriteSequence::updateObject()
{

}

void SpriteSequence::showSequencesWindow(bool edit)
{
    SequenceWindow sequenceWindow;

    Magic3D::Sprite* sprite = (Magic3D::Sprite*)getObject();
    if (!edit && sprite)
    {
        sprite->setCurrentSequence(NULL);
    }

    sequenceWindow.setObject(sprite);
    if (sequenceWindow.exec() == QDialog::Accepted)
    {
        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void SpriteSequence::selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    if (oldSelection.isEmpty())
    {

    }
    if (!newSelection.isEmpty())
    {
        QModelIndex index = newSelection.indexes().front();
        if (index.isValid())
        {
            Magic3D::Sprite* sprite = (Magic3D::Sprite*)getObject();
            if (sprite)
            {
                SequenceTreeItem* item = static_cast<SequenceTreeItem*>(index.internalPointer());
                if (item && item->getType() == SEQUENCE)
                {
                    sprite->setCurrentSequence(item->data(0).toString().toStdString());
                    sprite->setLooping(true);
                    sprite->play();
                }
            }
        }
    }
}

void SpriteSequence::on_treeSequences_doubleClicked(QModelIndex index)
{
    selectionChangedSlot(QItemSelection(index, index), QItemSelection(index, index));
    showSequencesWindow(true);
}

void SpriteSequence::on_btnAdd_clicked()
{
    showSequencesWindow(false);
}

void SpriteSequence::on_btnRemove_clicked()
{
    Magic3D::Object* object = getObject();
    if (object && object->getType() == Magic3D::eOBJECT_SPRITE)
    {
        Magic3D::Sprite* sprite = (Magic3D::Sprite*)getObject();
        if (sprite)
        {
            if (ui->treeSequences->currentIndex().isValid())
            {
                SequenceTreeItem* item = static_cast<SequenceTreeItem*>(ui->treeSequences->currentIndex().internalPointer());

                if (item->getType() != SEQUENCE)
                {
                    item = item->parent();
                }

                if (sprite->getCurrentSequenceName().compare(item->data(0).toString().toStdString()) == 0)
                {
                    sprite->setCurrentSequence(NULL);
                }
                sprite->removeSequence(item->data(0).toString().toStdString());
                update();
                mainWindow->setProjectChanged(true);
                mainWindow->update();
            }
        }
    }
}
