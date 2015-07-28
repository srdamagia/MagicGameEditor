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

#include "object3dinfo.h"
#include "ui_object3dinfo.h"
#include "meshesinfo.h"

#include <magic3d/magic3d.h>

Object3DInfo::Object3DInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::Object3DInfo)
{
    ui->setupUi(this);

    model = new QFileSystemModel();

    cameraInfo = new CameraInfo(mainWindow);
    ui->layoutOther->addWidget(cameraInfo);

    lightInfo = new LightInfo(mainWindow);
    ui->layoutOther->addWidget(lightInfo);

    soundInfo = new SoundInfo(mainWindow);
    ui->layoutOther->addWidget(soundInfo);
}

Object3DInfo::~Object3DInfo()
{
    if (cameraInfo)
    {
        delete cameraInfo;
        cameraInfo = NULL;
    }

    if (lightInfo)
    {
        delete lightInfo;
        lightInfo = NULL;
    }

    if (soundInfo)
    {
        delete soundInfo;
        soundInfo = NULL;
    }

    if (model)
    {
        ui->lstFiles->setModel(NULL);
        delete model;
        model = NULL;
    }
    delete ui;
}

Magic3D::Model* Object3DInfo::getModel(bool parent)
{
    Magic3D::Object* object = parent ? getObject()->getParent() : getObject();
    Magic3D::Model* model = NULL;

    if (object && object->getType() == Magic3D::eOBJECT_MODEL)
    {
        model = (Magic3D::Model*)object;
    }
    return model;
}

