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

#include "constraintinfo.h"
#include "physicsinfo.h"
#include "ui_constraintinfo.h"
#include "utils.h"

ConstraintInfo::ConstraintInfo(MainWindow* mainWindow, PhysicsInfo* physics) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::ConstraintInfo)
{
    this->physics = physics;
    ui->setupUi(this);

    calculateConnected = false;
}

ConstraintInfo::~ConstraintInfo()
{
    delete ui;
}

QString ConstraintInfo::getConstraintType(Magic3D::PhysicsConstraint* constraint)
{
    QString result = "Constraint";
    if (constraint)
    {
        switch (constraint->getType())
        {
            case Magic3D::eCONSTRAINT_CUSTOM:         result = "6DOF Custom"; break;
            case Magic3D::eCONSTRAINT_POINT_TO_POINT: result = "Point To Point"; break;
            case Magic3D::eCONSTRAINT_HINGE:          result = "Hinge"; break;
            case Magic3D::eCONSTRAINT_SLIDER:         result = "Slider"; break;
            case Magic3D::eCONSTRAINT_CONE:           result = "Cone"; break;
        }
    }

    return result;
}

void ConstraintInfo::update()
{
    updating = true;
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    Magic3D::PhysicsConstraint* constraint = getConstraint();

    if (physicsObject && constraint)
    {
        ui->lblTitle->setText(tr("%0 - %1").arg(constraintIndex).arg(getConstraintType(constraint)));

        ui->lblTitle->setChecked(constraint->getConstraint() != NULL);

        if (!ui->txtPivotX->hasFocus())
        {
            ui->txtPivotX->setValue(constraint->getPivot().getX());
        }
        if (!ui->txtPivotY->hasFocus())
        {
            ui->txtPivotY->setValue(constraint->getPivot().getY());
        }
        if (!ui->txtPivotZ->hasFocus())
        {
            ui->txtPivotZ->setValue(constraint->getPivot().getZ());
        }

        if (!ui->txtAxisX->hasFocus())
        {
            ui->txtAxisX->setValue(constraint->getAxis().getX());
        }
        if (!ui->txtAxisY->hasFocus())
        {
            ui->txtAxisY->setValue(constraint->getAxis().getY());
        }
        if (!ui->txtAxisZ->hasFocus())
        {
            ui->txtAxisZ->setValue(constraint->getAxis().getZ());
        }

        if (!ui->txtConnectAxisX->hasFocus())
        {
            ui->txtConnectAxisX->setValue(constraint->getConnectedAxis().getX());
        }
        if (!ui->txtConnectAxisY->hasFocus())
        {
            ui->txtConnectAxisY->setValue(constraint->getConnectedAxis().getY());
        }
        if (!ui->txtConnectAxisZ->hasFocus())
        {
            ui->txtConnectAxisZ->setValue(constraint->getConnectedAxis().getZ());
        }

        if (!ui->txtConnectPivotX->hasFocus())
        {
            ui->txtConnectPivotX->setValue(constraint->getConnectedPivot().getX());
        }
        if (!ui->txtConnectPivotY->hasFocus())
        {
            ui->txtConnectPivotY->setValue(constraint->getConnectedPivot().getY());
        }
        if (!ui->txtConnectPivotZ->hasFocus())
        {
            ui->txtConnectPivotZ->setValue(constraint->getConnectedPivot().getZ());
        }

        if (!ui->txtLinearLowerX->hasFocus())
        {
            ui->txtLinearLowerX->setValue(constraint->getLinearLowerLimit().getX());
        }
        if (!ui->txtLinearLowerY->hasFocus())
        {
            ui->txtLinearLowerY->setValue(constraint->getLinearLowerLimit().getY());
        }
        if (!ui->txtLinearLowerZ->hasFocus())
        {
            ui->txtLinearLowerZ->setValue(constraint->getLinearLowerLimit().getZ());
        }

        if (!ui->txtLinearUpperX->hasFocus())
        {
            ui->txtLinearUpperX->setValue(constraint->getLinearUpperLimit().getX());
        }
        if (!ui->txtLinearUpperY->hasFocus())
        {
            ui->txtLinearUpperY->setValue(constraint->getLinearUpperLimit().getY());
        }
        if (!ui->txtLinearUpperZ->hasFocus())
        {
            ui->txtLinearUpperZ->setValue(constraint->getLinearUpperLimit().getZ());
        }

        if (!ui->txtAngularLowerX->hasFocus())
        {
            ui->txtAngularLowerX->setValue(constraint->getAngularLowerLimit().getX());
        }
        if (!ui->txtAngularLowerY->hasFocus())
        {
            ui->txtAngularLowerY->setValue(constraint->getAngularLowerLimit().getY());
        }
        if (!ui->txtAngularLowerZ->hasFocus())
        {
            ui->txtAngularLowerZ->setValue(constraint->getAngularLowerLimit().getZ());
        }

        if (!ui->txtAngularUpperX->hasFocus())
        {
            ui->txtAngularUpperX->setValue(constraint->getAngularUpperLimit().getX());
        }
        if (!ui->txtAngularUpperY->hasFocus())
        {
            ui->txtAngularUpperY->setValue(constraint->getAngularUpperLimit().getY());
        }
        if (!ui->txtAngularUpperZ->hasFocus())
        {
            ui->txtAngularUpperZ->setValue(constraint->getAngularUpperLimit().getZ());
        }

        bool hingecone = constraint->getType() == Magic3D::eCONSTRAINT_HINGE || constraint->getType() == Magic3D::eCONSTRAINT_CONE;
        ui->chkAngularOnly->setChecked(constraint->isAngularOnly());
        ui->chkAngularOnly->setVisible(hingecone);

        ui->chkBreakingImpulse->setChecked(constraint->isBreakable());
        ui->txtBreakinImpulse->setValue(constraint->isBreakable() ? constraint->getBreakingImpulse() : 0.0f);

        ui->txtLow->setValue(constraint->getLimitLow());
        ui->txtHigh->setValue(constraint->getLimitLow());
        ui->txtTwistSpan->setValue(constraint->getLimitLow());
        ui->txtSoftness->setValue(constraint->getLimitLow());
        ui->txtBias->setValue(constraint->getLimitLow());
        ui->txtRelaxation->setValue(constraint->getLimitLow());
        ui->txtDamping->setValue(constraint->getLimitLow());
        ui->txtThresh->setValue(constraint->getLimitLow());

        ui->gbLimits->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CUSTOM || constraint->getType() == Magic3D::eCONSTRAINT_SLIDER || hingecone);
        ui->pnl6DOF->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CUSTOM);
        ui->pnlParams->setVisible(hingecone);
        ui->lblLow->setText(constraint->getType() == Magic3D::eCONSTRAINT_HINGE ? "Low:" : "Swing Span 1:");
        ui->lblHigh->setText(constraint->getType() == Magic3D::eCONSTRAINT_HINGE ? "High:" : "Swing Span 2:");
        ui->lblTwistSpan->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CONE);
        ui->txtTwistSpan->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CONE);
        ui->lblDamping->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CONE);
        ui->txtDamping->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CONE);
        ui->lblThresh->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CONE);
        ui->txtThresh->setVisible(constraint->getType() == Magic3D::eCONSTRAINT_CONE);

        ui->cmbConnectObject->clear();
        mainWindow->setComboList(physics->getParentsList(), ui->cmbConnectObject);

        if (!constraint->getConnectedObject().empty())
        {
            ui->cmbConnectObject->setCurrentIndex(ui->cmbConnectObject->findText(QString::fromStdString(constraint->getConnectedObject())));
        }
        else
        {
            ui->cmbConnectObject->setCurrentIndex(ui->cmbConnectObject->findText(UTILS_NONE));
        }

        ui->cmbConnectBone->setCurrentIndex(0);
        if (getModel(constraint->getConnectedObject()))
        {
            updateBoneCombo(ui->cmbConnectBone, constraint->getConnectedObject());
            ui->cmbConnectBone->setEnabled(true);
            if (!constraint->getConnectedBone().empty())
            {
                ui->cmbConnectBone->setCurrentIndex(ui->cmbConnectBone->findText(QString::fromStdString(constraint->getConnectedBone())));
            }
        }
        else
        {
            ui->cmbConnectBone->setEnabled(false);
        }
    }
    updating = false;
}

