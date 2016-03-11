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

#include "vegetationinfo.h"
#include "ui_vegetationinfo.h"
#include "utils.h"

VegetationInfo::VegetationInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::VegetationInfo)
{
    ui->setupUi(this);
    updateList = true;
}

VegetationInfo::~VegetationInfo()
{
    delete ui;
}

void VegetationInfo::updateTerrainCombo()
{
    updating = true;
    QStringList items;
    items.append(UTILS_NONE);

    ui->cmbTerrain->clear();

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

            if (obj->getType() == Magic3D::eOBJECT_TERRAIN)
            {
                items.append(QString::fromStdString(obj->getName()));
            }
        }
    }

    qSort(items.begin(), items.end(), lessNone);
    ui->cmbTerrain->addItems(items);
    updating = false;
}

void VegetationInfo::update()
{
    updating = true;

    Magic3D::Object* object = getObject();

    if (object && object->getType() == Magic3D::eOBJECT_VEGETATION)
    {
        Magic3D::Vegetation* vegetation = (Magic3D::Vegetation*)object;

        Magic3D::TreeProperties& properties = vegetation->getTreeProperties();

        if (!vegetation->getTerrain().empty())
        {
            ui->cmbTerrain->setCurrentIndex(ui->cmbTerrain->findText(QString::fromStdString(vegetation->getTerrain())));
        }
        else
        {
            ui->cmbTerrain->setCurrentIndex(0);
        }        

        ui->txtDensity->setValue(vegetation->getDensity());
        ui->sldDensity->setValue(vegetation->getDensity());

        if (!ui->txtMinHeight->hasFocus())
        {
            ui->txtMinHeight->setValue(vegetation->getMinHeight());
        }
        if (!ui->txtMaxHeight->hasFocus())
        {
            ui->txtMaxHeight->setValue(vegetation->getMaxHeight());
        }

        ui->txtTrunkTexture->setValue(properties.trunkIndex);
        ui->txtTwigTexture->setValue(properties.twigIndex);
        ui->txtSeed->setValue(properties.seed);
        ui->txtBranchSegments->setValue(properties.segments);
        ui->txtBranchLevels->setValue(properties.levels);
        ui->txtTrunkForks->setValue(properties.treeSteps);
        ui->txtVMultiplier->setValue(properties.vMultiplier);
        ui->txtTwigScale->setValue(properties.twigScale);

        ui->txtInitialLength->setValue(properties.initalBranchLength);
        ui->txtFalloffRate->setValue(properties.lengthFalloffFactor);
        ui->txtFalloffPower->setValue(properties.lengthFalloffPower);
        ui->txtMaxClumping->setValue(properties.clumpMax);
        ui->txtMinClumping->setValue(properties.clumpMin);
        ui->txtSymmetry->setValue(properties.branchFactor);
        ui->txtDroop->setValue(properties.dropAmount);
        ui->txtGrowth->setValue(properties.growAmount);
        ui->txtSweep->setValue(properties.sweepAmount);

        ui->txtTrunkRadius->setValue(properties.maxRadius);
        ui->txtRadiusFalloff->setValue(properties.radiusFalloffRate);
        ui->txtClimbRate->setValue(properties.climbRate);
        ui->txtKink->setValue(properties.trunkKink);
        ui->txtTaperRate->setValue(properties.taperRate);
        ui->txtTwists->setValue(properties.twistRate);
        ui->txtTrunkLength->setValue(properties.trunkLength);

        ui->sldBranchSegments->setValue(ui->txtBranchSegments->value());
        ui->sldBranchLevels->setValue(ui->txtBranchLevels->value());
        ui->sldTrunkForks->setValue(ui->txtTrunkForks->value());
        ui->sldVMultiplier->setValue(ui->txtVMultiplier->value() * 1000.0f);
        ui->sldTwigScale->setValue(ui->txtTwigScale->value() * 1000.0f);

        ui->sldInitialLength->setValue(ui->txtInitialLength->value() * 1000.0f);
        ui->sldFalloffRate->setValue(ui->txtFalloffRate->value() * 1000.0f);
        ui->sldFalloffPower->setValue(ui->txtFalloffPower->value() * 1000.0f);
        ui->sldMaxClumping->setValue(ui->txtMaxClumping->value() * 1000.0f);
        ui->sldMinClumping->setValue(ui->txtMinClumping->value() * 1000.0f);
        ui->sldSymmetry->setValue(ui->txtSymmetry->value() * 1000.0f);
        ui->sldDroop->setValue(ui->txtDroop->value() * 1000.0f);
        ui->sldGrowth->setValue(ui->txtGrowth->value() * 1000.0f);
        ui->sldSweep->setValue(ui->txtSweep->value() * 1000.0f);

        ui->sldTrunkRadius->setValue(ui->txtTrunkRadius->value() * 1000.0f);
        ui->sldRadiusFalloff->setValue(ui->txtRadiusFalloff->value() * 1000.0f);
        ui->sldClimbRate->setValue(ui->txtClimbRate->value() * 1000.0f);
        ui->sldKink->setValue(ui->txtKink->value() * 1000.0f);
        ui->sldTaperRate->setValue(ui->txtTaperRate->value() * 1000.0f);
        ui->sldTwists->setValue(ui->txtTwists->value() * 1000.0f);
        ui->sldTrunkLength->setValue(ui->txtTrunkLength->value() * 1000.0f);

        if (updateList)
        {
            int index = ui->lstTrees->currentRow();
            ui->lstTrees->clear();
            for (int i = 0; i < (int)vegetation->getTreesList()->size(); i++)
            {
                ui->lstTrees->addItem(tr("%1").arg(i));
            }
            if (index >= 0 && index < (int)ui->lstTrees->count())
            {
                ui->lstTrees->setCurrentRow(index);
            }
        }
    }

    updating = false;
}

