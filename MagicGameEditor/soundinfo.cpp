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

#include "soundinfo.h"
#include "ui_soundinfo.h"
#include "magic3d/magic3d.h"

SoundInfo::SoundInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::SoundInfo)
{
    ui->setupUi(this);
}

SoundInfo::~SoundInfo()
{
    delete ui;
}

void SoundInfo::update()
{
    updating = true;

    Magic3D::Object* object = getObject();

    if (object && object->getType() == Magic3D::eOBJECT_SOUND)
    {
        Magic3D::Sound* sound = (Magic3D::Sound*)object;

        ui->sldVolume->setValue(sound->getGain() * 1000.0f);
        ui->sldPitch->setValue(sound->getPitch() * 1000.0f);

        if (!ui->txtDistance->hasFocus())
        {
            ui->txtDistance->setValue(sound->getDistance());
        }
        ui->chkLoop->setChecked(sound->isLooping());
    }

    updating = false;
}

void SoundInfo::updateObject()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_SOUND)
    {
        Magic3D::Sound* sound = (Magic3D::Sound*)object;

        sound->setGain(ui->sldVolume->value() * 0.001f);
        sound->setPitch(ui->sldPitch->value() * 0.001f);
        sound->setDistance(ui->txtDistance->value());
        sound->setLooping(ui->chkLoop->isChecked());

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void SoundInfo::on_sldVolume_valueChanged(int value)
{
    if (value)
    {

    }
    updateObject();
}

void SoundInfo::on_sldPitch_valueChanged(int value)
{
    if (value)
    {

    }
    updateObject();
}

void SoundInfo::on_chkLoop_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void SoundInfo::on_btnPlay_clicked()
{
    Magic3D::Object* object = getObject();
    if (object && object->getType() == Magic3D::eOBJECT_SOUND)
    {
        Magic3D::Sound* sound = (Magic3D::Sound*)object;
        sound->play();
        update();
    }
}

void SoundInfo::on_btnStop_clicked()
{
    Magic3D::Object* object = getObject();
    if (object && object->getType() == Magic3D::eOBJECT_SOUND)
    {
        Magic3D::Sound* sound = (Magic3D::Sound*)object;
        sound->stop();
        update();
    }
}

void SoundInfo::on_txtDistance_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}
