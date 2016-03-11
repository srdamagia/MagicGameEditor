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

#ifndef CONSTRAINTINFO_H
#define CONSTRAINTINFO_H

#include "magic3dobjectpanel.h"

namespace Ui {
class ConstraintInfo;
}

namespace Magic3D
{
    class PhysicsConstraint;
}

class ConstraintInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit ConstraintInfo(MainWindow* mainWindow, PhysicsInfo* physics);
    ~ConstraintInfo();

    virtual void update();
    virtual void updateObject();

    void setConstraintIndex(int index);
    int getConstraintIndex();

private slots:
    void on_txtPivotX_valueChanged(double arg1);

    void on_txtPivotY_valueChanged(double arg1);

    void on_txtPivotZ_valueChanged(double arg1);

    void on_txtAxisX_valueChanged(double arg1);

    void on_txtAxisY_valueChanged(double arg1);

    void on_txtAxisZ_valueChanged(double arg1);

    void on_cmbConnectObject_currentIndexChanged(int index);

    void on_cmbConnectBone_currentIndexChanged(int index);

    void on_txtConnectPivotX_valueChanged(double arg1);

    void on_txtConnectPivotY_valueChanged(double arg1);

    void on_txtConnectPivotZ_valueChanged(double arg1);

    void on_txtConnectAxisX_valueChanged(double arg1);

    void on_txtConnectAxisY_valueChanged(double arg1);

    void on_txtConnectAxisZ_valueChanged(double arg1);

    void on_txtLinearLowerX_valueChanged(double arg1);

    void on_txtLinearLowerY_valueChanged(double arg1);

    void on_txtLinearLowerZ_valueChanged(double arg1);

    void on_txtLinearUpperX_valueChanged(double arg1);

    void on_txtLinearUpperY_valueChanged(double arg1);

    void on_txtLinearUpperZ_valueChanged(double arg1);

    void on_txtAngularLowerX_valueChanged(double arg1);

    void on_txtAngularLowerY_valueChanged(double arg1);

    void on_txtAngularLowerZ_valueChanged(double arg1);

    void on_txtAngularUpperX_valueChanged(double arg1);

    void on_txtAngularUpperY_valueChanged(double arg1);

    void on_txtAngularUpperZ_valueChanged(double arg1);

    void on_lblTitle_toggled(bool checked);

    void on_btnDelete_clicked();

    void on_chkAngularOnly_toggled(bool checked);

    void on_chkBreakingImpulse_toggled(bool checked);

    void on_txtBreakinImpulse_valueChanged(double arg1);

    void on_txtLow_valueChanged(double arg1);

    void on_txtHigh_valueChanged(double arg1);

    void on_txtTwistSpan_valueChanged(double arg1);

    void on_txtSoftness_valueChanged(double arg1);

    void on_txtBias_valueChanged(double arg1);

    void on_txtRelaxation_valueChanged(double arg1);

    void on_txtDamping_valueChanged(double arg1);

    void on_txtThresh_valueChanged(double arg1);

    void on_gbMotor_toggled(bool arg1);

    void on_txtVelocity_valueChanged(double arg1);

    void on_txtImpulse_valueChanged(double arg1);

    void on_txtForce_valueChanged(double arg1);

private:
    Ui::ConstraintInfo *ui;
    int constraintIndex;
    bool calculateConnected;

    PhysicsInfo* physics;

    QString getConstraintType(Magic3D::PhysicsConstraint* constraint);
    Magic3D::PhysicsConstraint* getConstraint();
};

#endif // CONSTRAINTINFO_H
