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

#ifndef BONEINFO_H
#define BONEINFO_H

#include <QWidget>
#include "magic3dobjectpanel.h"

namespace Ui {
    class BoneInfo;
}

class BoneInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit BoneInfo(MainWindow* mainWindow);
    ~BoneInfo();

    virtual void update();
    virtual void updateObject();
    virtual void updateSimulation();

private:
    Ui::BoneInfo *ui;

private slots:

    void on_chkAnimate_toggled(bool checked);
    void on_txtPosX_valueChanged(double arg1);
    void on_txtPosY_valueChanged(double arg1);
    void on_txtPosZ_valueChanged(double arg1);
    void on_txtRotX_valueChanged(double arg1);
    void on_txtRotY_valueChanged(double arg1);
    void on_txtRotZ_valueChanged(double arg1);
    void on_txtScaleX_valueChanged(double arg1);
    void on_txtScaleY_valueChanged(double arg1);
    void on_txtScaleZ_valueChanged(double arg1);
};

#endif // BONEINFO_H
