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

#include "terraininfo.h"
#include "ui_terraininfo.h"

TerrainInfo::TerrainInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::TerrainInfo)
{
    ui->setupUi(this);

    needGenerateTerrain = false;
}

TerrainInfo::~TerrainInfo()
{
    delete ui;
}

bool TerrainInfo::isEditing()
{
    return ui->btnEdit->isChecked();
}

void TerrainInfo::setRadius(float radius)
{
    ui->txtRadius->setValue(radius);
}

float TerrainInfo::getRadius()
{
    return ui->txtRadius->value();
}

void TerrainInfo::reset(bool emptyHeightMap)
{
    if (!updating)
    {
        Magic3D::Object* object = getObject();

        if (object && object->getType() == Magic3D::eOBJECT_TERRAIN)
        {
            Magic3D::Terrain* terrain = (Magic3D::Terrain*)object;

            if (emptyHeightMap)
            {
                terrain->setHeightMap("");
                ui->txtLoad->clear();
            }
            needGenerateTerrain = true;
        }
    }
}

void TerrainInfo::update()
{
    updating = true;

    Magic3D::Object* object = getObject();

    if (object && object->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = (Magic3D::Terrain*)object;

        ui->txtLength->setValue(terrain->getLength());
        ui->txtWidth->setValue(terrain->getWidth());
        ui->txtTileLength->setValue(terrain->getTileLength());
        ui->txtTileWidth->setValue(terrain->getTileWidth());

        ui->txtLoad->setText(QString::fromStdString(terrain->getHeightMap()));
    }

    updating = false;
}

void TerrainInfo::updateObject()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = (Magic3D::Terrain*)object;

        terrain->setSize(ui->txtLength->value(), ui->txtWidth->value(), ui->txtTileLength->value(), ui->txtTileWidth->value());
        terrain->setHeightMap(ui->txtLoad->text().toStdString());

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void TerrainInfo::editTerrain(Magic3D::Camera* camera, bool mouseDown, bool ctrl, QMouseEvent *event)
{
    if (isEditing() && camera && event && getObject() && getObject()->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        int x = event->pos().x();
        int y = event->pos().y();

        Magic3D::Terrain* terrain = static_cast<Magic3D::Terrain*>(getObject());
        Magic3D::Matrix4 inv = inverse(terrain->getMatrixFromParent());


        Magic3D::Vector3 pos = camera->getPosition3D((float)x, (float)y, 1.0f, Magic3D::Renderer::getInstance()->getCurrentViewPort());
        Magic3D::Vector3 npos = camera->getPosition3D((float)x, (float)y, 2.0f, Magic3D::Renderer::getInstance()->getCurrentViewPort());
        Magic3D::Vector3 dir = normalize(npos - pos);

        bool found = false;
        if (terrain->getRigidBody())
        {
            Magic3D::RayCastReturn result = Magic3D::Physics::getInstance()->rayCast(pos, pos + dir * camera->getFar(), false);
            pos = ((inv) * Magic3D::Vector4(result.point, 1.0f)).getXYZ();
            found = result.physicsObject;
        }

        if (!found)
        {
            pos = camera->getPosition3DOnPlane((float)x, (float)y, terrain->getDirectionUp(), terrain->getPositionFromParent(), Magic3D::Renderer::getInstance()->getCurrentViewPort());
            pos = ((inv) * Magic3D::Vector4(pos, 1.0f)).getXYZ();
        }

        Magic3D::Material* material = terrain->getMeshes()->at(0)->getMaterials()->at(0);

        if (material)
        {
            Magic3D::MaterialVar_Float* var = static_cast<Magic3D::MaterialVar_Float*>(material->getVar("mouse"));

            if (var)
            {
                var->setValue(0, pos.getX());
                var->setValue(1, pos.getZ());
            }

            var = static_cast<Magic3D::MaterialVar_Float*>(material->getVar("radius"));

            if (var)
            {
                var->setValue(0, ui->txtRadius->value());
            }

            Magic3D::MaterialVar_Boolean* varb = static_cast<Magic3D::MaterialVar_Boolean*>(material->getVar("square"));

            if (varb)
            {
                varb->setValue(0, ui->btnSquare->isChecked());
            }
        }

        float factor = 1.0f;

        if ((ui->btnLower->isChecked() && !ctrl) || ((ui->btnRaise->isChecked() || ui->btnFlatten->isChecked()) && ctrl))
        {
                factor = -1.0f;
        }

        if (mouseDown && event->buttons() & Qt::LeftButton)
        {
            //terrain->setHeight(pos.getX(), pos.getZ(), 100.0);
            terrain->modify(pos.getX(), pos.getZ(), ui->txtRadius->value(), ui->txtHeight->value() * factor, ui->btnFlatten->isChecked(), ui->btnSquare->isChecked());
            //Magic3D::Log::logFormat(Magic3D::eLOG_RENDERER, "position: %8.3f, %8.3f, %8.3f", pos.getX(), pos.getY(), pos.getZ());            
        }
        //finalPos = (inv * Magic3D::Vector4(finalPos, 1.0f)).getXYZ();
    }
}

