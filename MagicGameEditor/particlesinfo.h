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

#ifndef PARTICLESINFO_H
#define PARTICLESINFO_H

#include <QWidget>
#include "magic3dobjectpanel.h"

namespace Magic3D
{
    class Particles;
}
namespace Ui {
    class ParticlesInfo;
}

class ParticlesInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit ParticlesInfo(MainWindow* mainWindow);
    ~ParticlesInfo();

    virtual void update();
    virtual void updateObject();

private:
    Ui::ParticlesInfo *ui;

    Magic3D::Particles* getParticles();

    void updateUVS();
    Magic3D::Vector3 getTextureSize();
private slots:


private slots:
    void on_btnPlay_clicked();
    void on_chkLoop_toggled(bool checked);
    void on_chkInvertedDirection_toggled(bool checked);
    void on_chkCreationDirection_toggled(bool checked);
    void on_chkFollowEmitter_toggled(bool checked);
    void on_chkRecreateWhenDie_toggled(bool checked);
    void on_rbBillboardBoth_toggled(bool checked);
    void on_rbBillboardVertical_toggled(bool checked);
    void on_rbBillboardHorizontal_toggled(bool checked);
    void on_rbBillboardNone_toggled(bool checked);
    void on_btnMaxDieColor_clicked();
    void on_btnMinDieColor_clicked();
    void on_btnMaxEmitColor_clicked();
    void on_btnMinEmitColor_clicked();
    void on_btnSave_clicked();
    void on_chkTrail_toggled(bool checked);
    void on_txtMinParticlesCount_valueChanged(int arg1);
    void on_txtMaxParticlesCount_valueChanged(int arg1);
    void on_txtParticlesPerSecond_valueChanged(int arg1);
    void on_txtMinCreationDistanceX_valueChanged(double arg1);
    void on_txtMinCreationDistanceY_valueChanged(double arg1);
    void on_txtMinCreationDistanceZ_valueChanged(double arg1);
    void on_txtMaxCreationDistanceX_valueChanged(double arg1);
    void on_txtMaxCreationDistanceY_valueChanged(double arg1);
    void on_txtMaxCreationDistanceZ_valueChanged(double arg1);
    void on_txtMinRadius_valueChanged(double arg1);
    void on_txtMaxRadius_valueChanged(double arg1);
    void on_txtMinSpeed_valueChanged(double arg1);
    void on_txtMaxSpeed_valueChanged(double arg1);
    void on_txtMinAcceleration_valueChanged(double arg1);
    void on_txtMaxAcceleration_valueChanged(double arg1);
    void on_txtAccelerationX_valueChanged(double arg1);
    void on_txtAccelerationY_valueChanged(double arg1);
    void on_txtAccelerationZ_valueChanged(double arg1);
    void on_txtMinAge_valueChanged(double arg1);
    void on_txtMaxAge_valueChanged(double arg1);
    void on_txtMinEmitSize_valueChanged(double arg1);
    void on_txtMaxEmitSize_valueChanged(double arg1);
    void on_txtMinDieSize_valueChanged(double arg1);
    void on_txtMaxDieSize_valueChanged(double arg1);
    void on_txtMinSpinSpeed_valueChanged(double arg1);
    void on_txtMaxSpinSpeed_valueChanged(double arg1);
    void on_txtMinSpinAcceleration_valueChanged(double arg1);
    void on_txtMaxSpinAcceleration_valueChanged(double arg1);
    void on_txtU1_valueChanged(double arg1);
    void on_txtV1_valueChanged(double arg1);
    void on_txtU2_valueChanged(double arg1);
    void on_txtV2_valueChanged(double arg1);
    void on_txtU3_valueChanged(double arg1);
    void on_txtV3_valueChanged(double arg1);
    void on_txtU4_valueChanged(double arg1);
    void on_txtV4_valueChanged(double arg1);
    void on_txtDistortionFrequency_valueChanged(double arg1);
    void on_txtDistortionFactor_valueChanged(double arg1);
    void on_txtDistortionWave_valueChanged(double arg1);
    void on_rbDistortionBubble_toggled(bool checked);
    void on_rbDistortionWave_toggled(bool checked);
    void on_rbDistortionNoise_toggled(bool checked);
    void on_gbDistortion_toggled(bool arg1);
    void on_txtTile_valueChanged(double arg1);
    void on_txtTileColumn_valueChanged(int arg1);
    void on_txtTileRow_valueChanged(int arg1);
    void on_btnStop_clicked();
};

#endif // PARTICLESINFO_H
