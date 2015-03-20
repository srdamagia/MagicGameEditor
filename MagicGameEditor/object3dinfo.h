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

#ifndef OBJECT3DINFO_H
#define OBJECT3DINFO_H

#include <QFileSystemModel>
#include "magic3dobjectpanel.h"
#include "camerainfo.h"
#include "lightinfo.h"
#include "soundinfo.h"
#include "tweensinfo.h"
#include "utils.h"

namespace Ui {
    class Object3DInfo;
}

namespace Magic3D
{
    class Skeleton;
    class Model;
}

class Object3DInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit Object3DInfo(MainWindow* mainWindow);
    ~Object3DInfo();

    virtual void update();
    virtual void updateSimulation();
    virtual void updateObject();

    void updateParentCombo();
    void updateParentBoneCombo();
    void listBones(Magic3D::Skeleton* bone, QStringList& items);
private:
    Ui::Object3DInfo *ui;

    CameraInfo* cameraInfo;
    LightInfo* lightInfo;
    SoundInfo* soundInfo;
    QFileSystemModel* model;

    Magic3D::Model* getModel(bool parent = false);

private slots:
    void on_cmbParentBone_currentIndexChanged(int index);
    void on_chkZOrder_toggled(bool checked);
    void on_cmbParent_currentIndexChanged(int index);
    void on_chkVisible_toggled(bool checked);
    void on_lstFiles_clicked(QModelIndex index);
    void on_chkScript_toggled(bool checked);
    void on_chkPick_toggled(bool checked);
    void on_txtPosX_valueChanged(double arg1);
    void on_txtPosY_valueChanged(double arg1);
    void on_txtPosZ_valueChanged(double arg1);
    void on_txtScaleX_valueChanged(double arg1);
    void on_txtScaleY_valueChanged(double arg1);
    void on_txtScaleZ_valueChanged(double arg1);
    void on_txtRotX_valueChanged(double arg1);
    void on_txtRotY_valueChanged(double arg1);
    void on_txtRotZ_valueChanged(double arg1);
    void on_txtFlag_valueChanged(int arg1);
    void on_chkPosition_toggled(bool checked);
    void on_chkRotation_toggled(bool checked);
    void on_chkScale_toggled(bool checked);
    void on_chkEnabled_toggled(bool checked);
    void on_chkBillboardX_toggled(bool checked);
    void on_chkBillboardY_toggled(bool checked);
    void on_txtInstance_textChanged(const QString &arg1);
    void on_chkSpawn_toggled(bool checked);
};

#endif // OBJECT3DINFO_H