void VegetationInfo::updateValues()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_VEGETATION)
    {
        Magic3D::Vegetation* vegetation = (Magic3D::Vegetation*)object;

        Magic3D::TreeProperties& properties = vegetation->getTreeProperties();

        vegetation->setDensity(ui->sldDensity->value());

        properties.segments            = ui->sldBranchSegments->value();
        if (properties.segments % 2 != 0)
        {
            properties.segments -= properties.segments % 2;
        }

        properties.levels              = ui->sldBranchLevels->value();
        properties.treeSteps           = ui->sldTrunkForks->value();
        properties.vMultiplier         = (float)ui->sldVMultiplier->value() / 1000.0f;
        properties.twigScale           = (float)ui->sldTwigScale->value() / 1000.0f;

        properties.initalBranchLength  = (float)ui->sldInitialLength->value() / 1000.0f;
        properties.lengthFalloffFactor = (float)ui->sldFalloffRate->value() / 1000.0f;
        properties.lengthFalloffPower  = (float)ui->sldFalloffPower->value() / 1000.0f;
        properties.clumpMax            = (float)ui->sldMaxClumping->value() / 1000.0f;
        properties.clumpMin            = (float)ui->sldMinClumping->value() / 1000.0f;
        properties.branchFactor        = (float)ui->sldSymmetry->value() / 1000.0f;
        properties.dropAmount          = (float)ui->sldDroop->value() / 1000.0f;
        properties.growAmount          = (float)ui->sldGrowth->value() / 1000.0f;
        properties.sweepAmount         = (float)ui->sldSweep->value() / 1000.0f;

        properties.maxRadius           = (float)ui->sldTrunkRadius->value() / 1000.0f;
        properties.radiusFalloffRate   = (float)ui->sldRadiusFalloff->value() / 1000.0f;
        properties.climbRate           = (float)ui->sldClimbRate->value() / 1000.0f;
        properties.trunkKink           = (float)ui->sldKink->value() / 1000.0f;
        properties.taperRate           = (float)ui->sldTaperRate->value() / 1000.0f;
        properties.twistRate           = (float)ui->sldTwists->value() / 1000.0f;
        properties.trunkLength         = (float)ui->sldTrunkLength->value() / 1000.0f;

        vegetation->generateVegetation();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void VegetationInfo::updateObject()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_VEGETATION)
    {
        Magic3D::Vegetation* vegetation = (Magic3D::Vegetation*)object;
        Magic3D::TreeProperties& properties = vegetation->getTreeProperties();

        if (ui->cmbTerrain->currentIndex() > 0)
        {
            vegetation->setTerrain(ui->cmbTerrain->currentText().toStdString());
        }
        else
        {
            vegetation->setTerrain("");
        }

        vegetation->setDensity(ui->txtDensity->value());
        vegetation->setMinHeight(ui->txtMinHeight->value());
        vegetation->setMaxHeight(ui->txtMaxHeight->value());

        properties.trunkIndex = ui->txtTrunkTexture->value();
        properties.twigIndex = ui->txtTwigTexture->value();
        properties.seed = ui->txtSeed->value();
        properties.segments = ui->txtBranchSegments->value();
        if (properties.segments % 2 != 0)
        {
            properties.segments -= properties.segments % 2;
        }

        properties.levels = ui->txtBranchLevels->value();
        properties.treeSteps = ui->txtTrunkForks->value();
        properties.vMultiplier = ui->txtVMultiplier->value();
        properties.twigScale = ui->txtTwigScale->value();

        properties.initalBranchLength = ui->txtInitialLength->value();
        properties.lengthFalloffFactor = ui->txtFalloffRate->value();
        properties.lengthFalloffPower = ui->txtFalloffPower->value();
        properties.clumpMax = ui->txtMaxClumping->value();
        properties.clumpMin = ui->txtMinClumping->value();
        properties.branchFactor = ui->txtSymmetry->value();
        properties.dropAmount = ui->txtDroop->value();
        properties.growAmount = ui->txtGrowth->value();
        properties.sweepAmount = ui->txtSweep->value();

        properties.maxRadius = ui->txtTrunkRadius->value();
        properties.radiusFalloffRate = ui->txtRadiusFalloff->value();
        properties.climbRate = ui->txtClimbRate->value();
        properties.trunkKink = ui->txtKink->value();
        properties.taperRate = ui->txtTaperRate->value();
        properties.twistRate = ui->txtTwists->value();
        properties.trunkLength = ui->txtTrunkLength->value();

        vegetation->generateVegetation();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void VegetationInfo::on_txtSeed_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtBranchSegments_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtBranchLevels_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTrunkForks_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtVMultiplier_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTwigScale_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtInitialLength_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtFalloffRate_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtFalloffPower_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtMaxClumping_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtMinClumping_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtSymmetry_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtDroop_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtGrowth_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtSweep_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTrunkRadius_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtRadiusFalloff_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtClimbRate_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtKink_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTaperRate_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTwists_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTrunkLength_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_sldBranchSegments_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldBranchLevels_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldTrunkForks_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldVMultiplier_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldTwigScale_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldInitialLength_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldFalloffRate_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldFalloffPower_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldMaxClumping_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldMinClumping_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldSymmetry_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldDroop_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldGrowth_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldSweep_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldTrunkRadius_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldRadiusFalloff_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldClimbRate_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldKink_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldTaperRate_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldTwists_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_sldTrunkLength_valueChanged(int value)
{
    if (value)
    {

    }
    updateValues();
}

void VegetationInfo::on_cmbTerrain_currentIndexChanged(int index)
{
    if (index > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_sldDensity_valueChanged(int value)
{
    if (value > 0)
    {

    }
    updateValues();
}

void VegetationInfo::on_txtDensity_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtMinHeight_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtMaxHeight_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTrunkTexture_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_txtTwigTexture_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void VegetationInfo::on_lstTrees_currentRowChanged(int currentRow)
{
    if (currentRow > 0)
    {

    }
    if (!updating)
    {
        Magic3D::Object* object = getObject();

        if (!updating && object && object->getType() == Magic3D::eOBJECT_VEGETATION)
        {
            Magic3D::Vegetation* vegetation = (Magic3D::Vegetation*)object;
            if (currentRow >=0 && currentRow < (int)vegetation->getTreesList()->size())
            {
                vegetation->setTreeProperties(vegetation->getTreesList()->at(currentRow));
                updateList = false;
                update();
                updateList = true;
                vegetation->generateVegetation();
            }
        }
    }
}

void VegetationInfo::on_btnAdd_clicked()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_VEGETATION)
    {
        Magic3D::Vegetation* vegetation = (Magic3D::Vegetation*)object;

        vegetation->getTreesList()->push_back(vegetation->getTreeProperties());
        update();
    }
}

void VegetationInfo::on_btnRemove_clicked()
{
    Magic3D::Object* object = getObject();

    if (!updating && object && object->getType() == Magic3D::eOBJECT_VEGETATION)
    {
        Magic3D::Vegetation* vegetation = (Magic3D::Vegetation*)object;

        int currentRow = ui->lstTrees->currentRow();
        if (currentRow >=0 && currentRow < (int)vegetation->getTreesList()->size())
        {
            vegetation->getTreesList()->erase(vegetation->getTreesList()->begin() + currentRow);
            if (currentRow >= (int)vegetation->getTreesList()->size())
            {
                currentRow = vegetation->getTreesList()->size() - 1;
            }
            if (currentRow < 0)
            {
                currentRow = 0;
            }
            if (currentRow >= 0 && currentRow < (int)vegetation->getTreesList()->size())
            {
                ui->lstTrees->setCurrentRow(currentRow);
            }
            update();
        }
    }
}
