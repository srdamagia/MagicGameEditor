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

#include "camerainfo.h"
#include "ui_camerainfo.h"
#include <magic3d/magic3d.h>

CameraInfo::CameraInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::CameraInfo)
{
    ui->setupUi(this);
}

CameraInfo::~CameraInfo()
{
    delete ui;
}

void CameraInfo::update()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getType() == Magic3D::eOBJECT_CAMERA)
    {
        Magic3D::Camera* camera = (Magic3D::Camera*)object;

        ui->sldAngle->setValue(camera->getFovy());
        if (!ui->txtNear->hasFocus())
        {
            ui->txtNear->setValue(camera->getNear());
        }

        if (!ui->txtFar->hasFocus())
        {
            ui->txtFar->setValue(camera->getFar());
        }

        if (camera->getProjectionType() == Magic3D::ePROJECTION_PERSPECTIVE)
        {
            ui->radPerspective->setChecked(true);
        }
        else
        {
            ui->radOrthographic->setChecked(true);
        }

        ui->lblAngle->setText(tr("Angle: %1%2").arg(ui->sldAngle->value()).arg(QChar(0x00B0)));

        if (!ui->txtInteraxial->hasFocus())
        {
            ui->txtInteraxial->setValue(camera->getInteraxial());
        }
        if (!ui->txtInterpupillary->hasFocus())
        {
            ui->txtInterpupillary->setValue(camera->getInterpupillary());
        }
        if (!ui->txtNeckDistance->hasFocus())
        {
            ui->txtNeckDistance->setValue(camera->getNeckDistance());
        }

        if (!ui->txtDistortion->hasFocus())
        {
            ui->txtDistortion->setValue(camera->getDistortion());
        }
        if (!ui->txtDistortionCube->hasFocus())
        {
            ui->txtDistortionCube->setValue(camera->getDistortionCube());
        }
        if (!ui->txtDistortionX->hasFocus())
        {
            ui->txtDistortionX->setValue(camera->getDistortionX());
        }
        if (!ui->txtDistortionY->hasFocus())
        {
            ui->txtDistortionY->setValue(camera->getDistortionY());
        }
    }
    updating = false;
}

void CameraInfo::updateObject()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getType() == Magic3D::eOBJECT_CAMERA)
    {
        Magic3D::Camera* camera = (Magic3D::Camera*)object;

        camera->setView(ui->sldAngle->value(), ui->txtNear->value(), ui->txtFar->value());

        if (ui->radPerspective->isChecked())
        {
            Magic3D::Camera* ortho = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getOrthographic();

            if (ortho && ortho->getName().compare(camera->getName()) == 0)
            {
                Magic3D::Renderer::getInstance()->getCurrentViewPort()->setOrthographic(NULL);
            }

            camera->setProjectionType(Magic3D::ePROJECTION_PERSPECTIVE);
            Magic3D::Renderer::getInstance()->getCurrentViewPort()->setPerspective(camera);
        }
        else
        {
            Magic3D::Camera* perspective = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getPerspective();

            if (perspective && perspective->getName().compare(camera->getName()) == 0)
            {
                Magic3D::Renderer::getInstance()->getCurrentViewPort()->setPerspective(NULL);
            }

            camera->setProjectionType(Magic3D::ePROJECTION_ORTHOGRAPHIC);
            Magic3D::Renderer::getInstance()->getCurrentViewPort()->setOrthographic(camera);
        }

        camera->setInteraxial(ui->txtInteraxial->value());
        camera->setInterpupillary(ui->txtInterpupillary->value());
        camera->setNeckDistance(ui->txtNeckDistance->value());

        camera->setDistortion(ui->txtDistortion->value());
        camera->setDistortionCube(ui->txtDistortionCube->value());
        camera->setDistortionX(ui->txtDistortionX->value());
        camera->setDistortionY(ui->txtDistortionY->value());

        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void CameraInfo::on_radPerspective_toggled(bool checked)
{
    if (checked)
    {
        updateObject();
    }
}

void CameraInfo::on_radOrthographic_toggled(bool checked)
{
    if (checked)
    {
        updateObject();
    }
}

void CameraInfo::on_sldAngle_valueChanged(int value)
{
    if (value)
    {

    }
    updateObject();
}

void CameraInfo::on_txtNear_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtFar_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtInteraxial_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtInterpupillary_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtNeckDistance_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtDistortion_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtDistortionCube_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtDistortionX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void CameraInfo::on_txtDistortionY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}
