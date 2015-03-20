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

#include "sceneinfo.h"
#include "ui_sceneinfo.h"
#include "utils.h"

#include "magic3d/magic3d.h"

SceneInfo::SceneInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::SceneInfo)
{
    ui->setupUi(this);
}

SceneInfo::~SceneInfo()
{
    delete ui;
}

void SceneInfo::update()
{
    updating = true;

    ui->chkScreenEffects->setChecked(Magic3D::Renderer::getInstance()->isUsingScreenEffects());

    if (!ui->txtTimeScale->hasFocus())
    {
        ui->txtTimeScale->setValue(Magic3D::Magic3D::getInstance()->getTimeScale());
    }

    setButtonColor(ui->btnColorClear,   Magic3D::Scene::getInstance()->getColorClear());
    setButtonColor(ui->btnColorAmbient, Magic3D::Scene::getInstance()->getColorAmbient());
    setButtonColor(ui->btnColorFog,     Magic3D::Scene::getInstance()->getFogColor());

    if (!ui->txtFogStart->hasFocus())
    {
        ui->txtFogStart->setValue(Magic3D::Scene::getInstance()->getFogStart());
    }
    if (!ui->txtFogFinish->hasFocus())
    {
        ui->txtFogFinish->setValue(Magic3D::Scene::getInstance()->getFogFinish());
    }

    if (!ui->txtGravityX->hasFocus())
    {
        ui->txtGravityX->setValue(Magic3D::Scene::getInstance()->getGravity().getX());
    }
    if (!ui->txtGravityY->hasFocus())
    {
        ui->txtGravityY->setValue(Magic3D::Scene::getInstance()->getGravity().getY());
    }
    if (!ui->txtGravityZ->hasFocus())
    {
        ui->txtGravityZ->setValue(Magic3D::Scene::getInstance()->getGravity().getZ());
    }

    if (!ui->txtWindX->hasFocus())
    {
        ui->txtWindX->setValue(Magic3D::Scene::getInstance()->getWind().getX());
    }
    if (!ui->txtWindY->hasFocus())
    {
        ui->txtWindY->setValue(Magic3D::Scene::getInstance()->getWind().getY());
    }
    if (!ui->txtWindZ->hasFocus())
    {
        ui->txtWindZ->setValue(Magic3D::Scene::getInstance()->getWind().getZ());
    }

    ui->sldVolumeMaster->setValue(Magic3D::Scene::getInstance()->getVolumeMaster() * 1000.0f);

    if (mainWindow && mainWindow->getProject())
    {
        if (!ui->txtGrid2DX->hasFocus())
        {
            ui->txtGrid2DX->setValue(mainWindow->getProject()->getGrid2DX());
        }
        if (!ui->txtGrid2DY->hasFocus())
        {
            ui->txtGrid2DY->setValue(mainWindow->getProject()->getGrid2DY());
        }

        if (!ui->txtGrid3DX->hasFocus())
        {
            ui->txtGrid3DX->setValue(mainWindow->getProject()->getGrid3DX());
        }
        if (!ui->txtGrid3DY->hasFocus())
        {
            ui->txtGrid3DY->setValue(mainWindow->getProject()->getGrid3DY());
        }
        if (!ui->txtGrid3DZ->hasFocus())
        {
            ui->txtGrid3DZ->setValue(mainWindow->getProject()->getGrid3DZ());
        }
    }

    if (Magic3D::Scene::getInstance()->getFBOCamera())
    {
        ui->cmbFBOCamera->setCurrentIndex(ui->cmbFBOCamera->findText(QString::fromStdString(Magic3D::Scene::getInstance()->getFBOCamera()->getName())));
    }
    else
    {
        ui->cmbFBOCamera->setCurrentIndex(0);
    }

    updating = false;
}

