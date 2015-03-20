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

#include "meshesinfo.h"
#include "ui_meshesinfo.h"

#include <magic3d/magic3d.h>

MeshesInfo::MeshesInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::MeshesInfo)
{
    ui->setupUi(this);
}

MeshesInfo::~MeshesInfo()
{
    while (!meshInfo.isEmpty())
    {
        delete meshInfo.takeFirst();
    }

    delete ui;
}

void MeshesInfo::update()
{
    Magic3D::Object* object = getObject();
    if (object)
    {
        mainWindow->updateMaterialsList();

        int size = object->getMeshes()->size();
        while (meshInfo.size() < size)
        {
            MeshInfo* pnl = new MeshInfo(mainWindow);
            pnl->setPhysicsObject(object);
            pnl->setMeshIndex(meshInfo.size());
            meshInfo.append(pnl);
            ui->meshesLayout->addWidget(pnl);
        }

        while (meshInfo.size() > size)
        {
            MeshInfo* pnl = meshInfo.takeLast();
            ui->meshesLayout->removeWidget(pnl);
            delete pnl;
        }

        int vertices = 0;
        int triangles = 0;
        for (int i = 0; i < meshInfo.size(); i++)
        {
            MeshInfo* pnl = meshInfo[i];
            if (pnl)
            {
                pnl->setPhysicsObject(object);
                pnl->setMeshIndex(i);
                pnl->update();
            }

            vertices += object->getMeshes()->at(i)->getData()->getVerticesCount();
            triangles += object->getMeshes()->at(i)->getData()->getTrianglesCount();
        }

        ui->lblMeshes->setText(QString::number(meshInfo.size()));
        ui->lblVertices->setText(QString::number(vertices));
        ui->lblTriangles->setText(QString::number(triangles));
    }
}

void MeshesInfo::updateObject()
{
    mainWindow->setProjectChanged(true);
    mainWindow->update();
}
