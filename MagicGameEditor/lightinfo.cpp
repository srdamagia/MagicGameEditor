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

#include "lightinfo.h"
#include "ui_lightinfo.h"
#include "magic3d/magic3d.h"

LightInfo::LightInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::LightInfo)
{
    ui->setupUi(this);
}

LightInfo::~LightInfo()
{
    delete ui;
}

void LightInfo::update()
{
    updating = true;

    Magic3D::Object* object = getObject();

    if (object && object->getType() == Magic3D::eOBJECT_LIGHT)
    {
        Magic3D::Light* light = (Magic3D::Light*)object;
        setButtonColor(ui->btnColorAmbient,  light->getColorAmbient());
        setButtonColor(ui->btnColorDiffuse,  light->getColorDiffuse());
        setButtonColor(ui->btnColorSpecular, light->getColorSpecular());

        switch (light->getLightType())
        {
            case Magic3D::eLIGHT_TYPE_DIRECTIONAL: ui->rbDirectional->setChecked(true); break;
            case Magic3D::eLIGHT_TYPE_POINT: ui->rbPoint->setChecked(true); break;
            case Magic3D::eLIGHT_TYPE_SPOT: ui->rbSpot->setChecked(true); break;
        }

        ui->chkShadows->setChecked(light->isShadows());

        if (!ui->txtSpotCutoff->hasFocus())
        {
            ui->txtSpotCutoff->setValue(light->getCutOff());
        }
        ui->sldSpotExponent->setValue(light->getExponent());

        if (!ui->txtAttenuationConstant->hasFocus())
        {
            ui->txtAttenuationConstant->setValue(light->getAttenuationConstant());
        }
        if (!ui->txtAttenuationLinear->hasFocus())
        {
            ui->txtAttenuationLinear->setValue(light->getAttenuationLinear());
        }
        if (!ui->txtAttenuationQuadratic->hasFocus())
        {
            ui->txtAttenuationQuadratic->setValue(light->getAttenuationQuadratic());
        }

        if (!ui->txtRadius->hasFocus())
        {
            ui->txtRadius->setValue(light->getFar());
        }

        ui->chkLens->setChecked(light->isShowingLens());
        ui->chkFlare->setChecked(light->isShowingFlare());
    }

    updating = false;
}

void LightInfo::updateObject()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_LIGHT)
    {
        Magic3D::Light* light = (Magic3D::Light*)object;

        if (ui->rbDirectional->isChecked())
        {
            light->setLightType(Magic3D::eLIGHT_TYPE_DIRECTIONAL);
        }
        else if (ui->rbPoint->isChecked())
        {
            light->setLightType(Magic3D::eLIGHT_TYPE_POINT);
        }
        else if (ui->rbSpot->isChecked())
        {
            light->setLightType(Magic3D::eLIGHT_TYPE_SPOT);
        }

        light->setShadows(ui->chkShadows->isChecked());

        light->setCutOff(ui->txtSpotCutoff->value());
        light->setExponent(ui->sldSpotExponent->value());

        light->setAttenuationConstant(ui->txtAttenuationConstant->value());
        light->setAttenuationLinear(ui->txtAttenuationLinear->value());
        light->setAttenuationQuadratic(ui->txtAttenuationQuadratic->value());

        light->setFar(ui->txtRadius->value());

        light->setShowLens(ui->chkLens->isChecked());
        light->setShowFlare(ui->chkFlare->isChecked());

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void LightInfo::on_btnColorAmbient_clicked()
{
    Magic3D::Object* object = getObject();
    if (object)
    {
        Magic3D::Light* light = (Magic3D::Light*)object;
        light->setColorAmbient(chooseColor(light->getColorAmbient()));

        updateObject();
    }
}

void LightInfo::on_btnColorDiffuse_clicked()
{
    Magic3D::Object* object = getObject();
    if (object)
    {
        Magic3D::Light* light = (Magic3D::Light*)object;
        light->setColorDiffuse(chooseColor(light->getColorDiffuse()));

        updateObject();
    }
}

void LightInfo::on_btnColorSpecular_clicked()
{
    Magic3D::Object* object = getObject();
    if (object)
    {
        Magic3D::Light* light = (Magic3D::Light*)object;
        light->setColorSpecular(chooseColor(light->getColorSpecular()));

        updateObject();
    }
}

void LightInfo::on_grpSpot_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void LightInfo::on_sldSpotExponent_valueChanged(int value)
{
    if (value)
    {

    }
    updateObject();
}

void LightInfo::on_chkLens_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void LightInfo::on_chkFlare_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void LightInfo::on_rbDirectional_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void LightInfo::on_rbPoint_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void LightInfo::on_rbSpot_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void LightInfo::on_txtAttenuationConstant_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void LightInfo::on_txtAttenuationLinear_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void LightInfo::on_txtAttenuationQuadratic_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void LightInfo::on_txtSpotCutoff_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void LightInfo::on_txtRadius_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void LightInfo::on_chkShadows_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
