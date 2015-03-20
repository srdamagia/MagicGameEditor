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

#ifndef CAMERAOPTIONS_H
#define CAMERAOPTIONS_H

#include <QWidget>

#include "mainwindow.h"

#define CAMERA_MOVE_SPEED 100.0f
#define CAMERA_LOOK_SPEED 100.0f

namespace Ui {
    class CameraOptions;
}

class CameraOptions : public QWidget
{
    Q_OBJECT

public:
    explicit CameraOptions(MainWindow *parent = 0);
    ~CameraOptions();

    void setSpeedMoveFactor(float speed);
    void setSpeedLookFactor(float speed);

    float getSpeedMoveFactor();
    float getSpeedLookFactor();

    float getSpeedMove();
    float getSpeedLook();

private:
    Ui::CameraOptions *ui;

    MainWindow* mainWindow;
    bool loading;

private slots:
    void on_sldLook_valueChanged(int value);
    void on_sldMove_valueChanged(int value);
};

#endif // CAMERAOPTIONS_H
