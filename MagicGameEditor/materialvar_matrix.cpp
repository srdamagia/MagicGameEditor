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

#include "materialvar_matrix.h"
#include "ui_materialvar_matrix.h"

MaterialVar_Matrix::MaterialVar_Matrix(MainWindow* mainWindow) :
    MaterialVar(mainWindow),
    ui(new Ui::MaterialVar_Matrix)
{
    ui->setupUi(this);
}

MaterialVar_Matrix::~MaterialVar_Matrix()
{
    delete ui;
}

void MaterialVar_Matrix::update()
{
    updating = true;
    if (getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Matrix* variable = (Magic3D::MaterialVar_Matrix*)getMaterial()->getVar(getMaterialVarIndex());
        Magic3D::SHADER_VAR type = variable->getType();

        ui->label->setText(QString::fromStdString(variable->getName()));

        bool matrix[4][4];
        matrix[0][0] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT2 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[0][1] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT2 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[0][2] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT3 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[0][3] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;

        matrix[1][0] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT2 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[1][1] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT2 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[1][2] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT3 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[1][3] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;

        matrix[2][0] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT3 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[2][1] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT3 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[2][2] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT3 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[2][3] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;

        matrix[3][0] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[3][1] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[3][2] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;
        matrix[3][3] = type >= Magic3D::eSHADER_VAR_FLOAT_MAT4 && type <= Magic3D::eSHADER_VAR_FLOAT_MAT4;

        ui->txt_0_0->setVisible(matrix[0][0]);
        ui->txt_0_1->setVisible(matrix[0][1]);
        ui->txt_0_2->setVisible(matrix[0][2]);
        ui->txt_0_3->setVisible(matrix[0][3]);

        ui->txt_1_0->setVisible(matrix[1][0]);
        ui->txt_1_1->setVisible(matrix[1][1]);
        ui->txt_1_2->setVisible(matrix[1][2]);
        ui->txt_1_3->setVisible(matrix[1][3]);

        ui->txt_2_0->setVisible(matrix[2][0]);
        ui->txt_2_1->setVisible(matrix[2][1]);
        ui->txt_2_2->setVisible(matrix[2][2]);
        ui->txt_2_3->setVisible(matrix[2][3]);

        ui->txt_3_0->setVisible(matrix[3][0]);
        ui->txt_3_1->setVisible(matrix[3][1]);
        ui->txt_3_2->setVisible(matrix[3][2]);
        ui->txt_3_3->setVisible(matrix[3][3]);


        if (matrix[0][0] && !ui->txt_0_0->hasFocus())
        {
            ui->txt_0_0->setValue(variable->getValue(0, 0));
        }
        if (matrix[0][1] && !ui->txt_0_1->hasFocus())
        {
            ui->txt_0_1->setValue(variable->getValue(0, 1));
        }
        if (matrix[0][2] && !ui->txt_0_2->hasFocus())
        {
            ui->txt_0_2->setValue(variable->getValue(0, 2));
        }
        if (matrix[0][3] && !ui->txt_0_3->hasFocus())
        {
            ui->txt_0_3->setValue(variable->getValue(0, 3));
        }

        if (matrix[1][0] && !ui->txt_1_0->hasFocus())
        {
            ui->txt_1_0->setValue(variable->getValue(1, 0));
        }
        if (matrix[1][1] && !ui->txt_1_1->hasFocus())
        {
            ui->txt_1_1->setValue(variable->getValue(1, 1));
        }
        if (matrix[1][2] && !ui->txt_1_2->hasFocus())
        {
            ui->txt_1_2->setValue(variable->getValue(1, 2));
        }
        if (matrix[1][3] && !ui->txt_1_3->hasFocus())
        {
            ui->txt_1_3->setValue(variable->getValue(1, 3));
        }

        if (matrix[2][0] && !ui->txt_2_0->hasFocus())
        {
            ui->txt_2_0->setValue(variable->getValue(2, 0));
        }
        if (matrix[2][1] && !ui->txt_2_1->hasFocus())
        {
            ui->txt_2_1->setValue(variable->getValue(2, 1));
        }
        if (matrix[2][2] && !ui->txt_2_2->hasFocus())
        {
            ui->txt_2_2->setValue(variable->getValue(2, 2));
        }
        if (matrix[2][3] && !ui->txt_2_3->hasFocus())
        {
            ui->txt_2_3->setValue(variable->getValue(2, 3));
        }

        if (matrix[3][0] && !ui->txt_3_0->hasFocus())
        {
            ui->txt_3_0->setValue(variable->getValue(3, 0));
        }
        if (matrix[3][1] && !ui->txt_3_1->hasFocus())
        {
            ui->txt_3_1->setValue(variable->getValue(3, 1));
        }
        if (matrix[3][2] && !ui->txt_3_2->hasFocus())
        {
            ui->txt_3_2->setValue(variable->getValue(3, 2));
        }
        if (matrix[3][3] && !ui->txt_3_3->hasFocus())
        {
            ui->txt_3_3->setValue(variable->getValue(3, 3));
        }
    }
    updating = false;
}

