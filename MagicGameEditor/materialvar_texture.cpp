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

#include "materialvar_texture.h"
#include "ui_materialvar_texture.h"
#include "utils.h"

#include <magic3d/magic3d.h>

MaterialVar_Texture::MaterialVar_Texture(MainWindow* mainWindow) :
    MaterialVar(mainWindow),
    ui(new Ui::MaterialVar_Texture)
{
    ui->setupUi(this);
}

MaterialVar_Texture::~MaterialVar_Texture()
{
    delete ui;
}

void MaterialVar_Texture::updateTextureCombo(QStringList textures)
{
    ui->cmbTexture->clear();
    ui->cmbTexture->addItems(textures);
}

void MaterialVar_Texture::update()
{
    updating = true;
    if (getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Texture* variable = (Magic3D::MaterialVar_Texture*)getMaterial()->getVar(getMaterialVarIndex());
        //Magic3D::SHADER_VAR type = variable->getType();

        ui->label->setText(QString::fromStdString(variable->getName()));

        Magic3D::Texture* texture = variable->getValue();
        if (texture)
        {
            ui->cmbTexture->setCurrentIndex(ui->cmbTexture->findText(QString::fromStdString(texture->getName())));
        }
        else
        {
            ui->cmbTexture->setCurrentIndex(ui->cmbTexture->findText(UTILS_NONE));
        }
    }
    updating = false;
}

void MaterialVar_Texture::updateObject()
{
    if (!updating && getMaterial() && getMaterial()->getVar(getMaterialVarIndex()))
    {
        Magic3D::MaterialVar_Texture* variable = (Magic3D::MaterialVar_Texture*)getMaterial()->getVar(getMaterialVarIndex());

        if (ui->cmbTexture->currentIndex() > 0)
        {
            variable->setValue(Magic3D::ResourceManager::getTextures()->get(ui->cmbTexture->currentText().toStdString()));
        }
        else
        {
            variable->setValue(NULL);
        }

        needSave();
        update();
        mainWindow->update();
    }
}

void MaterialVar_Texture::on_cmbTexture_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}
