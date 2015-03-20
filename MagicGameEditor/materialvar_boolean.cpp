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

#include "materialvar_boolean.h"
#include "ui_materialvar_boolean.h"

MaterialVar_Boolean::MaterialVar_Boolean(MainWindow* mainWindow) :
    MaterialVar(mainWindow),
    ui(new Ui::MaterialVar_Boolean)
{
    ui->setupUi(this);
}

MaterialVar_Boolean::~MaterialVar_Boolean()
{
    delete ui;
}

void MaterialVar_Boolean::update()
{
    updating = true;
    if (getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Boolean* variable = (Magic3D::MaterialVar_Boolean*)getMaterial()->getVar(getMaterialVarIndex());
        Magic3D::SHADER_VAR type = variable->getType();

        ui->label->setText(QString::fromStdString(variable->getName()));

        bool vX = type >= Magic3D::eSHADER_VAR_BOOL      && type <= Magic3D::eSHADER_VAR_BOOL_VEC4;
        bool vY = type >= Magic3D::eSHADER_VAR_BOOL_VEC2 && type <= Magic3D::eSHADER_VAR_BOOL_VEC4;
        bool vZ = type >= Magic3D::eSHADER_VAR_BOOL_VEC3 && type <= Magic3D::eSHADER_VAR_BOOL_VEC4;
        bool vW = type >= Magic3D::eSHADER_VAR_BOOL_VEC4 && type <= Magic3D::eSHADER_VAR_BOOL_VEC4;

        ui->txtX->setVisible(vX);
        ui->txtY->setVisible(vY);
        ui->txtZ->setVisible(vZ);
        ui->txtW->setVisible(vW);

        if (vX && !ui->txtX->hasFocus())
        {
            ui->txtX->setChecked(variable->getValue(0));
        }
        if (vY && !ui->txtY->hasFocus())
        {
            ui->txtY->setChecked(variable->getValue(1));
        }
        if (vZ && !ui->txtZ->hasFocus())
        {
            ui->txtZ->setChecked(variable->getValue(2));
        }
        if (vW && !ui->txtW->hasFocus())
        {
            ui->txtW->setChecked(variable->getValue(3));
        }

        ui->txtX->setText(vY ? "x:" : ":");
    }
    updating = false;
}

void MaterialVar_Boolean::updateObject()
{
    if (!updating && getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Boolean* variable = (Magic3D::MaterialVar_Boolean*)getMaterial()->getVar(getMaterialVarIndex());

        if (ui->txtX->isVisible())
        {
            variable->setValue(0, ui->txtX->isChecked());
        }

        if (ui->txtY->isVisible())
        {
            variable->setValue(1, ui->txtY->isChecked());
        }

        if (ui->txtZ->isVisible())
        {
            variable->setValue(2, ui->txtZ->isChecked());
        }

        if (ui->txtW->isVisible())
        {
            variable->setValue(3, ui->txtW->isChecked());
        }

        needSave();
        update();
        mainWindow->update();
    }
}

void MaterialVar_Boolean::on_txtX_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialVar_Boolean::on_txtY_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialVar_Boolean::on_txtZ_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialVar_Boolean::on_txtW_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