void Object3DInfo::update()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getRender() == Magic3D::eRENDER_3D)
    {
        bool isModel = object->getType() == Magic3D::eOBJECT_MODEL;
        bool isSound = object->getType() == Magic3D::eOBJECT_SOUND;
        bool isParticles = object->getType() == Magic3D::eOBJECT_PARTICLES;
        bool isInstance = object->getType() == Magic3D::eOBJECT_INSTANCE;
        ui->grpInstance->setVisible(isInstance);
        ui->grpFile->setVisible(isModel || isSound || isParticles);
        if (isModel)
        {
            QString path = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData()) + "/" + M3D_PATH_MODEL;
            model->setRootPath(path);
            model->setNameFilters(QStringList("*.dae"));
            model->setNameFilterDisables(false);
            ui->lstFiles->setModel(model);
            ui->lstFiles->setRootIndex(model->index(path));
        }
        if (isSound)
        {
            QString path = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData()) + "/" + M3D_PATH_SOUND;
            model->setRootPath(path);
            model->setNameFilters(QStringList("*.ogg"));
            ui->lstFiles->setModel(model);
            ui->lstFiles->setRootIndex(model->index(path));
        }
        if (isParticles)
        {
            QString path = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData()) + "/" + M3D_PATH_PARTICLES;
            model->setRootPath(path);
            model->setNameFilters(QStringList("*.particles"));
            ui->lstFiles->setModel(model);
            ui->lstFiles->setRootIndex(model->index(path));
        }

        ui->lblName->setText(QString::fromStdString(object->getName()));
        if (object->isInParentFrustum())
        {
            ui->lblName->setStyleSheet("QLabel { color: green }");
        }
        else
        {
            ui->lblName->setStyleSheet("QLabel { color: red }");
        }

        if (object->getParent())
        {
            ui->cmbParent->setCurrentIndex(ui->cmbParent->findText(QString::fromStdString(object->getParent()->getName())));
        }
        else
        {
            ui->cmbParent->setCurrentIndex(0);
        }

        ui->cmbParentBone->setCurrentIndex(0);
        if (object->getParent() && object->getParent()->getType() == Magic3D::eOBJECT_MODEL)
        {
            updateParentBoneCombo();
            ui->cmbParentBone->setEnabled(true);
            if (object->getParentBone())
            {
                ui->cmbParentBone->setCurrentIndex(ui->cmbParentBone->findText(QString::fromStdString(object->getParentBone()->getName())));
            }
        }
        else
        {
            ui->cmbParentBone->setEnabled(false);
        }

        ui->chkPosition->setChecked(object->isParentPosition());
        ui->chkRotation->setChecked(object->isParentRotation());
        ui->chkScale->setChecked(object->isParentScale());

        ui->chkEnabled->setChecked(object->isEnabled());
        ui->chkVisible->setVisible(object->getType() != Magic3D::eOBJECT_CAMERA);
        ui->chkVisible->setChecked(object->isVisible());
        ui->chkZOrder->setVisible(object->getType() != Magic3D::eOBJECT_CAMERA);
        ui->chkZOrder->setChecked(object->isZOrder());
        ui->chkPick->setVisible(object->getType() != Magic3D::eOBJECT_CAMERA);
        ui->chkPick->setChecked(object->isPickable());

        if (!ui->txtPosX->hasFocus())
        {
            ui->txtPosX->setValue(object->getPosition().getX());
        }
        if (!ui->txtPosY->hasFocus())
        {
            ui->txtPosY->setValue(object->getPosition().getY());
        }
        if (!ui->txtPosZ->hasFocus())
        {
            ui->txtPosZ->setValue(object->getPosition().getZ());
        }

        if (!ui->txtScaleX->hasFocus())
        {
            ui->txtScaleX->setValue(object->getScale().getX());
        }
        if (!ui->txtScaleY->hasFocus())
        {
            ui->txtScaleY->setValue(object->getScale().getY());
        }
        if (!ui->txtScaleZ->hasFocus())
        {
            ui->txtScaleZ->setValue(object->getScale().getZ());
        }

        //Magic3D::Vector3 euler = Magic3D::Math::euler(object->getRotation());
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

        ui->chkScript->setChecked(object->isScripted());

        if (isInstance)
        {
            Magic3D::ObjectInstance* instance = static_cast<Magic3D::ObjectInstance*>(object);
            if (instance->getInstance())
            {
                ui->txtInstance->setText(QString::fromStdString(instance->getInstance()->getName()));
            }
            else
            {
                ui->txtInstance->clear();
            }
            ui->chkSpawn->setChecked(instance->isSpawnedOnLoad());
        }

        cameraInfo->setVisible(object->getType() == Magic3D::eOBJECT_CAMERA);
        cameraInfo->setPhysicsObject(object);
        cameraInfo->update();

        lightInfo->setVisible(object->getType() == Magic3D::eOBJECT_LIGHT);
        lightInfo->setPhysicsObject(object);
        lightInfo->update();

        soundInfo->setVisible(object->getType() == Magic3D::eOBJECT_SOUND);
        soundInfo->setPhysicsObject(object);
        soundInfo->update();

        Magic3D::Box box = object->getBoundingBox();
        ui->lblWidth->setText(tr("width: %0").arg(box.getWidth() * object->getScale().getX(), 0, 'f', 3));
        ui->lblHeight->setText(tr("height: %0").arg(box.getHeight() * object->getScale().getY(), 0, 'f', 3));
        ui->lblDepth->setText(tr("depth: %0").arg(box.getDepth() * object->getScale().getZ(), 0, 'f', 3));

        if (isModel)
        {
            ui->txtFile->setText(QString::fromStdString(getModel()->getFileName()));
        }
        if (isSound)
        {
            ui->txtFile->setText(QString::fromStdString(((Magic3D::Sound*)object)->getFileName()));
        }

        if (!ui->txtFlag->hasFocus())
        {
            ui->txtFlag->setValue(object->getFlag());
        }

        ui->chkBillboardX->setChecked(object->getBillboard() == Magic3D::eBILLBOARD_HORIZONTAL || object->getBillboard() == Magic3D::eBILLBOARD_BOTH);
        ui->chkBillboardY->setChecked(object->getBillboard() == Magic3D::eBILLBOARD_VERTICAL || object->getBillboard() == Magic3D::eBILLBOARD_BOTH);

        mainWindow->getMagic3D()->updateScriptView(false);

        if (object->getType() == Magic3D::eOBJECT_CAMERA)
        {
            Magic3D::Vector3 pos = object->getPosition();
            mainWindow->getMagic3D()->getCameraVector()->setX(pos.getX());
            mainWindow->getMagic3D()->getCameraVector()->setY(pos.getY());
            mainWindow->getMagic3D()->getCameraVector()->setZ(pos.getZ());
        }
    }
    else
    {
        cameraInfo->setVisible(false);
        lightInfo->setVisible(false);
        soundInfo->setVisible(false);
        ui->lstFiles->setModel(NULL);
    }

    updating = false;
}

