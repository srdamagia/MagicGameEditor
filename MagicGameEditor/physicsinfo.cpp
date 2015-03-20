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

#include "physicsinfo.h"
#include "ui_physicsinfo.h"
#include <magic3d/magic3d.h>

PhysicsInfo::PhysicsInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::PhysicsInfo)
{
    ui->setupUi(this);

    toolBar = new QToolBar(this);
    toolBar->addAction(ui->actionPoint_To_Point);
    toolBar->addAction(ui->actionHinge);
    toolBar->addAction(ui->actionSlider);
    toolBar->addAction(ui->actionCone);
    toolBar->addAction(ui->actionGeneric);
    ui->layoutConstraintToolBar->addWidget(toolBar);
}

PhysicsInfo::~PhysicsInfo()
{
    if (toolBar)
    {
        delete toolBar;
        toolBar = NULL;
    }

    delete ui;
}

void PhysicsInfo::update()
{
    updating = true;

    Magic3D::PhysicsObject* object = getPhysicsObject();
    if (object)
    {
        bool isBone = object->getType() == Magic3D::eOBJECT_BONE;
        ui->chkPhysics->setChecked(object->getRigidBody());

        ui->gbCollision->setEnabled(ui->chkPhysics->isChecked());
        ui->gbBody->setEnabled(ui->chkPhysics->isChecked());

        ui->chkBox->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_BOX);
        ui->chkSphere->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_SPHERE);
        ui->chkCapsule->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_CAPSULE);
        ui->chkCylinder->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_CYLINDER);
        ui->chkCone->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_CONE);
        ui->chkConvexHull->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_CONVEXHULL);
        ui->chkTriangleMesh->setChecked(object->getShape() == Magic3D::ePHYSICS_SHAPE_TRIANGLEMESH);

        ui->chkConvexHull->setEnabled(object->getRender() != Magic3D::eRENDER_2D);
        ui->chkTriangleMesh->setEnabled(object->getRender() != Magic3D::eRENDER_2D);

        ui->cmbConvexHull->setEnabled(object->getShape() == Magic3D::ePHYSICS_SHAPE_CONVEXHULL || object->getShape() == Magic3D::ePHYSICS_SHAPE_TRIANGLEMESH);

        ui->txtWidth->setEnabled(object->getShape() == Magic3D::ePHYSICS_SHAPE_BOX || object->getShape() == Magic3D::ePHYSICS_SHAPE_CYLINDER);
        ui->txtHeight->setEnabled(object->getShape() != Magic3D::ePHYSICS_SHAPE_SPHERE && object->getShape() != Magic3D::ePHYSICS_SHAPE_TRIANGLEMESH && object->getShape() != Magic3D::ePHYSICS_SHAPE_CONVEXHULL);
        ui->txtDepth->setEnabled(object->getShape() == Magic3D::ePHYSICS_SHAPE_BOX || object->getShape() == Magic3D::ePHYSICS_SHAPE_CYLINDER);

        if (!isBone && getObject()->getCollisionMesh())
        {
            ui->cmbConvexHull->setCurrentIndex(getObject()->getCollisionMeshIndex());
        }
        else
        {
            ui->cmbConvexHull->setCurrentIndex(0);
        }

        ui->txtRadius->setEnabled(object->getShape() == Magic3D::ePHYSICS_SHAPE_SPHERE || object->getShape() == Magic3D::ePHYSICS_SHAPE_CAPSULE || object->getShape() == Magic3D::ePHYSICS_SHAPE_CONE);

        if (!ui->txtPosX->hasFocus())
        {
            ui->txtPosX->setValue(object->getShapePosition().getX());
        }
        if (!ui->txtPosY->hasFocus())
        {
            ui->txtPosY->setValue(object->getShapePosition().getY());
        }
        if (!ui->txtPosZ->hasFocus())
        {
            ui->txtPosZ->setValue(object->getShapePosition().getZ());
        }

        if (!ui->txtWidth->hasFocus())
        {
            ui->txtWidth->setValue(object->getShapeSize().getX());
        }
        if (!ui->txtHeight->hasFocus())
        {
            ui->txtHeight->setValue(object->getShapeSize().getY());
        }
        if (!ui->txtDepth->hasFocus())
        {
            ui->txtDepth->setValue(object->getShapeSize().getZ());
        }

        ui->chkRotX->setChecked(object->getShapeRotation().getX() != 0.0f);
        ui->chkRotY->setChecked(object->getShapeRotation().getY() != 0.0f);
        ui->chkRotZ->setChecked(object->getShapeRotation().getZ() != 0.0f);

        ui->chkTransX->setChecked(object->getShapeTranslation().getX() != 0.0f);
        ui->chkTransY->setChecked(object->getShapeTranslation().getY() != 0.0f);
        ui->chkTransZ->setChecked(object->getShapeTranslation().getZ() != 0.0f);

        if (!ui->txtRadius->hasFocus())
        {
            ui->txtRadius->setValue(object->getRadius());
        }
        if (!ui->txtMargin->hasFocus())
        {
            ui->txtMargin->setValue(object->getMargin());
        }
        if (!ui->txtMass->hasFocus())
        {
            ui->txtMass->setValue(object->getMass());
        }
        if (!ui->txtFriction->hasFocus())
        {
            ui->txtFriction->setValue(object->getFriction());
        }
        if (!ui->txtRestitution->hasFocus())
        {
            ui->txtRestitution->setValue(object->getRestitution());
        }
        if (!ui->txtDampingLinear->hasFocus())
        {
            ui->txtDampingLinear->setValue(object->getDampingLinear());
        }
        if (!ui->txtDampingAngular->hasFocus())
        {
            ui->txtDampingAngular->setValue(object->getDampingAngular());
        }

        ui->chkGhost->setChecked(object->isGhost());

        updateConstraints();
    }

    updating = false;
}

