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

#include "object2dinfo.h"
#include "ui_object2dinfo.h"
#include "spritesequence.h"
#include "textinfo.h"

#include "utils.h"
#include <magic3d/magic3d.h>

Object2DInfo::Object2DInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::Object2DInfo)
{
    ui->setupUi(this);

    textinfo = new TextInfo(mainWindow);
    spritesequence = new SpriteSequence(mainWindow);

    ui->layoutText->addWidget(textinfo);
    ui->layoutOther->addWidget(spritesequence);        
}

Object2DInfo::~Object2DInfo()
{
    if (textinfo)
    {
        delete textinfo;
        textinfo = NULL;
    }

    if (spritesequence)
    {
        delete spritesequence;
        spritesequence = NULL;
    }

    delete ui;
}

void Object2DInfo::update()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getRender() == Magic3D::eRENDER_2D)
    {
        Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;

        ui->lblName->setText(QString::fromStdString(sprite->getName()));
        if (sprite->isInParentFrustum())
        {
            ui->lblName->setStyleSheet("QLabel { color: green }");
        }
        else
        {
            ui->lblName->setStyleSheet("QLabel { color: red }");
        }

        if (sprite->getParent())
        {
            ui->cmbParent->setCurrentIndex(ui->cmbParent->findText(QString::fromStdString(sprite->getParent()->getName())));
        }
        else
        {
            ui->cmbParent->setCurrentIndex(0);
        }

        ui->chkPosition->setChecked(sprite->isParentPosition());
        ui->chkRotation->setChecked(sprite->isParentRotation());
        ui->chkScale->setChecked(sprite->isParentScale());

        ui->chkEnabled->setChecked(sprite->isEnabled());
        ui->chkVisible->setChecked(sprite->isVisible());
        ui->chkPick->setChecked(sprite->isPickable());

        if (!ui->txtPosX->hasFocus())
        {
            ui->txtPosX->setValue(sprite->getPosition().getX());
        }
        if (!ui->txtPosY->hasFocus())
        {
            ui->txtPosY->setValue(sprite->getPosition().getY());
        }

        if (!ui->txtWidth->hasFocus())
        {
            ui->txtWidth->setValue(sprite->getWidth());
        }
        if (!ui->txtHeight->hasFocus())
        {
            ui->txtHeight->setValue(sprite->getHeight());
        }

        Magic3D::Vector3 euler = object->getRotationEuler();

        if (!ui->txtRotX->hasFocus())
        {
            ui->txtRotX->setValue(euler.getX());
        }
        if (!ui->txtRotY->hasFocus())
        {
            ui->txtRotY->setValue(euler.getY());
        }
        if (!ui->txtRotZ->hasFocus())
        {
            ui->txtRotZ->setValue(euler.getZ());
        }

        if (!ui->txtScaleX->hasFocus())
        {
            ui->txtScaleX->setValue(sprite->getScale().getX());
        }
        if (!ui->txtScaleY->hasFocus())
        {
            ui->txtScaleY->setValue(sprite->getScale().getY());
        }

        ui->chkScript->setChecked(object->isScripted());

        ui->rbLeft->setChecked(sprite->getHorizontalAlign() == Magic3D::eHORIZONTAL_ALIGN_LEFT);
        ui->rbHCenter->setChecked(sprite->getHorizontalAlign() == Magic3D::eHORIZONTAL_ALIGN_CENTER);
        ui->rbRight->setChecked(sprite->getHorizontalAlign() == Magic3D::eHORIZONTAL_ALIGN_RIGHT);
        if (!ui->txtAnchorHorizontal->hasFocus())
        {
            ui->txtAnchorHorizontal->setValue(sprite->getHorizontalAnchor());
        }

        ui->rbTop->setChecked(sprite->getVerticalAlign() == Magic3D::eVERTICAL_ALIGN_TOP);
        ui->rbVCenter->setChecked(sprite->getVerticalAlign() == Magic3D::eVERTICAL_ALIGN_CENTER);
        ui->rbBottom->setChecked(sprite->getVerticalAlign() == Magic3D::eVERTICAL_ALIGN_BOTTOM);
        if (!ui->txtAnchorVertical->hasFocus())
        {
            ui->txtAnchorVertical->setValue(sprite->getVerticalAnchor());
        }        

        if (!ui->txtFlag->hasFocus())
        {
            ui->txtFlag->setValue(object->getFlag());
        }

        textinfo->setVisible(object->getType() == Magic3D::eOBJECT_GUI_LABEL);
        textinfo->setPhysicsObject(object);
        textinfo->update();

        spritesequence->setVisible(object->getType() == Magic3D::eOBJECT_SPRITE);
        spritesequence->setPhysicsObject(object);
        spritesequence->update();

        mainWindow->getMagic3D()->updateScriptView(false);
    }
    else
    {
        textinfo->setVisible(false);
        spritesequence->setVisible(false);        
    }

    updating = false;
}

void Object2DInfo::updateSimulation()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getRender() == Magic3D::eRENDER_2D)
    {
        Magic3D::Matrix4 m = object->getMatrixFromParent();
        ui->txtPosX->setValue(m.getTranslation().getX());
        ui->txtPosY->setValue(m.getTranslation().getY());
        //ui->txtPosZ->setValue(m.getTranslation().getZ());

        Magic3D::Vector3 euler = Magic3D::Math::euler(object->getRotationFromParent());

        ui->txtRotX->setValue(euler.getX());
        ui->txtRotY->setValue(euler.getY());
        ui->txtRotZ->setValue(euler.getZ());

        ui->txtScaleX->setValue(object->getScale().getX());
        ui->txtScaleY->setValue(object->getScale().getY());
    }
    updating = false;
}

