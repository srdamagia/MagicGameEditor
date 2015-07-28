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

#include "tweeninfo.h"
#include "ui_tweeninfo.h"
#include "utils.h"
#include "tweensinfo.h"

#include <magic3d/magic3d.h>

TweenInfo::TweenInfo(MainWindow* mainWindow, TweensInfo* tweens) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::TweenInfo)
{
    this->tweens = tweens;
    ui->setupUi(this);

    ui->cmbUpdateType->addItem("Circular Easing In");
    ui->cmbUpdateType->addItem("Circular Easing In Out");
    ui->cmbUpdateType->addItem("Circular Easing Out");
    ui->cmbUpdateType->addItem("Cubic Easing In");
    ui->cmbUpdateType->addItem("Cubic Easing In Out");
    ui->cmbUpdateType->addItem("Cubic Easing Out");
    ui->cmbUpdateType->addItem("Elastic Easing In");
    ui->cmbUpdateType->addItem("Elastic Easing In Out");
    ui->cmbUpdateType->addItem("Elastic Easing Out");
    ui->cmbUpdateType->addItem("Exponential Easing In");
    ui->cmbUpdateType->addItem("Exponential Easing In Out");
    ui->cmbUpdateType->addItem("Exponential Easing Out");
    ui->cmbUpdateType->addItem("Quadratic Easing In");
    ui->cmbUpdateType->addItem("Quadratic Easing In Out");
    ui->cmbUpdateType->addItem("Quadratic Easing Out");
    ui->cmbUpdateType->addItem("Quartic Easing In");
    ui->cmbUpdateType->addItem("Quartic Easing In Out");
    ui->cmbUpdateType->addItem("Quartic Easing Out");
    ui->cmbUpdateType->addItem("Quintic Easing In");
    ui->cmbUpdateType->addItem("Quintic Easing In Out");
    ui->cmbUpdateType->addItem("Quintic Easing Out");
    ui->cmbUpdateType->addItem("Simple Linear");
    ui->cmbUpdateType->addItem("Sinusoidal Easing In");
    ui->cmbUpdateType->addItem("Sinusoidal Easing In Out");
    ui->cmbUpdateType->addItem("Sinusoidal Easing Out");
}

TweenInfo::~TweenInfo()
{
    tweens = NULL;
    delete ui;
}

QString TweenInfo::getTweenType(Magic3D::Tween* tween)
{
    QString result = "Tween";
    if (tween)
    {
        switch (tween->getType())
        {
            case Magic3D::eTWEEN_ALPHA:     result = "Alpha"; break;
            case Magic3D::eTWEEN_DELAY:     result = "Delay"; break;
            case Magic3D::eTWEEN_ROTATE:    result = "Rotate"; break;
            case Magic3D::eTWEEN_SCALE:     result = "Scale"; break;
            case Magic3D::eTWEEN_TRANSLATE: result = "Translate"; break;
            case Magic3D::eTWEEN_GOTO:      result = "Go To"; break;
            case Magic3D::eTWEEN_LOOKAT:    result = "Look At"; break;
            case Magic3D::eTWEEN_CUSTOM:    result = "Custom"; break;
        }
    }

    return result;
}

