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

#include "tweensinfo.h"
#include "ui_tweensinfo.h"

#include <magic3d/magic3d.h>

TweensInfo::TweensInfo(MainWindow* mainWindow) :
   Magic3DObjectPanel(mainWindow),
    ui(new Ui::TweensInfo)
{
    ui->setupUi(this);

    tween = NULL;

    toolBar = new QToolBar(this);
    toolBar->addAction(ui->actionTranslate);
    toolBar->addAction(ui->actionRotate);
    toolBar->addAction(ui->actionScale);
    toolBar->addAction(ui->actionAlpha);
    toolBar->addAction(ui->actionGo_To);
    toolBar->addAction(ui->actionLook_At);
    toolBar->addAction(ui->actionDelay);
    toolBar->addAction(ui->actionCustom);
    ui->layoutToolBar->addWidget(toolBar);
}

TweensInfo::~TweensInfo()
{
    tween = NULL;

    while (!tweenInfo.isEmpty())
    {
        delete tweenInfo.takeFirst();
    }

    if (toolBar)
    {
        delete toolBar;
        toolBar = NULL;
    }

    delete ui;
}

void TweensInfo::update()
{
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    if (physicsObject)
    {
        mainWindow->updateParentsList(true, false);
        parentsList = mainWindow->getParentsList();

        int count = physicsObject->getTweenCount();
        while (tweenInfo.size() < count)
        {
            TweenInfo* pnl = new TweenInfo(mainWindow, this);
            pnl->setPhysicsObject(physicsObject);
            pnl->setTweenIndex(tweenInfo.size());
            tweenInfo.append(pnl);
            ui->layoutTweens->addWidget(pnl);
        }
        while (tweenInfo.size() > count)
        {
            TweenInfo* pnl = tweenInfo.takeLast();
            ui->layoutTweens->removeWidget(pnl);
            delete pnl;
        }

        for (int i = 0; i < tweenInfo.size(); i++)
        {
            TweenInfo* pnl = tweenInfo[i];
            if (pnl)
            {
                pnl->setPhysicsObject(physicsObject);
                pnl->setTweenIndex(i);
                pnl->update();
            }
        }
    }
}

void TweensInfo::updateObject()
{
    mainWindow->setProjectChanged(true);
    mainWindow->update();

    update();
}

void TweensInfo::saveObject(Magic3D::Tween* tween)
{
    if (this->tween)
    {
        resetObject();
    }
    alpha.clear();
    if (tween && getPhysicsObject())
    {
        position = getPhysicsObject()->getPosition();
        rotation = getPhysicsObject()->getRotation();
        scale = getPhysicsObject()->getScale();


        if (getObject() && !getBone())
        {
            std::vector<Magic3D::Mesh*>* meshes = getObject()->getMeshes();
            std::vector<Magic3D::Mesh*>::const_iterator it_m = meshes->begin();

            while (it_m != meshes->end())
            {
                Magic3D::Mesh* mesh = *it_m++;
                std::vector<Magic3D::Material*>* materials = mesh->getMaterials();
                std::vector<Magic3D::Material*>::const_iterator it_mat = materials->begin();

                while (it_mat != materials->end())
                {
                    Magic3D::Material* material = *it_mat++;

                    Magic3D::ColorRGBA color = material->getColorDiffuse();
                    alpha.append(color.a);
                }
            }
        }

        this->tween = tween;
        this->nextTweenObject = QString::fromStdString(this->tween->getNextTweenObject());
        this->nextTweenBone = QString::fromStdString(this->tween->getNextTweenBone());
        this->nextTweenIndex = this->tween->getNextTweenIndex();
        this->tween->setNextTween("", "", 0);
    }
}

void TweensInfo::resetObject(bool update)
{
    if (tween && getPhysicsObject())
    {
        tween->stop();
        tween->setNextTween(nextTweenObject.toStdString(), nextTweenBone.toStdString(), nextTweenIndex);
        getPhysicsObject()->setPosition(position);
        getPhysicsObject()->setRotation(rotation);
        getPhysicsObject()->setScale(scale);
        getPhysicsObject()->resetPhysics();

        if (getObject() && !getBone())
        {
            std::vector<Magic3D::Mesh*>* meshes = getObject()->getMeshes();
            std::vector<Magic3D::Mesh*>::const_iterator it_m = meshes->begin();

            int index = 0;
            while (it_m != meshes->end())
            {
                Magic3D::Mesh* mesh = *it_m++;
                std::vector<Magic3D::Material*>* materials = mesh->getMaterials();
                std::vector<Magic3D::Material*>::const_iterator it_mat = materials->begin();

                while (it_mat != materials->end())
                {
                    Magic3D::Material* material = *it_mat++;

                    Magic3D::ColorRGBA color = material->getColorDiffuse();
                    color.a = alpha[index++];
                    material->setColorDiffuse(color);
                }
            }
        }

        this->tween = NULL;
        if (update)
        {
            this->update();
        }
    }
}

QStringList& TweensInfo::getParentsList()
{
    return parentsList;
}

void TweensInfo::on_actionTranslate_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenTranslate());
        updateObject();
    }
}

void TweensInfo::on_actionRotate_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenRotate());
        updateObject();
    }
}

void TweensInfo::on_actionScale_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenScale());
        updateObject();
    }
}

void TweensInfo::on_actionAlpha_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenAlpha());
        updateObject();
    }
}

void TweensInfo::on_actionGo_To_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenGoTo());
        updateObject();
    }
}

void TweensInfo::on_actionLook_At_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenLookAt());
        updateObject();
    }
}

void TweensInfo::on_actionDelay_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::TweenDelay());
        updateObject();
    }
}

void TweensInfo::on_actionCustom_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addTween(new Magic3D::Tween(Magic3D::eTWEEN_CUSTOM));
        updateObject();
    }
}