void MaterialVar_Matrix::updateObject()
{
    if (!updating && getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Matrix* variable = (Magic3D::MaterialVar_Matrix*)getMaterial()->getVar(getMaterialVarIndex());

        if (ui->txt_0_0->isVisible())
        {
            variable->setValue(0, 0, ui->txt_0_0->value());
        }
        if (ui->txt_0_1->isVisible())
        {
            variable->setValue(0, 1, ui->txt_0_1->value());
        }
        if (ui->txt_0_2->isVisible())
        {
            variable->setValue(0, 2, ui->txt_0_2->value());
        }
        if (ui->txt_0_3->isVisible())
        {
            variable->setValue(0, 3, ui->txt_0_3->value());
        }

        if (ui->txt_1_0->isVisible())
        {
            variable->setValue(1, 0, ui->txt_1_0->value());
        }
        if (ui->txt_1_1->isVisible())
        {
            variable->setValue(1, 1, ui->txt_1_1->value());
        }
        if (ui->txt_1_2->isVisible())
        {
            variable->setValue(1, 2, ui->txt_1_2->value());
        }
        if (ui->txt_1_3->isVisible())
        {
            variable->setValue(1, 3, ui->txt_1_3->value());
        }

        if (ui->txt_2_0->isVisible())
        {
            variable->setValue(2, 0, ui->txt_2_0->value());
        }
        if (ui->txt_2_1->isVisible())
        {
            variable->setValue(2, 1, ui->txt_2_1->value());
        }
        if (ui->txt_2_2->isVisible())
        {
            variable->setValue(2, 2, ui->txt_2_2->value());
        }
        if (ui->txt_2_3->isVisible())
        {
            variable->setValue(2, 3, ui->txt_2_3->value());
        }

        if (ui->txt_3_0->isVisible())
        {
            variable->setValue(3, 0, ui->txt_3_0->value());
        }
        if (ui->txt_3_1->isVisible())
        {
            variable->setValue(3, 1, ui->txt_3_1->value());
        }
        if (ui->txt_3_2->isVisible())
        {
            variable->setValue(3, 2, ui->txt_3_2->value());
        }
        if (ui->txt_3_3->isVisible())
        {
            variable->setValue(3, 3, ui->txt_3_3->value());
        }

        needSave();
        update();
        mainWindow->update();
    }
}

void MaterialVar_Matrix::on_txt_0_0_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_0_1_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_0_2_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_0_3_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_1_0_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_1_1_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_1_2_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_1_3_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_2_0_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_2_1_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_2_2_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_2_3_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_3_0_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_3_1_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_3_2_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void MaterialVar_Matrix::on_txt_3_3_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}
