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

#ifndef VEGETATIONINFO_H
#define VEGETATIONINFO_H

#include "magic3dobjectpanel.h"

namespace Ui {
    class VegetationInfo;
}

class VegetationInfo : public Magic3DObjectPanel
{
    Q_OBJECT

private:
    void updateValues();
    bool updateList;
public:
    explicit VegetationInfo(MainWindow* mainWindow);
    ~VegetationInfo();

    virtual void update();
    virtual void updateObject();    

    void updateTerrainCombo();

private slots:
    void on_txtSeed_valueChanged(int arg1);

    void on_txtBranchSegments_valueChanged(int arg1);

    void on_txtBranchLevels_valueChanged(int arg1);

    void on_txtTrunkForks_valueChanged(int arg1);

    void on_txtVMultiplier_valueChanged(double arg1);

    void on_txtTwigScale_valueChanged(double arg1);

    void on_txtInitialLength_valueChanged(double arg1);

    void on_txtFalloffRate_valueChanged(double arg1);

    void on_txtFalloffPower_valueChanged(double arg1);

    void on_txtMaxClumping_valueChanged(double arg1);

    void on_txtMinClumping_valueChanged(double arg1);

    void on_txtSymmetry_valueChanged(double arg1);

    void on_txtDroop_valueChanged(double arg1);

    void on_txtGrowth_valueChanged(double arg1);

    void on_txtSweep_valueChanged(double arg1);

    void on_txtTrunkRadius_valueChanged(double arg1);

    void on_txtRadiusFalloff_valueChanged(double arg1);

    void on_txtClimbRate_valueChanged(double arg1);

    void on_txtKink_valueChanged(double arg1);

    void on_txtTaperRate_valueChanged(double arg1);

    void on_txtTwists_valueChanged(double arg1);

    void on_txtTrunkLength_valueChanged(double arg1);

    void on_sldBranchSegments_valueChanged(int value);

    void on_sldBranchLevels_valueChanged(int value);

    void on_sldTrunkForks_valueChanged(int value);

    void on_sldVMultiplier_valueChanged(int value);

    void on_sldTwigScale_valueChanged(int value);

    void on_sldInitialLength_valueChanged(int value);

    void on_sldFalloffRate_valueChanged(int value);

    void on_sldFalloffPower_valueChanged(int value);

    void on_sldMaxClumping_valueChanged(int value);

    void on_sldMinClumping_valueChanged(int value);

    void on_sldSymmetry_valueChanged(int value);

    void on_sldDroop_valueChanged(int value);

    void on_sldGrowth_valueChanged(int value);

    void on_sldSweep_valueChanged(int value);

    void on_sldTrunkRadius_valueChanged(int value);

    void on_sldRadiusFalloff_valueChanged(int value);

    void on_sldClimbRate_valueChanged(int value);

    void on_sldKink_valueChanged(int value);

    void on_sldTaperRate_valueChanged(int value);

    void on_sldTwists_valueChanged(int value);

    void on_sldTrunkLength_valueChanged(int value);

    void on_cmbTerrain_currentIndexChanged(int index);

    void on_sldDensity_valueChanged(int value);

    void on_txtDensity_valueChanged(int arg1);

    void on_txtMinHeight_valueChanged(double arg1);

    void on_txtMaxHeight_valueChanged(double arg1);

    void on_txtTrunkTexture_valueChanged(int arg1);

    void on_txtTwigTexture_valueChanged(int arg1);

    void on_lstTrees_currentRowChanged(int currentRow);

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

private:
    Ui::VegetationInfo *ui;
};

#endif // VEGETATIONINFO_H
