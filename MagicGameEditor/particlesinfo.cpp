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

#include "particlesinfo.h"
#include "ui_particlesinfo.h"
#include "utils.h"
#include "magic3d/magic3d.h"

ParticlesInfo::ParticlesInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::ParticlesInfo)
{
    ui->setupUi(this);
}

ParticlesInfo::~ParticlesInfo()
{
    delete ui;
}

Magic3D::Particles* ParticlesInfo::getParticles()
{
    Magic3D::Object* object = getObject();
    Magic3D::Particles* particles = NULL;

    if (object && object->getType() == Magic3D::eOBJECT_PARTICLES)
    {
        particles = (Magic3D::Particles*)object;
    }

    return particles;
}

void ParticlesInfo::updateUVS()
{
    updating = true;
    Magic3D::Particles* particles = getParticles();
    if (particles)
    {
        Magic3D::Vector3 tex = getTextureSize();
        float tile = ui->txtTile->value();
        float width = 1.0f / (tex.getX() / tile);
        float height = 1.0f / (tex.getX() / tile);

        float col = (float)ui->txtTileColumn->value();
        float row = (float)ui->txtTileRow->value();

        Magic3D::Texture2D uv1, uv2, uv3, uv4;

        uv1.u = col * width;
        uv1.v = 1.0f - row * height;

        uv2.u = uv1.u;
        uv2.v = uv1.v - height;

        uv3.u = uv1.u + width;
        uv3.v = uv1.v - height;

        uv4.u = uv1.u + width;
        uv4.v = uv1.v;

        particles->setUV1(uv1);
        particles->setUV2(uv2);
        particles->setUV3(uv3);
        particles->setUV4(uv4);

        ui->txtU1->setValue(particles->getUV1().u);
        ui->txtV1->setValue(particles->getUV1().v);
        ui->txtU2->setValue(particles->getUV2().u);
        ui->txtV2->setValue(particles->getUV2().v);
        ui->txtU3->setValue(particles->getUV3().u);
        ui->txtV3->setValue(particles->getUV3().v);
        ui->txtU4->setValue(particles->getUV4().u);
        ui->txtV4->setValue(particles->getUV4().v);

        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
    updating = false;
}

Magic3D::Vector3 ParticlesInfo::getTextureSize()
{
    Magic3D::Vector3 result = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    Magic3D::Particles* particles = getParticles();
    if (particles && particles->getMeshes()->size() > 0 && particles->getMeshes()->at(0) &&
        particles->getMeshes()->at(0)->getMaterials()->size() > 0 && particles->getMeshes()->at(0)->getMaterials()->at(0))
    {
        Magic3D::Texture* tex = particles->getMeshes()->at(0)->getMaterials()->at(0)->getTexture(0);
        if (tex)
        {
            result.setX((float)tex->width);
            result.setY((float)tex->height);
        }
    }

    return result;
}

void ParticlesInfo::update()
{
    updating = true;
    Magic3D::Particles* particles = getParticles();

    if (particles)
    {
        ui->chkLoop->setChecked(particles->isLooping());

        if (!ui->txtMinParticlesCount->hasFocus())
        {
            ui->txtMinParticlesCount->setValue(particles->getMinParticlesCount());
        }
        if (!ui->txtMaxParticlesCount->hasFocus())
        {
            ui->txtMaxParticlesCount->setValue(particles->getMaxParticlesCount());
        }

        if (!ui->txtParticlesPerSecond->hasFocus())
        {
            ui->txtParticlesPerSecond->setValue(particles->getParticlesSecond());
        }

        if (!ui->txtMinCreationDistanceX->hasFocus())
        {
            ui->txtMinCreationDistanceX->setValue(particles->getMinCreationDistance().getX());
        }
        if (!ui->txtMinCreationDistanceY->hasFocus())
        {
            ui->txtMinCreationDistanceY->setValue(particles->getMinCreationDistance().getY());
        }
        if (!ui->txtMinCreationDistanceZ->hasFocus())
        {
            ui->txtMinCreationDistanceZ->setValue(particles->getMinCreationDistance().getZ());
        }

        if (!ui->txtMaxCreationDistanceX->hasFocus())
        {
            ui->txtMaxCreationDistanceX->setValue(particles->getMaxCreationDistance().getX());
        }
        if (!ui->txtMaxCreationDistanceY->hasFocus())
        {
            ui->txtMaxCreationDistanceY->setValue(particles->getMaxCreationDistance().getY());
        }
        if (!ui->txtMaxCreationDistanceZ->hasFocus())
        {
            ui->txtMaxCreationDistanceZ->setValue(particles->getMaxCreationDistance().getZ());
        }

        if (!ui->txtMinSpeed->hasFocus())
        {
            ui->txtMinSpeed->setValue(particles->getMinSpeed());
        }
        if (!ui->txtMaxSpeed->hasFocus())
        {
            ui->txtMaxSpeed->setValue(particles->getMaxSpeed());
        }

        if (!ui->txtMinAcceleration->hasFocus())
        {
            ui->txtMinAcceleration->setValue(particles->getMinAcceleration());
        }
        if (!ui->txtMaxAcceleration->hasFocus())
        {
            ui->txtMaxAcceleration->setValue(particles->getMaxAcceleration());
        }

        if (!ui->txtAccelerationX->hasFocus())
        {
            ui->txtAccelerationX->setValue(particles->getAcceleration().getX());
        }
        if (!ui->txtAccelerationY->hasFocus())
        {
            ui->txtAccelerationY->setValue(particles->getAcceleration().getY());
        }
        if (!ui->txtAccelerationZ->hasFocus())
        {
            ui->txtAccelerationZ->setValue(particles->getAcceleration().getZ());
        }

        if (!ui->txtMinAge->hasFocus())
        {
            ui->txtMinAge->setValue(particles->getMinAge());
        }
        if (!ui->txtMaxAge->hasFocus())
        {
            ui->txtMaxAge->setValue(particles->getMaxAge());
        }

        if (!ui->txtMinEmitSize->hasFocus())
        {
            ui->txtMinEmitSize->setValue(particles->getMinEmitSize());
        }
        if (!ui->txtMaxEmitSize->hasFocus())
        {
            ui->txtMaxEmitSize->setValue(particles->getMaxEmitSize());
        }

        if (!ui->txtMinDieSize->hasFocus())
        {
            ui->txtMinDieSize->setValue(particles->getMinDieSize());
        }
        if (!ui->txtMaxDieSize->hasFocus())
        {
            ui->txtMaxDieSize->setValue(particles->getMaxDieSize());
        }

        if (!ui->txtMinSpinSpeed->hasFocus())
        {
            ui->txtMinSpinSpeed->setValue(particles->getMinSpinSpeed());
        }
        if (!ui->txtMaxSpinSpeed->hasFocus())
        {
            ui->txtMaxSpinSpeed->setValue(particles->getMaxSpinSpeed());
        }

        if (!ui->txtMinSpinAcceleration->hasFocus())
        {
            ui->txtMinSpinAcceleration->setValue(particles->getMinSpinAcceleration());
        }
        if (!ui->txtMaxSpinAcceleration->hasFocus())
        {
            ui->txtMaxSpinAcceleration->setValue(particles->getMaxSpinAcceleration());
        }

        if (!ui->txtMinRadius->hasFocus())
        {
            ui->txtMinRadius->setValue(particles->getMinRadius());
        }
        if (!ui->txtMaxRadius->hasFocus())
        {
            ui->txtMaxRadius->setValue(particles->getMaxRadius());
        }

        switch (particles->getBillboardParticle())
        {
            case Magic3D::eBILLBOARD_HORIZONTAL: ui->rbBillboardHorizontal->setChecked(true); break;
            case Magic3D::eBILLBOARD_VERTICAL:   ui->rbBillboardVertical->setChecked(true);   break;
            case Magic3D::eBILLBOARD_BOTH:       ui->rbBillboardBoth->setChecked(true);       break;
            default:                             ui->rbBillboardNone->setChecked(true);       break;
        }

        ui->chkRecreateWhenDie->setChecked(particles->isRecreatedWhenDie());
        ui->chkFollowEmitter->setChecked(particles->isFollowingEmitter());
        ui->chkCreationDirection->setChecked(particles->isUsingCreationDirection());
        ui->chkInvertedDirection->setChecked(particles->isInvertedDirection());

        ui->chkTrail->setChecked(particles->isTrail());

        if (!ui->txtU1->hasFocus())
        {
            ui->txtU1->setValue(particles->getUV1().u);
        }
        if (!ui->txtV1->hasFocus())
        {
            ui->txtV1->setValue(particles->getUV1().v);
        }
        if (!ui->txtU2->hasFocus())
        {
            ui->txtU2->setValue(particles->getUV2().u);
        }
        if (!ui->txtV2->hasFocus())
        {
            ui->txtV2->setValue(particles->getUV2().v);
        }
        if (!ui->txtU3->hasFocus())
        {
            ui->txtU3->setValue(particles->getUV3().u);
        }
        if (!ui->txtV3->hasFocus())
        {
            ui->txtV3->setValue(particles->getUV3().v);
        }
        if (!ui->txtU4->hasFocus())
        {
            ui->txtU4->setValue(particles->getUV4().u);
        }
        if (!ui->txtV4->hasFocus())
        {
            ui->txtV4->setValue(particles->getUV4().v);
        }

        ui->gbDistortion->setChecked(particles->isDistorting());
        if (!ui->txtDistortionFrequency->hasFocus())
        {
            ui->txtDistortionFrequency->setValue(particles->getDistortionFrequency());
        }
        if (!ui->txtDistortionFactor->hasFocus())
        {
            ui->txtDistortionFactor->setValue(particles->getDistortionFactor());
        }
        if (!ui->txtDistortionWave->hasFocus())
        {
            ui->txtDistortionWave->setValue(particles->getDistortionWave());
        }

        switch (particles->getDistortiionType())
        {
            case Magic3D::eDISTORTION_BUBBLE: ui->rbDistortionBubble->setChecked(true); break;
            case Magic3D::eDISTORTION_WAVE:   ui->rbDistortionWave->setChecked(true);   break;
            case Magic3D::eDISTORTION_NOISE:  ui->rbDistortionNoise->setChecked(true);  break;
            default:                          ui->rbDistortionBubble->setChecked(true); break;
        }

        setButtonColor(ui->btnMinEmitColor, particles->getMinEmitColor());
        setButtonColor(ui->btnMaxEmitColor, particles->getMaxEmitColor());

        setButtonColor(ui->btnMinDieColor, particles->getMinDieColor());
        setButtonColor(ui->btnMaxDieColor, particles->getMaxDieColor());

        Magic3D::Vector3 tex = getTextureSize();
        ui->lblTextureSize->setText(tr("%1x%2").arg((int)tex.getX()).arg((int)tex.getY()));
    }

    updating = false;
}

void ParticlesInfo::updateObject()
{
    Magic3D::Particles* particles = getParticles();

    if (!updating && particles)
    {

        particles->stop();

        particles->setLooping(ui->chkLoop->isChecked());
        particles->setMinParticlesCount(ui->txtMinParticlesCount->value());
        particles->setMaxParticlesCount(ui->txtMaxParticlesCount->value());

        particles->setParticlesSecond(ui->txtParticlesPerSecond->value());

        particles->setMinCreationDistance(Magic3D::Vector3(ui->txtMinCreationDistanceX->value(),
                                                           ui->txtMinCreationDistanceY->value(),
                                                           ui->txtMinCreationDistanceZ->value()));

        particles->setMaxCreationDistance(Magic3D::Vector3(ui->txtMaxCreationDistanceX->value(),
                                                           ui->txtMaxCreationDistanceY->value(),
                                                           ui->txtMaxCreationDistanceZ->value()));

        particles->setMinSpeed(ui->txtMinSpeed->value());
        particles->setMaxSpeed(ui->txtMaxSpeed->value());

        particles->setMinAcceleration(ui->txtMinAcceleration->value());
        particles->setMaxAcceleration(ui->txtMaxAcceleration->value());

        particles->setAcceleration(Magic3D::Vector3(ui->txtAccelerationX->value(),
                                                    ui->txtAccelerationY->value(),
                                                    ui->txtAccelerationZ->value()));

        particles->setMinAge(ui->txtMinAge->value());
        particles->setMaxAge(ui->txtMaxAge->value());

        particles->setMinEmitSize(ui->txtMinEmitSize->value());
        particles->setMaxEmitSize(ui->txtMaxEmitSize->value());

        particles->setMinDieSize(ui->txtMinDieSize->value());
        particles->setMaxDieSize(ui->txtMaxDieSize->value());

        particles->setMinSpinSpeed(ui->txtMinSpinSpeed->value());
        particles->setMaxSpinSpeed(ui->txtMaxSpinSpeed->value());

        particles->setMinSpinAcceleration(ui->txtMinSpinAcceleration->value());
        particles->setMaxSpinAcceleration(ui->txtMaxSpinAcceleration->value());

        particles->setMinRadius(ui->txtMinRadius->value());
        particles->setMaxRadius(ui->txtMaxRadius->value());

        Magic3D::BILLBOARD billboard = Magic3D::eBILLBOARD_NONE;
        if (ui->rbBillboardHorizontal->isChecked())
        {
            billboard = Magic3D::eBILLBOARD_HORIZONTAL;
        }
        else if (ui->rbBillboardVertical->isChecked())
        {
            billboard = Magic3D::eBILLBOARD_VERTICAL;
        }
        else if (ui->rbBillboardBoth->isChecked())
        {
            billboard = Magic3D::eBILLBOARD_BOTH;
        }

        particles->setBillboardParticle(billboard);

        particles->setRecreateWhenDie(ui->chkRecreateWhenDie->isChecked());
        particles->setFollowEmitter(ui->chkFollowEmitter->isChecked());
        particles->setUseCreationDirection(ui->chkCreationDirection->isChecked());
        particles->setInvertedDirection(ui->chkInvertedDirection->isChecked());

        particles->setTrail(ui->chkTrail->isChecked());

        particles->setUV1(Magic3D::Texture2D(ui->txtU1->value(), ui->txtV1->value()));
        particles->setUV2(Magic3D::Texture2D(ui->txtU2->value(), ui->txtV2->value()));
        particles->setUV3(Magic3D::Texture2D(ui->txtU3->value(), ui->txtV3->value()));
        particles->setUV4(Magic3D::Texture2D(ui->txtU4->value(), ui->txtV4->value()));

        particles->setDistorting(ui->gbDistortion->isChecked());
        particles->setDistortionFrequency((ui->txtDistortionFrequency->value()));
        particles->setDistortionFactor((ui->txtDistortionFactor->value()));
        particles->setDistortionWave((ui->txtDistortionWave->value()));

        Magic3D::DISTORTION distortion = Magic3D::eDISTORTION_BUBBLE;
        if (ui->rbDistortionBubble->isChecked())
        {
            distortion = Magic3D::eDISTORTION_BUBBLE;
        } else if (ui->rbDistortionWave->isChecked())
        {
            distortion = Magic3D::eDISTORTION_WAVE;
        } else if (ui->rbDistortionNoise->isChecked())
        {
            distortion = Magic3D::eDISTORTION_NOISE;
        }

        particles->setDistortionType(distortion);

        particles->play();

        particles->update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}


void ParticlesInfo::on_btnMinEmitColor_clicked()
{
    Magic3D::Particles* particles = getParticles();

    if (!updating && particles)
    {
        particles->setMinEmitColor(chooseColor(particles->getMinEmitColor()));

        particles->update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void ParticlesInfo::on_btnMaxEmitColor_clicked()
{
    Magic3D::Particles* particles = getParticles();

    if (!updating && particles)
    {
        particles->setMaxEmitColor(chooseColor(particles->getMaxEmitColor()));

        particles->update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void ParticlesInfo::on_btnMinDieColor_clicked()
{
    Magic3D::Particles* particles = getParticles();

    if (!updating && particles)
    {
        particles->setMinDieColor(chooseColor(particles->getMinDieColor()));

        particles->update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void ParticlesInfo::on_btnMaxDieColor_clicked()
{
    Magic3D::Particles* particles = getParticles();

    if (!updating && particles)
    {
        particles->setMaxDieColor(chooseColor(particles->getMaxDieColor()));

        particles->update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void ParticlesInfo::on_rbBillboardNone_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_rbBillboardHorizontal_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_rbBillboardVertical_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_rbBillboardBoth_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_chkRecreateWhenDie_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_chkFollowEmitter_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_chkCreationDirection_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_chkInvertedDirection_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_chkLoop_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_btnPlay_clicked()
{
    Magic3D::Particles* particles = getParticles();

    if (particles)
    {

        particles->stop();
        particles->play();
    }
}

void ParticlesInfo::on_btnSave_clicked()
{
    if (getParticles())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Particles"), UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData()) + tr(M3D_PATH_PARTICLES), tr(MGE_PARTICLES_FILTER) );
        if (!fileName.isEmpty())
        {
            getParticles()->save(fileName.toStdString());
        }
    }
}

void ParticlesInfo::on_chkTrail_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinParticlesCount_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxParticlesCount_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtParticlesPerSecond_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinCreationDistanceX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinCreationDistanceY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinCreationDistanceZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxCreationDistanceX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxCreationDistanceY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxCreationDistanceZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinRadius_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxRadius_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinSpeed_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxSpeed_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinAcceleration_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxAcceleration_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtAccelerationX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtAccelerationY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtAccelerationZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinAge_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxAge_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinEmitSize_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxEmitSize_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinDieSize_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxDieSize_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinSpinSpeed_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxSpinSpeed_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMinSpinAcceleration_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtMaxSpinAcceleration_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtU1_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtV1_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtU2_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtV2_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtU3_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtV3_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtU4_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtV4_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtDistortionFrequency_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtDistortionFactor_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ParticlesInfo::on_txtDistortionWave_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ParticlesInfo::on_gbDistortion_toggled(bool arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void ParticlesInfo::on_rbDistortionBubble_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_rbDistortionWave_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ParticlesInfo::on_rbDistortionNoise_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}


void ParticlesInfo::on_txtTile_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateUVS();
}



void ParticlesInfo::on_txtTileColumn_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateUVS();
}

void ParticlesInfo::on_txtTileRow_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateUVS();
}

void ParticlesInfo::on_btnStop_clicked()
{
    Magic3D::Particles* particles = getParticles();

    if (particles)
    {
        particles->stop();
    }
}