void PhysicsInfo::updateConstraints()
{
    Magic3D::PhysicsObject* physicsObject = getPhysicsObject();
    if (physicsObject)
    {
        mainWindow->updateParentsList(false, true);
        parentsList = mainWindow->getParentsList();

        int count = physicsObject->getConstraintCount();
        while (constraintInfo.size() < count)
        {
            ConstraintInfo* pnl = new ConstraintInfo(mainWindow, this);
            pnl->setPhysicsObject(physicsObject);
            pnl->setConstraintIndex(constraintInfo.size());
            constraintInfo.append(pnl);
            ui->layoutConstraints->addWidget(pnl);
        }
        while (constraintInfo.size() > count)
        {
            ConstraintInfo* pnl = constraintInfo.takeLast();
            ui->layoutConstraints->removeWidget(pnl);
            delete pnl;
        }

        for (int i = 0; i < constraintInfo.size(); i++)
        {
            ConstraintInfo* pnl = constraintInfo[i];
            if (pnl)
            {
                pnl->setPhysicsObject(physicsObject);
                pnl->setConstraintIndex(i);
                pnl->update();
            }
        }
    }
}

void PhysicsInfo::updateObject()
{
    Magic3D::PhysicsObject* object = getPhysicsObject();
    if (object && !updating)
    {
        bool isBone = object->getType() == Magic3D::eOBJECT_BONE;
        if (ui->chkPhysics->isChecked())
        {
            Magic3D::PHYSICS_SHAPE shape = Magic3D::ePHYSICS_SHAPE_BOX;

            if (ui->chkSphere->isChecked())
            {
                shape = Magic3D::ePHYSICS_SHAPE_SPHERE;
            }
            else if (ui->chkCapsule->isChecked())
            {
                shape = Magic3D::ePHYSICS_SHAPE_CAPSULE;
            }
            else if (ui->chkCylinder->isChecked())
            {
                shape = Magic3D::ePHYSICS_SHAPE_CYLINDER;
            }
            else if (ui->chkCone->isChecked())
            {
                shape = Magic3D::ePHYSICS_SHAPE_CONE;
            }
            else if (ui->chkTriangleMesh->isChecked())
            {
                shape = Magic3D::ePHYSICS_SHAPE_TRIANGLEMESH;
            }
            else if (ui->chkConvexHull->isChecked())
            {
                shape = Magic3D::ePHYSICS_SHAPE_CONVEXHULL;
            }

            object->setShape(shape);

            object->setShapePosition(Magic3D::Vector3(ui->txtPosX->value(), ui->txtPosY->value(), ui->txtPosZ->value()));
            object->setShapeSize(Magic3D::Vector3(ui->txtWidth->value(), ui->txtHeight->value(), ui->txtDepth->value()));

            object->setShapeRotation(Magic3D::Vector3(ui->chkRotX->isChecked() ? 1.0f : 0.0f, ui->chkRotY->isChecked() ? 1.0f : 0.0f, ui->chkRotZ->isChecked() ? 1.0f : 0.0f));
            object->setShapeTranslation(Magic3D::Vector3(ui->chkTransX->isChecked() ? 1.0f : 0.0f, ui->chkTransY->isChecked() ? 1.0f : 0.0f, ui->chkTransZ->isChecked() ? 1.0f : 0.0f));

            object->setRadius(ui->txtRadius->value());
            object->setMargin(ui->txtMargin->value());
            object->setMass(ui->txtMass->value());
            object->setFriction(ui->txtFriction->value());
            object->setRestitution(ui->txtRestitution->value());
            object->setDampingLinear(ui->txtDampingLinear->value());
            object->setDampingAngular(ui->txtDampingAngular->value());
            object->setGhost(ui->chkGhost->isChecked());

            if (!isBone && getObject() && (shape == Magic3D::ePHYSICS_SHAPE_CONVEXHULL || shape == Magic3D::ePHYSICS_SHAPE_TRIANGLEMESH))
            {
                int mesh = ui->cmbConvexHull->currentIndex();
                if (mesh >= 0)
                {
                    getObject()->setCollisionMeshIndex(mesh);
                }
                else
                {
                    getObject()->setCollisionMeshIndex(-1);
                }
            }

            if (object->getShape() == Magic3D::ePHYSICS_SHAPE_TRIANGLEMESH)
            {
                object->setMass(0.0f);
            }

            Magic3D::Physics::getInstance()->add(object);
        }
        else
        {
            if (object->getRigidBody())
            {
                Magic3D::Physics::getInstance()->remove(object);
            }
        }

        if (getObject())
        {
            getObject()->update();
        }
        update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();

    }
}