void SceneInfo::updateObject()
{
    if (!updating && mainWindow && mainWindow->getProject())
    {
        Magic3D::Magic3D::getInstance()->setTimeScale(ui->txtTimeScale->value());
        Magic3D::Renderer::getInstance()->setUsingScreenEffects(ui->chkScreenEffects->isChecked());

        Magic3D::Scene::getInstance()->setFogStart(ui->txtFogStart->value());
        Magic3D::Scene::getInstance()->setFogFinish(ui->txtFogFinish->value());

        Magic3D::Scene::getInstance()->setGravity(Magic3D::Vector3(ui->txtGravityX->value(), ui->txtGravityY->value(), ui->txtGravityZ->value()));
        Magic3D::Scene::getInstance()->setWind(Magic3D::Vector3(ui->txtWindX->value(), ui->txtWindY->value(), ui->txtWindZ->value()));

        Magic3D::Scene::getInstance()->setVolumeMaster(ui->sldVolumeMaster->value() * 0.001f);

        mainWindow->getProject()->setGrid2D(ui->txtGrid2DX->value(), ui->txtGrid2DY->value());
        mainWindow->getProject()->setGrid3D(ui->txtGrid3DX->value(), ui->txtGrid3DY->value(), ui->txtGrid3DZ->value());

        if (ui->cmbFBOCamera->currentIndex() > 0)
        {
            Magic3D::Object* obj = Magic3D::ResourceManager::getObjects()->get(ui->cmbFBOCamera->currentText().toStdString());

            if (!Magic3D::Scene::getInstance()->getFBOCamera() || obj->getName().compare(Magic3D::Scene::getInstance()->getFBOCamera()->getName()) != 0)
            {
                Magic3D::Scene::getInstance()->setFBOCamera(static_cast<Magic3D::Camera*>(obj));
            }
        }
        else
        {
            Magic3D::Scene::getInstance()->setFBOCamera(NULL);
        }

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void SceneInfo::updateCamerasCombo()
{
    updating = true;
    QStringList items;
    items.append(UTILS_NONE);

    QString current = ui->cmbFBOCamera->currentText();
    ui->cmbFBOCamera->clear();

    std::vector<Magic3D::Layer*>* layers = Magic3D::Scene::getInstance()->getLayers();
    std::vector<Magic3D::Layer*>::const_iterator it_l = layers->begin();
    while (it_l != layers->end())
    {
        Magic3D::Layer* layer = *it_l++;

        std::vector<Magic3D::Object*>* objects = layer->getObjects3D();
        std::vector<Magic3D::Object*>::const_iterator it_o = objects->begin();
        while (it_o != objects->end())
        {
            Magic3D::Object* obj = *it_o++;

            if (obj->getType() == Magic3D::eOBJECT_CAMERA)
            {
                Magic3D::Camera* camera = static_cast<Magic3D::Camera*>(obj);

                if (camera->getProjectionType() != Magic3D::ePROJECTION_ORTHOGRAPHIC)
                {
                    items.append(QString::fromStdString(obj->getName()));
                }
            }
        }
    }

    qSort(items.begin(), items.end(), lessNone);
    ui->cmbFBOCamera->addItems(items);
    if (current.isEmpty())
    {
        ui->cmbFBOCamera->setCurrentIndex(0);
    }
    else
    {
        ui->cmbFBOCamera->setCurrentIndex(ui->cmbFBOCamera->findText(current));
    }
    updating = false;
}

void SceneInfo::on_btnColorClear_clicked()
{
    Magic3D::Scene::getInstance()->setColorClear(chooseColor(Magic3D::Scene::getInstance()->getColorClear()));
    mainWindow->setProjectChanged(true);
    mainWindow->update();

    update();
}

void SceneInfo::on_btnColorAmbient_clicked()
{
    Magic3D::Scene::getInstance()->setColorAmbient(chooseColor(Magic3D::Scene::getInstance()->getColorAmbient()));
    mainWindow->setProjectChanged(true);
    mainWindow->update();

    update();
}

void SceneInfo::on_btnColorFog_clicked()
{
    Magic3D::Scene::getInstance()->setFogColor(chooseColor(Magic3D::Scene::getInstance()->getFogColor()));
    mainWindow->setProjectChanged(true);
    mainWindow->update();

    update();
}

void SceneInfo::on_sldVolumeMaster_valueChanged(int value)
{
    if (value)
    {

    }
    updateObject();
}

void SceneInfo::on_txtFogStart_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtFogFinish_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGravityX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGravityY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGravityZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGrid3DX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGrid3DY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGrid3DZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGrid2DX_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void SceneInfo::on_txtGrid2DY_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void SceneInfo::on_cmbFBOCamera_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void SceneInfo::on_txtWindX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtWindY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_txtWindZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void SceneInfo::on_chkScreenEffects_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void SceneInfo::on_txtTimeScale_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}
