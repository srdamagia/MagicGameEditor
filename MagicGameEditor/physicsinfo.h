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

#ifndef PHYSICSINFO_H
#define PHYSICSINFO_H

#include "magic3dobjectpanel.h"
#include "constraintinfo.h"

namespace Ui {
    class PhysicsInfo;
}

class PhysicsInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit PhysicsInfo(MainWindow* mainWindow);
    ~PhysicsInfo();

    virtual void update();
    virtual void updateObject();

    void updateMeshCombo();
    QStringList& getParentsList();

private:
    Ui::PhysicsInfo *ui;

    QToolBar* toolBar;

    QList<ConstraintInfo*> constraintInfo;
    QStringList parentsList;

    void updateConstraints();

private slots:
    void on_chkTransZ_toggled(bool checked);
    void on_chkTransY_toggled(bool checked);
    void on_chkTransX_toggled(bool checked);
    void on_chkRotZ_toggled(bool checked);
    void on_chkRotY_toggled(bool checked);
    void on_chkRotX_toggled(bool checked);
    void on_cmbConvexHull_currentIndexChanged(int index);
    void on_chkConvexHull_toggled(bool checked);
    void on_chkTriangleMesh_toggled(bool checked);
    void on_chkCone_toggled(bool checked);
    void on_chkCylinder_toggled(bool checked);
    void on_chkCapsule_toggled(bool checked);
    void on_chkSphere_toggled(bool checked);
    void on_chkBox_toggled(bool checked);
    void on_chkGhost_toggled(bool checked);
    void on_txtPosX_valueChanged(double arg1);
    void on_txtPosY_valueChanged(double arg1);
    void on_txtPosZ_valueChanged(double arg1);
    void on_txtWidth_valueChanged(double arg1);
    void on_txtHeight_valueChanged(double arg1);
    void on_txtDepth_valueChanged(double arg1);
    void on_txtRadius_valueChanged(double arg1);
    void on_txtMargin_valueChanged(double arg1);
    void on_txtMass_valueChanged(double arg1);
    void on_txtFriction_valueChanged(double arg1);
    void on_txtRestitution_valueChanged(double arg1);
    void on_txtDampingLinear_valueChanged(double arg1);
    void on_txtDampingAngular_valueChanged(double arg1);
    void on_chkPhysics_toggled(bool arg1);
    void on_actionPoint_To_Point_triggered();
    void on_actionHinge_triggered();
    void on_actionSlider_triggered();
    void on_actionCone_triggered();
    void on_actionGeneric_triggered();
    void on_txtGroup_valueChanged(int arg1);
    void on_chkGrp_0_toggled(bool checked);
    void on_chkGrp_1_toggled(bool checked);
    void on_chkGrp_2_toggled(bool checked);
    void on_chkGrp_3_toggled(bool checked);
    void on_chkGrp_4_toggled(bool checked);
    void on_chkGrp_5_toggled(bool checked);
    void on_chkGrp_6_toggled(bool checked);
    void on_chkGrp_7_toggled(bool checked);
    void on_chkGrp_8_toggled(bool checked);
    void on_chkGrp_9_toggled(bool checked);
    void on_grpGroupCollision_toggled(bool arg1);
};

#endif // PHYSICSINFO_H
