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

#ifndef TWEENINFO_H
#define TWEENINFO_H

#include "magic3dobjectpanel.h"

namespace Ui {
class TweenInfo;
}

namespace Magic3D
{
    class Tween;
}

class TweenInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit TweenInfo(MainWindow* mainWindow, TweensInfo* tweens);
    ~TweenInfo();

    virtual void update();
    virtual void updateObject();

    void setTweenIndex(int index);
    int getTweenIndex();

private slots:
    void on_btnPlay_clicked();

    void on_btnStop_clicked();

    void on_btnDelete_clicked();

    void on_cmbUpdateType_currentIndexChanged(int index);

    void on_txtRepeat_valueChanged(int arg1);

    void on_txtStartValue_valueChanged(double arg1);

    void on_txtSpeed_valueChanged(double arg1);

    void on_txtDuration_valueChanged(double arg1);

    void on_txtX_valueChanged(double arg1);

    void on_txtY_valueChanged(double arg1);

    void on_txtZ_valueChanged(double arg1);

    void on_txtNextIndex_valueChanged(int arg1);

    void on_chkPlayAtStart_toggled(bool checked);

    void on_chkAllwaysUpdate_toggled(bool checked);

    void on_cmbNextTweenObject_currentIndexChanged(int index);

    void on_cmbNextTweenBone_currentIndexChanged(int index);

    void on_chkIgnoreTimeScale_toggled(bool checked);

private:
    Ui::TweenInfo *ui;
    int tweenIndex;

    TweensInfo* tweens;

    QString getTweenType(Magic3D::Tween* tween);
    Magic3D::Tween* getTween();
};

#endif // TWEENINFO_H
