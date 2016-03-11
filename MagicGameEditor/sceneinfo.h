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

#ifndef SCENEINFO_H
#define SCENEINFO_H

#include "magic3dobjectpanel.h"

namespace Ui {
    class SceneInfo;
}

class SceneInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit SceneInfo(MainWindow* mainWindow);
    ~SceneInfo();

    virtual void update();
    virtual void updateObject();

    void updateCamerasCombo();

private:
    Ui::SceneInfo *ui;

private slots:
    void on_sldVolumeMaster_valueChanged(int value);
    void on_btnColorFog_clicked();
    void on_btnColorAmbient_clicked();
    void on_btnColorClear_clicked();
    void on_txtFogStart_valueChanged(double arg1);
    void on_txtFogFinish_valueChanged(double arg1);
    void on_txtGravityX_valueChanged(double arg1);
    void on_txtGravityY_valueChanged(double arg1);
    void on_txtGravityZ_valueChanged(double arg1);
    void on_txtGrid3DX_valueChanged(double arg1);
    void on_txtGrid3DY_valueChanged(double arg1);
    void on_txtGrid3DZ_valueChanged(double arg1);
    void on_txtGrid2DX_valueChanged(double arg1);
    void on_txtGrid2DY_valueChanged(double arg1);
    void on_cmbFBOCamera_currentIndexChanged(int index);
    void on_txtWindX_valueChanged(double arg1);
    void on_txtWindY_valueChanged(double arg1);
    void on_txtWindZ_valueChanged(double arg1);
    void on_chkScreenEffects_toggled(bool checked);
    void on_txtTimeScale_valueChanged(double arg1);
};

#endif // SCENEINFO_H
