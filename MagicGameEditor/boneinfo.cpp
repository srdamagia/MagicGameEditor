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

#include "boneinfo.h"
#include "ui_boneinfo.h"
#include <magic3d/magic3d.h>

BoneInfo::BoneInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::BoneInfo)
{
    ui->setupUi(this);
}

BoneInfo::~BoneInfo()
{
    delete ui;
}

void BoneInfo::update()
{
    updating = true;
    Magic3D::Bone* bone = getBone();
    if (bone)
    {
        Magic3D::Matrix4 matrix = bone->getMatrix();
        Magic3D::Matrix3 matrixRot = matrix.getUpper3x3();

        Magic3D::Vector3 position = matrix.getTranslation();
        Magic3D::Vector3 rotation = Magic3D::Math::euler(Magic3D::Quaternion(matrixRot));
        Magic3D::Vector3 scale    = Magic3D::Vector3(length(matrixRot.getCol0()), length(matrixRot.getCol1()), length(matrixRot.getCol2()));


        ui->lblIndex->setText(tr("Index: %1").arg(bone->getIndex()));

        if (!ui->txtPosX->hasFocus())
        {
            ui->txtPosX->setValue(position.getX());
        }
        if (!ui->txtPosY->hasFocus())
        {
            ui->txtPosY->setValue(position.getY());
        }
        if (!ui->txtPosZ->hasFocus())
        {
            ui->txtPosZ->setValue(position.getZ());
        }

        if (!ui->txtRotX->hasFocus())
        {
            ui->txtRotX->setValue(rotation.getX());
        }
        if (!ui->txtRotY->hasFocus())
        {
            ui->txtRotY->setValue(rotation.getY());
        }
        if (!ui->txtRotZ->hasFocus())
        {
            ui->txtRotZ->setValue(rotation.getZ());
        }

        if (!ui->txtScaleX->hasFocus())
        {
            ui->txtScaleX->setValue(scale.getX());
        }
        if (!ui->txtScaleY->hasFocus())
        {
            ui->txtScaleY->setValue(scale.getY());
        }
        if (!ui->txtScaleZ->hasFocus())
        {
            ui->txtScaleZ->setValue(scale.getZ());
        }

        ui->chkAnimate->setChecked(bone->isAnimated());
    }
    updating = false;
}

void BoneInfo::updateObject()
{
    Magic3D::Bone* bone = getBone();
    if (!updating && bone)
    {
        Magic3D::Vector3 position = Magic3D::Vector3(ui->txtPosX->value(), ui->txtPosY->value(), ui->txtPosZ->value());
        Magic3D::Quaternion rotation = Magic3D::Math::quaternion(Magic3D::Vector3(ui->txtRotX->value(), ui->txtRotY->value(), ui->txtRotZ->value()));
        Magic3D::Vector3 scale = Magic3D::Vector3(ui->txtScaleX->value(), ui->txtScaleY->value(), ui->txtScaleZ->value());

        Magic3D::Matrix4 matrix = Magic3D::Matrix4(rotation, position);
        matrix = appendScale(matrix, scale);

        bone->setMatrix(matrix);

        bone->setAnimated(ui->chkAnimate->isChecked());

        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void BoneInfo::updateSimulation()
{
    updating = true;
    Magic3D::Bone* bone = getBone();
    if (bone)
    {
        Magic3D::Matrix4 matrix = bone->getMatrix();
        Magic3D::Matrix3 matrixRot = matrix.getUpper3x3();

        Magic3D::Vector3 position = matrix.getTranslation();
        Magic3D::Vector3 rotation = Magic3D::Math::euler(Magic3D::Quaternion(matrixRot));
        Magic3D::Vector3 scale    = Magic3D::Vector3(length(matrixRot.getCol0()), length(matrixRot.getCol1()), length(matrixRot.getCol2()));

        if (!ui->txtPosX->hasFocus())
        {
            ui->txtPosX->setValue(position.getX());
        }
        if (!ui->txtPosY->hasFocus())
        {
            ui->txtPosY->setValue(position.getY());
        }
        if (!ui->txtPosZ->hasFocus())
        {
            ui->txtPosZ->setValue(position.getZ());
        }

        if (!ui->txtRotX->hasFocus())
        {
            ui->txtRotX->setValue(rotation.getX());
        }
        if (!ui->txtRotY->hasFocus())
        {
            ui->txtRotY->setValue(rotation.getY());
        }
        if (!ui->txtRotZ->hasFocus())
        {
            ui->txtRotZ->setValue(rotation.getZ());
        }

        if (!ui->txtScaleX->hasFocus())
        {
            ui->txtScaleX->setValue(scale.getX());
        }
        if (!ui->txtScaleY->hasFocus())
        {
            ui->txtScaleY->setValue(scale.getY());
        }
        if (!ui->txtScaleZ->hasFocus())
        {
            ui->txtScaleZ->setValue(scale.getZ());
        }
    }
    updating = false;
}

void BoneInfo::on_chkAnimate_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void BoneInfo::on_txtPosX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtPosY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtPosZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtRotX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtRotY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtRotZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtScaleX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtScaleY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void BoneInfo::on_txtScaleZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}