void PhysicsInfo::updateMeshCombo()
{
    if (getObject())
    {
        updating = true;
        QStringList items;

        ui->cmbConvexHull->clear();

        std::vector<Magic3D::Mesh*>* meshes = getObject()->getMeshes();
        std::vector<Magic3D::Mesh*>::const_iterator it_m = meshes->begin();
        while (it_m != meshes->end())
        {
            Magic3D::Mesh* mesh = *it_m++;

            items.append(QString::fromStdString(mesh->getData()->getName()));
        }

        ui->cmbConvexHull->addItems(items);
        updating = false;
    }
}

QStringList& PhysicsInfo::getParentsList()
{
    return parentsList;
}

void PhysicsInfo::on_chkBox_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkSphere_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkCapsule_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkCylinder_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkCone_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkTriangleMesh_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkConvexHull_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_cmbConvexHull_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkRotX_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkRotY_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkRotZ_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkTransX_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkTransY_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkTransZ_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkGhost_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtPosX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtPosY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtPosZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtWidth_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtHeight_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtDepth_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtRadius_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtMargin_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtMass_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtFriction_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtRestitution_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtDampingLinear_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_txtDampingAngular_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void PhysicsInfo::on_chkPhysics_toggled(bool arg1)
{
    if (arg1)
    {

    }
    updateObject();
}

void PhysicsInfo::on_actionPoint_To_Point_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addConstraint(new Magic3D::PhysicsConstraint(Magic3D::eCONSTRAINT_POINT_TO_POINT));
        updateObject();
    }
}

void PhysicsInfo::on_actionHinge_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addConstraint(new Magic3D::PhysicsConstraint(Magic3D::eCONSTRAINT_HINGE));
        updateObject();
    }
}

void PhysicsInfo::on_actionSlider_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addConstraint(new Magic3D::PhysicsConstraint(Magic3D::eCONSTRAINT_SLIDER));
        updateObject();
    }
}

void PhysicsInfo::on_actionCone_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addConstraint(new Magic3D::PhysicsConstraint(Magic3D::eCONSTRAINT_CONE));
        updateObject();
    }
}

void PhysicsInfo::on_actionGeneric_triggered()
{
    if (getPhysicsObject())
    {
        getPhysicsObject()->addConstraint(new Magic3D::PhysicsConstraint(Magic3D::eCONSTRAINT_CUSTOM));
        updateObject();
    }
}