void Object3DInfo::updateSimulation()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getRender() == Magic3D::eRENDER_3D)
    {
        if (Magic3D::Physics::getInstance()->isPlaying())
        {
            Magic3D::Matrix4 m = object->getMatrixFromParent();
            if (!ui->txtPosX->hasFocus())
                ui->txtPosX->setValue(m.getTranslation().getX());
            if (!ui->txtPosY->hasFocus())
                ui->txtPosY->setValue(m.getTranslation().getY());
            if (!ui->txtPosZ->hasFocus())
                ui->txtPosZ->setValue(m.getTranslation().getZ());

            Magic3D::Vector3 euler = Magic3D::Math::euler(object->getRotation());

            if (!ui->txtRotX->hasFocus())
                ui->txtRotX->setValue(euler.getX());
            if (!ui->txtRotY->hasFocus())
                ui->txtRotY->setValue(euler.getY());
            if (!ui->txtRotZ->hasFocus())
                ui->txtRotZ->setValue(euler.getZ());

            Magic3D::Vector3 scale = object->getScale();
            if (!ui->txtScaleX->hasFocus())
                ui->txtScaleX->setValue(scale.getX());
            if (!ui->txtScaleY->hasFocus())
                ui->txtScaleY->setValue(scale.getY());
            if (!ui->txtScaleZ->hasFocus())
                ui->txtScaleZ->setValue(scale.getZ());
        }
        else
        {
            ui->txtPosX->setValue(object->getPosition().getX());
            ui->txtPosY->setValue(object->getPosition().getY());
            ui->txtPosZ->setValue(object->getPosition().getZ());

            ui->txtScaleX->setValue(object->getScale().getX());
            ui->txtScaleY->setValue(object->getScale().getY());
            ui->txtScaleZ->setValue(object->getScale().getZ());

            Magic3D::Vector3 euler = object->getRotationEuler();

            ui->txtRotX->setValue(euler.getX());
            ui->txtRotY->setValue(euler.getY());
            ui->txtRotZ->setValue(euler.getZ());
        }
    }
    updating = false;
}