void Object2DInfo::updateObject()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getRender() == Magic3D::eRENDER_2D)
    {
        Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;

        Magic3D::Vector3 position = Magic3D::Vector3(ui->txtPosX->value(), ui->txtPosY->value(), 0.0f);

        if (ui->cmbParent->currentIndex() > 0)
        {
            Magic3D::Object* obj = Magic3D::ResourceManager::getObjects()->get(ui->cmbParent->currentText().toStdString());

            if (sprite->getParent() != obj)
            {
                if (sprite->getParent())
                {
                    position = sprite->getMatrixFromParent().getTranslation();
                }

                sprite->setParent(obj);

                if (sprite->getParent())
                {
                    position = (inverse(sprite->getParent()->getMatrixFromParent()) * Magic3D::Vector4(position, 1.0f)).getXYZ();
                }
            }
        }
        else
        {
            if (sprite->getParent())
            {
                position = sprite->getMatrixFromParent().getTranslation();
            }
            sprite->setParent(NULL);
        }

        sprite->setParentRelation(ui->chkPosition->isChecked(), ui->chkRotation->isChecked(), ui->chkScale->isChecked());

        sprite->setEnabled(ui->chkEnabled->isChecked());
        sprite->setVisible(ui->chkVisible->isChecked());
        sprite->setPickable(ui->chkPick->isChecked());
        sprite->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));
        sprite->setSize(ui->txtWidth->value(), ui->txtHeight->value());
        sprite->setRotationEuler(Magic3D::Vector3(ui->txtRotX->value(), ui->txtRotY->value(), ui->txtRotZ->value()));
        sprite->setScale(Magic3D::Vector3(ui->txtScaleX->value(), ui->txtScaleY->value(), 1.0f));

        sprite->setScripted(ui->chkScript->isChecked());

        if (ui->rbLeft->isChecked())
        {
            sprite->setHorizontalAlign(Magic3D::eHORIZONTAL_ALIGN_LEFT);
        }
        else if (ui->rbHCenter->isChecked())
        {
            sprite->setHorizontalAlign(Magic3D::eHORIZONTAL_ALIGN_CENTER);
        }
        else if (ui->rbRight->isChecked())
        {
            sprite->setHorizontalAlign(Magic3D::eHORIZONTAL_ALIGN_RIGHT);
        }

        sprite->setHorizontalAnchor(ui->txtAnchorHorizontal->value());

        if (ui->rbTop->isChecked())
        {
            sprite->setVerticalAlign(Magic3D::eVERTICAL_ALIGN_TOP);
        }
        else if (ui->rbVCenter->isChecked())
        {
            sprite->setVerticalAlign(Magic3D::eVERTICAL_ALIGN_CENTER);
        }
        else if (ui->rbBottom->isChecked())
        {
            sprite->setVerticalAlign(Magic3D::eVERTICAL_ALIGN_BOTTOM);
        }

        sprite->setVerticalAnchor(ui->txtAnchorVertical->value());


        sprite->setFlag(ui->txtFlag->value());

        sprite->update();
        Magic3D::Physics::getInstance()->reset(sprite);
        update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void Object2DInfo::updateParentCombo()
{
    updating = true;
    QStringList items;
    items.append(UTILS_NONE);

    ui->cmbParent->clear();

    std::vector<Magic3D::Layer*>* layers = Magic3D::Scene::getInstance()->getLayers();
    std::vector<Magic3D::Layer*>::const_iterator it_l = layers->begin();
    while (it_l != layers->end())
    {
        Magic3D::Layer* layer = *it_l++;

        std::vector<Magic3D::Object*>* objects = layer->getObjects2D();
        std::vector<Magic3D::Object*>::const_iterator it_o = objects->begin();
        while (it_o != objects->end())
        {
            Magic3D::Object* obj = *it_o++;

            items.append(QString::fromStdString(obj->getName()));
        }

        objects = layer->getCameras();
        it_o = objects->begin();
        while (it_o != objects->end())
        {
            Magic3D::Object* obj = *it_o++;

            if (obj->getType() == Magic3D::eOBJECT_CAMERA)
            {
                Magic3D::Camera* camera = static_cast<Magic3D::Camera*>(obj);

                if (camera->getProjectionType() == Magic3D::ePROJECTION_ORTHOGRAPHIC)
                {
                    items.append(QString::fromStdString(obj->getName()));
                }
            }
        }
    }

    qSort(items.begin(), items.end(), lessNone);
    ui->cmbParent->addItems(items);
    updating = false;
}

void Object2DInfo::on_cmbParent_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void Object2DInfo::on_chkVisible_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}


void Object2DInfo::on_chkScript_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_chkPick_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtPosX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtPosY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtWidth_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtHeight_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtRotX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtRotY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtRotZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtScaleX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtScaleY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtFlag_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void Object2DInfo::on_rbLeft_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_rbHCenter_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_rbRight_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_rbTop_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_rbVCenter_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_rbBottom_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtAnchorHorizontal_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_txtAnchorVertical_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object2DInfo::on_chkPosition_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_chkRotation_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_chkScale_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object2DInfo::on_chkEnabled_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