void TweenInfo::update()
{
    updating = true;
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    Magic3D::Tween* tween = getTween();

    if (physicsObject && tween)
    {
        ui->lblTitle->setText(tr("%0 - %1").arg(tweenIndex).arg(getTweenType(tween)));

        ui->chkPlayAtStart->setChecked(tween->isPlayingAtStart());
        ui->chkAllwaysUpdate->setChecked(tween->isAllwaysUpdated());
        ui->chkIgnoreTimeScale->setChecked(tween->isIgnoringTimeScale());
        ui->cmbUpdateType->setCurrentIndex(tween->getTweenUpdateType());

        if (!ui->txtRepeat->hasFocus())
        {
            ui->txtRepeat->setValue(tween->getRepeat());
        }

        if (!ui->txtStartValue->hasFocus())
        {
            ui->txtStartValue->setValue(tween->getStartValue());
        }

        if (!ui->txtSpeed->hasFocus())
        {
            ui->txtSpeed->setValue(tween->getSpeed());
        }

        if (!ui->txtDuration->hasFocus())
        {
            ui->txtDuration->setValue(tween->getDuration());
        }

        switch (tween->getType())
        {
            case Magic3D::eTWEEN_ALPHA:
            {
                if (!ui->txtX->hasFocus())
                {
                    ui->lblX->setText("Alpha:");
                    ui->txtX->setValue(static_cast<Magic3D::TweenAlpha*>(tween)->getAlpha());
                }
                break;
            }
            case Magic3D::eTWEEN_ROTATE:
            {
                ui->lblX->setText("Rotate x:");

                Magic3D::TweenRotate* rotate = static_cast<Magic3D::TweenRotate*>(tween);
                if (!ui->txtX->hasFocus())
                {
                    ui->txtX->setValue(rotate->getEuler().getX());
                }
                if (!ui->txtY->hasFocus())
                {
                    ui->txtY->setValue(rotate->getEuler().getY());
                }
                if (!ui->txtZ->hasFocus())
                {
                    ui->txtZ->setValue(rotate->getEuler().getZ());
                }
                break;
            }
            case Magic3D::eTWEEN_SCALE:
            {
                ui->lblX->setText("Scale x:");

                Magic3D::TweenScale* scale = static_cast<Magic3D::TweenScale*>(tween);
                if (!ui->txtX->hasFocus())
                {
                    ui->txtX->setValue(scale->getScale().getX());
                }
                if (!ui->txtY->hasFocus())
                {
                    ui->txtY->setValue(scale->getScale().getY());
                }
                if (!ui->txtZ->hasFocus())
                {
                    ui->txtZ->setValue(scale->getScale().getZ());
                }
                break;
            }
            case Magic3D::eTWEEN_TRANSLATE:
            {
                ui->lblX->setText("Translate x:");

                Magic3D::TweenTranslate* translate = static_cast<Magic3D::TweenTranslate*>(tween);
                if (!ui->txtX->hasFocus())
                {
                    ui->txtX->setValue(translate->getPosition().getX());
                }
                if (!ui->txtY->hasFocus())
                {
                    ui->txtY->setValue(translate->getPosition().getY());
                }
                if (!ui->txtZ->hasFocus())
                {
                    ui->txtZ->setValue(translate->getPosition().getZ());
                }
                break;
            }
            case Magic3D::eTWEEN_GOTO:
            {
                ui->lblX->setText("Position x:");

                Magic3D::TweenGoTo* goTo = static_cast<Magic3D::TweenGoTo*>(tween);
                if (!ui->txtX->hasFocus())
                {
                    ui->txtX->setValue(goTo->getPosition().getX());
                }
                if (!ui->txtY->hasFocus())
                {
                    ui->txtY->setValue(goTo->getPosition().getY());
                }
                if (!ui->txtZ->hasFocus())
                {
                    ui->txtZ->setValue(goTo->getPosition().getZ());
                }
                break;
            }
            case Magic3D::eTWEEN_LOOKAT:
            {
                ui->lblX->setText("Rotation x:");

                Magic3D::TweenLookAt* lookAt = static_cast<Magic3D::TweenLookAt*>(tween);
                if (!ui->txtX->hasFocus())
                {
                    ui->txtX->setValue(lookAt->getRotation().getX());
                }
                if (!ui->txtY->hasFocus())
                {
                    ui->txtY->setValue(lookAt->getRotation().getY());
                }
                if (!ui->txtZ->hasFocus())
                {
                    ui->txtZ->setValue(lookAt->getRotation().getZ());
                }
                break;
            }
            //case Magic3D::eTWEEN_DELAY: break;
            //case Magic3D::eTWEEN_CUSTOM: break;
            default: break;
        }
        bool visible = tween->getType() != Magic3D::eTWEEN_CUSTOM && tween->getType() != Magic3D::eTWEEN_DELAY;
        ui->lblX->setVisible(visible);
        ui->txtX->setVisible(visible);
        ui->lblY->setVisible(visible && tween->getType() != Magic3D::eTWEEN_ALPHA);
        ui->txtY->setVisible(visible && tween->getType() != Magic3D::eTWEEN_ALPHA);
        ui->lblZ->setVisible(visible && tween->getType() != Magic3D::eTWEEN_ALPHA);
        ui->txtZ->setVisible(visible && tween->getType() != Magic3D::eTWEEN_ALPHA);
        ui->spacerCustom->setVisible(visible && tween->getType() == Magic3D::eTWEEN_ALPHA);
        ui->spacerCustom2->setVisible(visible && tween->getType() == Magic3D::eTWEEN_ALPHA);

        ui->cmbNextTweenObject->clear();
        mainWindow->setComboList(tweens->getParentsList(), ui->cmbNextTweenObject);

        if (!tween->getNextTweenObject().empty())
        {
            ui->cmbNextTweenObject->setCurrentIndex(ui->cmbNextTweenObject->findText(QString::fromStdString(tween->getNextTweenObject())));
            if (!ui->txtNextIndex->hasFocus())
            {
                ui->txtNextIndex->setValue(tween->getNextTweenIndex());
            }
        }
        else
        {
            ui->cmbNextTweenObject->setCurrentIndex(ui->cmbNextTweenObject->findText(UTILS_NONE));
            ui->txtNextIndex->setValue(0);
        }

        ui->cmbNextTweenBone->setCurrentIndex(0);
        if (getModel(tween->getNextTweenObject()))
        {
            updateBoneCombo(ui->cmbNextTweenBone, tween->getNextTweenObject());
            ui->cmbNextTweenBone->setEnabled(true);
            if (!tween->getNextTweenBone().empty())
            {
                ui->cmbNextTweenBone->setCurrentIndex(ui->cmbNextTweenBone->findText(QString::fromStdString(tween->getNextTweenBone())));
            }
        }
        else
        {
            ui->cmbNextTweenBone->setEnabled(false);
        }
    }
    updating = false;
}

