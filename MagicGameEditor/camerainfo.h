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

#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include <QWidget>
#include "magic3dobjectpanel.h"

namespace Ui {
    class CameraInfo;
}

class CameraInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit CameraInfo(MainWindow* mainWindow);
    ~CameraInfo();

    virtual void update();
    virtual void updateObject();

private:
    Ui::CameraInfo *ui;

private slots:
    void on_sldAngle_valueChanged(int value);
    void on_radOrthographic_toggled(bool checked);
    void on_radPerspective_toggled(bool checked);
    void on_txtNear_valueChanged(double arg1);
    void on_txtFar_valueChanged(double arg1);
    void on_txtInteraxial_valueChanged(int arg1);
    void on_txtInterpupillary_valueChanged(int arg1);
    void on_txtNeckDistance_valueChanged(double arg1);
    void on_txtDistortion_valueChanged(double arg1);
    void on_txtDistortionCube_valueChanged(double arg1);
    void on_txtDistortionX_valueChanged(double arg1);
    void on_txtDistortionY_valueChanged(double arg1);
};

#endif // CAMERAINFO_H
