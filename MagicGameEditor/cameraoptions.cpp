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

#include "cameraoptions.h"
#include "ui_cameraoptions.h"

CameraOptions::CameraOptions(MainWindow *parent) : QWidget(parent),
    ui(new Ui::CameraOptions)
{
    mainWindow = parent;
    ui->setupUi(this);
    loading = false;
}

CameraOptions::~CameraOptions()
{
    delete ui;
}

void CameraOptions::setSpeedMoveFactor(float speed)
{
    if (!loading)
    {
        loading = true;
        ui->sldMove->setValue(speed * 100.0f);
        mainWindow->getProject()->setCameraMove(speed);
        mainWindow->update();
        loading = false;
    }
}

void CameraOptions::setSpeedLookFactor(float speed)
{
    if (!loading)
    {
        loading = true;
        ui->sldLook->setValue(speed * 100.0f);
        mainWindow->getProject()->setCameraLook(speed);
        mainWindow->update();
        loading = false;
    }
}

float CameraOptions::getSpeedMoveFactor()
{
    return ui->sldMove->value() / 100.0f;
}

float CameraOptions::getSpeedLookFactor()
{
    return ui->sldLook->value() / 100.0f;
}

float CameraOptions::getSpeedMove()
{
    return getSpeedMoveFactor() * CAMERA_MOVE_SPEED;
}

float CameraOptions::getSpeedLook()
{
    return getSpeedLookFactor() * CAMERA_LOOK_SPEED;
}

void CameraOptions::on_sldMove_valueChanged(int value)
{
    setSpeedMoveFactor((float)value / 100.0f);
}

void CameraOptions::on_sldLook_valueChanged(int value)
{
    setSpeedLookFactor((float)value / 100.0f);
}