void Object3DInfo::updateObject()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getRender() == Magic3D::eRENDER_3D)
    {
        float x = ui->txtPosX->value();
        float y = ui->txtPosY->value();
        float z = ui->txtPosZ->value();

        std::string boneName;

        if (ui->cmbParentBone->currentIndex() > 0)
        {
            boneName = ui->cmbParentBone->currentText().toStdString();
        }

        Magic3D::Object* objParent = NULL;
        if (ui->cmbParent->currentIndex() > 0)
        {
            objParent = Magic3D::ResourceManager::getObjects()->get(ui->cmbParent->currentText().toStdString());
        }

        if (object->getParent() && object->getParent() != objParent)
        {
            Magic3D::Object* obj = object->getParent();

            x += obj->getMatrixFromParent().getTranslation().getX();
            y += obj->getMatrixFromParent().getTranslation().getY();
            z += obj->getMatrixFromParent().getTranslation().getZ();
        }

        if (objParent)
        {
            if (object->getParent() != objParent)
            {
                object->setParent(objParent);

                if (object->getParent())
                {
                    x -= objParent->getMatrixFromParent().getTranslation().getX();
                    y -= objParent->getMatrixFromParent().getTranslation().getY();
                    z -= objParent->getMatrixFromParent().getTranslation().getZ();
                }
            }
        }
        else
        {
            object->setParent(NULL);
        }

        if (ui->cmbParentBone->currentIndex() > 0)
        {
            if (!boneName.empty())
            {
                Magic3D::Model* model = getModel(true);
                if (model && model->getSkeleton())
                {
                    object->setParentBone(model->getSkeleton()->getBoneByName(boneName));
                }
            }
        }
        else
        {
            object->setParentBone(NULL);
        }

        object->setParentRelation(ui->chkPosition->isChecked(), ui->chkRotation->isChecked(), ui->chkScale->isChecked());

        object->setEnabled(ui->chkEnabled->isChecked());
        object->setVisible(ui->chkVisible->isChecked());
        object->setZOrder(ui->chkZOrder->isChecked());
        object->setPickable(ui->chkPick->isChecked());

        object->setPosition(Magic3D::Vector3(x, y, z));
        object->setScale(Magic3D::Vector3(ui->txtScaleX->value(), ui->txtScaleY->value(), ui->txtScaleZ->value()));
        object->setRotationEuler(Magic3D::Vector3(ui->txtRotX->value(), ui->txtRotY->value(), ui->txtRotZ->value()));

        object->setScripted(ui->chkScript->isChecked());

        if (object->getType() == Magic3D::eOBJECT_INSTANCE)
        {
            Magic3D::ObjectInstance* instance = static_cast<Magic3D::ObjectInstance*>(object);
            instance->setSpawnOnLoad(ui->chkSpawn->isChecked());
        }

        object->setFlag(ui->txtFlag->value());

        if (ui->chkBillboardX->isChecked() && ui->chkBillboardY->isChecked())
        {
            object->setBillboard(Magic3D::eBILLBOARD_BOTH);
        }
        else if (ui->chkBillboardX->isChecked())
        {
            object->setBillboard(Magic3D::eBILLBOARD_HORIZONTAL);
        }
        else if (ui->chkBillboardY->isChecked())
        {
            object->setBillboard(Magic3D::eBILLBOARD_VERTICAL);
        }
        else
        {
            object->setBillboard(Magic3D::eBILLBOARD_NONE);
        }

        object->update();
        Magic3D::Physics::getInstance()->reset(object);
        update();

        if (object->getType() == Magic3D::eOBJECT_CAMERA)
        {
            Magic3D::Vector3 pos = object->getPositionFromParent();
            mainWindow->getMagic3D()->getCameraVector()->setX(pos.getX());
            mainWindow->getMagic3D()->getCameraVector()->setY(pos.getY());
            mainWindow->getMagic3D()->getCameraVector()->setZ(pos.getZ());
        }

        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void Object3DInfo::updateParentCombo()
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

        std::vector<Magic3D::Object*>* objects = layer->getObjects3D();
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

                if (camera->getProjectionType() == Magic3D::ePROJECTION_PERSPECTIVE)
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

void Object3DInfo::updateParentBoneCombo()
{
    //updating = true;
    QStringList items;
    items.append(UTILS_NONE);

    ui->cmbParentBone->clear();

    if (getObject() && getObject()->getParent() && getObject()->getParent()->getType() == Magic3D::eOBJECT_MODEL)
    {

        Magic3D::Model* model = getModel(true);

        if (model && model->getSkeleton())
        {
            listBones(model->getSkeleton(), items);
        }
    }

    qSort(items.begin(), items.end(), lessNone);
    ui->cmbParentBone->addItems(items);
    //updating = false;
}

void Object3DInfo::listBones(Magic3D::Skeleton* bone, QStringList& items)
{
    std::vector<Magic3D::Bone*>* bones = bone->getBones();
    std::vector<Magic3D::Bone*>::const_iterator it_b = bones->begin();
    while (it_b != bones->end())
    {
        Magic3D::Bone* bone = *it_b++;

        items.append(QString::fromStdString(bone->getName()));
    }
}

void Object3DInfo::on_lstFiles_clicked(QModelIndex index)
{

    Magic3D::Object* object = getObject();
    if (!updating && object && !model->isDir(index))
    {
        switch (object->getType())
        {
            case Magic3D::eOBJECT_MODEL:
            {
                getModel()->setFileName(model->data(index).toString().toStdString());
                while (getModel()->load()){};

                mainWindow->updateFromEngine();
                mainWindow->updateInfo();

                QString name = QString::fromStdString(object->getName());
                mainWindow->selectObjectByName(name, true);
                break;
            }
            case Magic3D::eOBJECT_SOUND:
            {
                ((Magic3D::Sound*)object)->setFileName(model->data(index).toString().toStdString());
                break;
            }
            case Magic3D::eOBJECT_PARTICLES:
            {
                ((Magic3D::Particles*)object)->load(model->data(index).toString().toStdString());
                mainWindow->updateInfo();
                break;
            }
            default: break;
        }

        object->update();
        update();

        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void Object3DInfo::on_chkVisible_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_cmbParent_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkZOrder_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_cmbParentBone_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkScript_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkPick_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtPosX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtPosY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtPosZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtScaleX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtScaleY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtScaleZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtRotX_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtRotY_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtRotZ_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtFlag_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkPosition_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkRotation_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkScale_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkEnabled_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkBillboardX_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_chkBillboardY_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void Object3DInfo::on_txtInstance_textChanged(const QString &arg1)
{
    if (!updating)
    {
        Magic3D::Object* object = getObject();
        if (object && object->getType() == Magic3D::eOBJECT_INSTANCE)
        {
            Magic3D::Object* instance = Magic3D::ResourceManager::getInstance()->getObjects()->get(arg1.toStdString());
            if (instance != static_cast<Magic3D::ObjectInstance*>(object)->getInstance())
            {
                static_cast<Magic3D::ObjectInstance*>(object)->setInstance(instance);
                mainWindow->setProjectChanged(true);
                mainWindow->update();
            }
        }
    }
}

void Object3DInfo::on_chkSpawn_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
