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

#include "meshinfo.h"
#include "ui_meshinfo.h"
#include "utils.h"

#include <magic3d/magic3d.h>

MeshInfo::MeshInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::MeshInfo)
{
    ui->setupUi(this);

    meshIndex = -1;
}

MeshInfo::~MeshInfo()
{
    delete ui;
}

void MeshInfo::update()
{
    updating = true;
    Magic3D::Object* object = getObject();
    Magic3D::Mesh* mesh = getMesh();

    if (object && mesh)
    {
        ui->lblTitle->setText(QString::fromStdString(mesh->getData()->getName()));
        ui->lblVertices->setText(QString::number(mesh->getData()->getVerticesCount()));
        ui->lblTriangles->setText(QString::number(mesh->getData()->getTrianglesCount()));

        ui->chkVisible->setChecked(mesh->isVisible());
        ui->chkDoubleSide->setChecked(mesh->isDoubleSide());
        ui->chkIlluminated->setChecked(mesh->isIlluminated());
        ui->chkFog->setChecked(mesh->isFogged());
        ui->chkReflective->setChecked(mesh->isReflective());
        ui->chkGlow->setChecked(mesh->isGlow());
        ui->chkCastShadow->setChecked(mesh->isCastingShadows());
        ui->chkReceiveShadows->setChecked(mesh->isReceivingShadows());

        ui->cmbMaterial->clear();
        ui->cmbMaterial->addItems(mainWindow->getMaterialList());
        if (mesh->getMaterials()->size() > 0)
        {
            ui->cmbMaterial->setCurrentIndex(ui->cmbMaterial->findText(QString::fromStdString(mesh->getMaterials()->at(0)->getName())));
        }
        else
        {
            ui->cmbMaterial->setCurrentIndex(ui->cmbMaterial->findText(UTILS_NONE));
        }

    }
    updating = false;
}

void MeshInfo::updateObject()
{
    Magic3D::Mesh* mesh = getMesh();
    if (!updating && mesh)
    {
        mesh->setVisible(ui->chkVisible->isChecked());
        mesh->setDoubleSide(ui->chkDoubleSide->isChecked());
        mesh->setIlluminated(ui->chkIlluminated->isChecked());
        mesh->setFogged(ui->chkFog->isChecked());
        mesh->setReflective(ui->chkReflective->isChecked());
        mesh->setGlow(ui->chkGlow->isChecked());
        mesh->setCastShadows(ui->chkCastShadow->isChecked());
        mesh->setReceiveShadows(ui->chkReceiveShadows->isChecked());

        mesh->getMaterials()->clear();
        if (ui->cmbMaterial->currentIndex() > 0 && ui->cmbMaterial->currentText() != UTILS_NONE)
        {
            std::string sequence;
            if (getObject()->getRender() == Magic3D::eRENDER_2D)
            {
                sequence = static_cast<Magic3D::Sprite*>(getObject())->getCurrentSequenceName();
                static_cast<Magic3D::Sprite*>(getObject())->setCurrentSequence(NULL);
            }
            bool created = false;
            mesh->addMaterial(Magic3D::ResourceManager::getMaterials()->load(ui->cmbMaterial->currentText().toStdString(), created));
            if (getObject()->getRender() == Magic3D::eRENDER_2D)
            {
                static_cast<Magic3D::Sprite*>(getObject())->setCurrentSequence(sequence);
            }
        }

        Magic3D::Object* object = getObject();
        if (object)
        {
            object->updateBoundingBox();
        }

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void MeshInfo::setMeshIndex(int index)
{
    meshIndex = index;
}

int MeshInfo::getMeshIndex()
{
    return meshIndex;
}

Magic3D::Mesh* MeshInfo::getMesh()
{
    Magic3D::Object* object = getObject();
    Magic3D::Mesh* result = NULL;
    if (getObject())
    {
        int size = object->getMeshes()->size();
        if (meshIndex >= 0 && meshIndex < size)
        {
            result = object->getMeshes()->at(meshIndex);
        }
    }

    return result;
}

void MeshInfo::on_chkVisible_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_chkDoubleSide_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_chkIlluminated_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_chkFog_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_chkCastShadow_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_chkReceiveShadows_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_cmbMaterial_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void MeshInfo::on_chkReflective_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MeshInfo::on_chkGlow_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