void TweenInfo::updateObject()
{
    Magic3D::Tween* tween = getTween();
    if (!updating && tween)
    {
        std::string nextObject;
        std::string nextBone;

        tween->setPlayingAtStart(ui->chkPlayAtStart->isChecked());
        tween->setAllwaysUpdate(ui->chkAllwaysUpdate->isChecked());
        tween->setIgnoreTimeScale(ui->chkIgnoreTimeScale->isChecked());
        tween->setTweenUpdateType((Magic3D::TweenUpdateType)ui->cmbUpdateType->currentIndex());

        tween->setRepeat(ui->txtRepeat->value());
        tween->setStartValue(ui->txtStartValue->value());
        tween->setSpeed(ui->txtSpeed->value());
        tween->setDuration(ui->txtDuration->value());

        switch (tween->getType())
        {
            case Magic3D::eTWEEN_ALPHA:
            {
                static_cast<Magic3D::TweenAlpha*>(tween)->setAlpha(ui->txtX->value());
                break;
            }
            case Magic3D::eTWEEN_ROTATE:
            {
                Magic3D::TweenRotate* rotate = static_cast<Magic3D::TweenRotate*>(tween);
                rotate->setEuler(Magic3D::Vector3(ui->txtX->value(), ui->txtY->value(), ui->txtZ->value()));
                break;
            }
            case Magic3D::eTWEEN_SCALE:
            {
                Magic3D::TweenScale* scale = static_cast<Magic3D::TweenScale*>(tween);
                scale->setScale(Magic3D::Vector3(ui->txtX->value(), ui->txtY->value(), ui->txtZ->value()));
                break;
            }
            case Magic3D::eTWEEN_TRANSLATE:
            {
                Magic3D::TweenTranslate* translate = static_cast<Magic3D::TweenTranslate*>(tween);
                translate->setPosition(Magic3D::Vector3(ui->txtX->value(), ui->txtY->value(), ui->txtZ->value()));
                break;
            }
            case Magic3D::eTWEEN_GOTO:
            {
                Magic3D::TweenGoTo* goTo = static_cast<Magic3D::TweenGoTo*>(tween);
                goTo->setPosition(Magic3D::Vector3(ui->txtX->value(), ui->txtY->value(), ui->txtZ->value()));
                break;
            }
            case Magic3D::eTWEEN_LOOKAT:
            {
                Magic3D::TweenLookAt* lookAt = static_cast<Magic3D::TweenLookAt*>(tween);
                lookAt->setRotation(Magic3D::Vector3(ui->txtX->value(), ui->txtY->value(), ui->txtZ->value()));
                break;
            }
            //case Magic3D::eTWEEN_DELAY: break;
            //case Magic3D::eTWEEN_CUSTOM: break;
            default: break;
        }

        if (ui->cmbNextTweenObject->currentIndex() > 0)
        {
            nextObject = ui->cmbNextTweenObject->currentText().toStdString();
        }
        else
        {
            nextObject.clear();
        }

        if (ui->cmbNextTweenBone->currentIndex() > 0)
        {
            nextBone = ui->cmbNextTweenBone->currentText().toStdString();

            Magic3D::Model* model = getModel(nextObject);
            if (model && model->getSkeleton())
            {
                if (!model->getSkeleton()->getBoneByName(nextBone))
                {
                    nextBone.clear();
                }
            }
        }
        else
        {
            nextBone.clear();
        }

        tween->setNextTween(nextObject, nextBone, ui->txtNextIndex->value());

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void TweenInfo::setTweenIndex(int index)
{
    tweenIndex = index;
}

int TweenInfo::getTweenIndex()
{
    return tweenIndex;
}

Magic3D::Tween* TweenInfo::getTween()
{
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    Magic3D::Tween* result = NULL;
    if (physicsObject)
    {
        result = physicsObject->getTween(tweenIndex);
    }

    return result;
}


void TweenInfo::on_btnPlay_clicked()
{
    Magic3D::Tween* tween = getTween();
    if (tween)
    {
        tweens->saveObject(tween);
        tween->play();
        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void TweenInfo::on_btnStop_clicked()
{
    tweens->resetObject();
    mainWindow->setProjectChanged(true);
    mainWindow->update();

    update();
}

void TweenInfo::on_btnDelete_clicked()
{
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    if (physicsObject)
    {
        tweens->resetObject();
        physicsObject->removeTween(tweenIndex);
        mainWindow->setProjectChanged(true);
        mainWindow->update();
        tweens->update();
    }
}

void TweenInfo::on_cmbUpdateType_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void TweenInfo::on_txtRepeat_valueChanged(int arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtStartValue_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtSpeed_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtDuration_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtX_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtY_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtZ_valueChanged(double arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_txtNextIndex_valueChanged(int arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void TweenInfo::on_chkPlayAtStart_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void TweenInfo::on_chkAllwaysUpdate_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void TweenInfo::on_cmbNextTweenObject_currentIndexChanged(int index)
{
    if (index > 0)
    {

    }
    updateObject();
}

void TweenInfo::on_cmbNextTweenBone_currentIndexChanged(int index)
{
    if (index > 0)
    {

    }
    updateObject();
}

void TweenInfo::on_chkIgnoreTimeScale_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
