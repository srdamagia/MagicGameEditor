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

#include "waterinfo.h"
#include "ui_waterinfo.h"

WaterInfo::WaterInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::WaterInfo)
{
    ui->setupUi(this);
}

WaterInfo::~WaterInfo()
{
    delete ui;
}

void WaterInfo::update()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getType() == Magic3D::eOBJECT_WATER)
    {
        Magic3D::Water* water = (Magic3D::Water*)object;

        ui->txtLength->setValue(water->getLength());
        ui->txtWidth->setValue(water->getWidth());
        ui->txtTileLength->setValue(water->getTileLength());
        ui->txtTileWidth->setValue(water->getTileWidth());
    }
    updating = false;
}

void WaterInfo::updateObject()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getType() == Magic3D::eOBJECT_WATER)
    {
        Magic3D::Water* water = (Magic3D::Water*)object;

        water->setSize(ui->txtLength->value(), ui->txtWidth->value(), ui->txtTileLength->value(), ui->txtTileWidth->value());

        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void WaterInfo::on_txtLength_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void WaterInfo::on_txtWidth_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void WaterInfo::on_txtTileLength_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void WaterInfo::on_txtTileWidth_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void WaterInfo::on_btnUpdate_clicked()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getType() == Magic3D::eOBJECT_WATER)
    {
        Magic3D::Water* water = (Magic3D::Water*)object;
        water->generateWater();
    }
}