void ConstraintInfo::updateObject()
{
    Magic3D::PhysicsConstraint* constraint = getConstraint();
    if (!updating && constraint)
    {
        std::string nextObject;
        std::string nextBone;

        constraint->setPivot(Magic3D::Vector3(ui->txtPivotX->value(), ui->txtPivotY->value(), ui->txtPivotZ->value()));
        constraint->setAxis(Magic3D::Vector3(ui->txtAxisX->value(), ui->txtAxisY->value(), ui->txtAxisZ->value()));
        constraint->setConnectedPivot(Magic3D::Vector3(ui->txtConnectPivotX->value(), ui->txtConnectPivotY->value(), ui->txtConnectPivotZ->value()));
        constraint->setConnectedAxis(Magic3D::Vector3(ui->txtConnectAxisX->value(), ui->txtConnectAxisY->value(), ui->txtConnectAxisZ->value()));

        constraint->setLinearLowerLimit(Magic3D::Vector3(ui->txtLinearLowerX->value(), ui->txtLinearLowerY->value(), ui->txtLinearLowerZ->value()));
        constraint->setLinearUpperLimit(Magic3D::Vector3(ui->txtLinearUpperX->value(), ui->txtLinearUpperY->value(), ui->txtLinearUpperZ->value()));
        constraint->setAngularLowerLimit(Magic3D::Vector3(ui->txtAngularLowerX->value(), ui->txtAngularLowerY->value(), ui->txtAngularLowerZ->value()));
        constraint->setAngularUpperLimit(Magic3D::Vector3(ui->txtAngularUpperX->value(), ui->txtAngularUpperY->value(), ui->txtAngularUpperZ->value()));

        constraint->setAngularOnly(ui->chkAngularOnly->isChecked());

        bool breakable = constraint->isBreakable();
        constraint->setBreakable(ui->chkBreakingImpulse->isChecked());
        if (constraint->isBreakable())
        {
            if (breakable)
            {
                constraint->setBreakingImpulse(ui->txtBreakinImpulse->value());
            }
            else
            {
                constraint->setBreakingImpulse(0.0f);
            }
        }
        else
        {
            constraint->setBreakingImpulse(SIMD_INFINITY);
        }

        switch(constraint->getType())
        {
            case Magic3D::eCONSTRAINT_HINGE:
            {
                constraint->setHingeLimit(ui->txtLow->value(),
                                          ui->txtHigh->value(),
                                          ui->txtSoftness->value(),
                                          ui->txtBias->value(),
                                          ui->txtRelaxation->value());
                break;
            }
            case Magic3D::eCONSTRAINT_CONE:
            {
                constraint->setConeLimit(ui->txtLow->value(),
                                         ui->txtHigh->value(),
                                         ui->txtTwistSpan->value(),
                                         ui->txtSoftness->value(),
                                         ui->txtBias->value(),
                                         ui->txtRelaxation->value());
                break;
            }
            default: break;
        }

        constraint->setDamping(ui->txtDamping->value());
        constraint->setThresh(ui->txtThresh->value());

        if (ui->cmbConnectObject->currentIndex() > 0)
        {
            nextObject = ui->cmbConnectObject->currentText().toStdString();
        }
        else
        {
            nextObject.clear();
        }

        if (ui->cmbConnectBone->currentIndex() > 0)
        {
            nextBone = ui->cmbConnectBone->currentText().toStdString();

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

        constraint->setConnected(nextObject, nextBone);

        Magic3D::PhysicsObject* object = getPhysicsObject();
        Magic3D::PhysicsObject* connected = constraint->getConnected();

        if (object && connected && connected->getRigidBody() && calculateConnected)
        {
            btTransform transformObj;
            btTransform transformCon;
            object->getRigidBody()->getMotionState()->getWorldTransform(transformObj);
            connected->getRigidBody()->getMotionState()->getWorldTransform(transformCon);

            btVector3 vobj = transformObj.getOrigin();
            btVector3 vcon = transformCon.getOrigin();

            /*btQuaternion rot = transformObj.getRotation();
            Magic3D::Quaternion qObj = Magic3D::Quaternion(rot.getX(), rot.getY(), rot.getZ(), rot.getW());

            rot = transformCon.getRotation();
            Magic3D::Quaternion qCon = Magic3D::Quaternion(rot.getX(), rot.getY(), rot.getZ(), rot.getW());

            Magic3D::Vector3 rotObj = Magic3D::Math::euler(qObj);
            Magic3D::Vector3 rotCon = Magic3D::Math::euler(qCon);*/

            Magic3D::Vector3 mid = Magic3D::Math::vector3(transformCon.inverse().getBasis() * (vobj-vcon));

            constraint->setConnectedPivot(mid);


            //constraint->setAxis(rotObj);
            //constraint->setConnectedAxis(rotCon);
        }

        if (ui->lblTitle->isChecked())
        {
            constraint->connect();
        }
        else
        {
            constraint->disconnect();
        }

        calculateConnected = false;

        mainWindow->setProjectChanged(true);
        mainWindow->update();

        update();
    }
}

void ConstraintInfo::setConstraintIndex(int index)
{
    constraintIndex = index;
}

int ConstraintInfo::getConstraintIndex()
{
    return constraintIndex;
}

Magic3D::PhysicsConstraint* ConstraintInfo::getConstraint()
{
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    Magic3D::PhysicsConstraint* result = NULL;
    if (physicsObject)
    {
        result = physicsObject->getConstraint(constraintIndex);
    }

    return result;
}

void ConstraintInfo::on_txtPivotX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtPivotY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtPivotZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAxisX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAxisY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAxisZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_cmbConnectObject_currentIndexChanged(int index)
{
    if (index > 0)
    {

    }
    if (!updating)
    {
        calculateConnected = true;
    }
    updateObject();
}

void ConstraintInfo::on_cmbConnectBone_currentIndexChanged(int index)
{
    if (index > 0)
    {

    }
    if (!updating)
    {
        calculateConnected = true;
    }
    updateObject();
}

void ConstraintInfo::on_txtConnectPivotX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtConnectPivotY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtConnectPivotZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtConnectAxisX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtConnectAxisY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtConnectAxisZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLinearLowerX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLinearLowerY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLinearLowerZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLinearUpperX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLinearUpperY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLinearUpperZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAngularLowerX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAngularLowerY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAngularLowerZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAngularUpperX_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAngularUpperY_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtAngularUpperZ_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_lblTitle_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ConstraintInfo::on_chkAngularOnly_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ConstraintInfo::on_chkBreakingImpulse_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtBreakinImpulse_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtLow_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtHigh_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtTwistSpan_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtSoftness_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtBias_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtRelaxation_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtDamping_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtThresh_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_gbMotor_toggled(bool arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtVelocity_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtImpulse_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_txtForce_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void ConstraintInfo::on_btnDelete_clicked()
{
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    if (physicsObject)
    {
        physicsObject->removeConstraint(constraintIndex);
        mainWindow->setProjectChanged(true);
        mainWindow->update();
        physics->update();
    }
}
