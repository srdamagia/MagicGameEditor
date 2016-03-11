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

#include "materialvar_float.h"
#include "ui_materialvar_float.h"

MaterialVar_Float::MaterialVar_Float(MainWindow *mainWindow) :
    MaterialVar(mainWindow),
    ui(new Ui::MaterialVar_Float)
{
    ui->setupUi(this);
}

MaterialVar_Float::~MaterialVar_Float()
{
    delete ui;
}

void MaterialVar_Float::update()
{
    updating = true;
    if (getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Float* variable = (Magic3D::MaterialVar_Float*)getMaterial()->getVar(getMaterialVarIndex());
        Magic3D::SHADER_VAR type = variable->getType();

        ui->label->setText(QString::fromStdString(variable->getName()));

        bool vX = type >= Magic3D::eSHADER_VAR_FLOAT      && type <= Magic3D::eSHADER_VAR_FLOAT_VEC4;
        bool vY = type >= Magic3D::eSHADER_VAR_FLOAT_VEC2 && type <= Magic3D::eSHADER_VAR_FLOAT_VEC4;
        bool vZ = type >= Magic3D::eSHADER_VAR_FLOAT_VEC3 && type <= Magic3D::eSHADER_VAR_FLOAT_VEC4;
        bool vW = type >= Magic3D::eSHADER_VAR_FLOAT_VEC4 && type <= Magic3D::eSHADER_VAR_FLOAT_VEC4;

        ui->txtX->setVisible(vX);
        ui->txtY->setVisible(vY);
        ui->txtZ->setVisible(vZ);
        ui->txtW->setVisible(vW);

        ui->lblX->setVisible(vX);
        ui->lblY->setVisible(vY);
        ui->lblZ->setVisible(vZ);
        ui->lblW->setVisible(vW);

        if (vX && !ui->txtX->hasFocus())
        {
            ui->txtX->setValue(variable->getValue(0));
        }
        if (vY && !ui->txtY->hasFocus())
        {
            ui->txtY->setValue(variable->getValue(1));
        }
        if (vZ && !ui->txtZ->hasFocus())
        {
            ui->txtZ->setValue(variable->getValue(2));
        }
        if (vW && !ui->txtW->hasFocus())
        {
            ui->txtW->setValue(variable->getValue(3));
        }

        ui->lblX->setText(vY ? "x:" : ":");
    }
    updating = false;
}

void MaterialVar_Float::updateObject()
{
    if (!updating && getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Float* variable = (Magic3D::MaterialVar_Float*)getMaterial()->getVar(getMaterialVarIndex());

        if (ui->txtX->isVisible())
        {
            variable->setValue(0, ui->txtX->value());
        }

        if (ui->txtY->isVisible())
        {
            variable->setValue(1, ui->txtY->value());
        }

        if (ui->txtZ->isVisible())
        {
            variable->setValue(2, ui->txtZ->value());
        }

        if (ui->txtW->isVisible())
        {
            variable->setValue(3, ui->txtW->value());
        }

        needSave();
        update();
        mainWindow->update();
    }
}

void MaterialVar_Float::on_txtX_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Float::on_txtY_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Float::on_txtZ_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Float::on_txtW_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}