void TerrainInfo::finishEdit()
{
    if (getObject() && getObject()->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = static_cast<Magic3D::Terrain*>(getObject());

        if (needGenerateTerrain)
        {
            needGenerateTerrain = false;
            terrain->loadFromHeightMap();
        }
        else
        {
            terrain->finishModify();
        }
    }
}

void TerrainInfo::on_btnEdit_toggled(bool checked)
{
    if (getObject() && getObject()->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = static_cast<Magic3D::Terrain*>(getObject());
        Magic3D::Material* material = terrain->getMeshes()->at(0)->getMaterials()->at(0);

        if (material)
        {
            Magic3D::MaterialVar_Float*var = static_cast<Magic3D::MaterialVar_Float*>(material->getVar("radius"));

            if (var)
            {
                if (checked)
                {
                    var->setValue(0, ui->txtRadius->value());
                }
                else
                {
                    var->setValue(0, 0.0f);
                }
            }
        }
    }
}

void TerrainInfo::on_btnRaise_toggled(bool checked)
{
    if (checked)
    {
        ui->btnLower->setChecked(false);
        ui->btnFlatten->setChecked(false);
    }
}

void TerrainInfo::on_btnLower_toggled(bool checked)
{
    if (checked)
    {
        ui->btnRaise->setChecked(false);
        ui->btnFlatten->setChecked(false);
    }
}

void TerrainInfo::on_btnFlatten_toggled(bool checked)
{
    if (checked)
    {
        ui->btnRaise->setChecked(false);
        ui->btnLower->setChecked(false);
    }
}

void TerrainInfo::on_txtRadius_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    if (isEditing() && getObject() && getObject()->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = static_cast<Magic3D::Terrain*>(getObject());
        Magic3D::Material* material = terrain->getMeshes()->at(0)->getMaterials()->at(0);

        if (material)
        {
            Magic3D::MaterialVar_Float*var = static_cast<Magic3D::MaterialVar_Float*>(material->getVar("radius"));

            if (var)
            {
                var->setValue(0, ui->txtRadius->value());
            }
        }
    }
}

void TerrainInfo::on_btnUpdate_clicked()
{
    finishEdit();
}

void TerrainInfo::on_btnSave_clicked()
{
    if (getObject() && getObject()->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = static_cast<Magic3D::Terrain*>(getObject());
        terrain->saveToHeightMap(ui->txtSave->text().toStdString());
    }
}

void TerrainInfo::on_btnLoad_clicked()
{
    if (getObject() && getObject()->getType() == Magic3D::eOBJECT_TERRAIN)
    {
        Magic3D::Terrain* terrain = static_cast<Magic3D::Terrain*>(getObject());
        terrain->setHeightMap(ui->txtLoad->text().toStdString());
        terrain->loadFromHeightMap();
    }
}

void TerrainInfo::on_txtLength_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    reset(true);
    updateObject();
}

void TerrainInfo::on_txtWidth_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    reset(true);
    updateObject();
}

void TerrainInfo::on_txtTileLength_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    reset(false);
    updateObject();
}

void TerrainInfo::on_txtTileWidth_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    reset(false);
    updateObject();
}
