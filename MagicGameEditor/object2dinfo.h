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

#ifndef OBJECT2DINFO_H
#define OBJECT2DINFO_H

#include "magic3dobjectpanel.h"

namespace Ui {
    class Object2DInfo;
}

class SpriteSequence;
class TweensInfo;

class Object2DInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit Object2DInfo(MainWindow* mainWindow);
    ~Object2DInfo();

    void loadFontsList();

    virtual void update();
    virtual void updateSimulation();
    virtual void updateObject();

    void updateParentCombo();

private:
    Ui::Object2DInfo *ui;

    SpriteSequence* spritesequence;
    bool eventFilter(QObject *object, QEvent *event);
private slots:
    void on_txtText_textChanged();
    void on_chkVisible_toggled(bool checked);
    void on_cmbParent_currentIndexChanged(int index);
    void on_chkScript_toggled(bool checked);
    void on_chkPick_toggled(bool checked);
    void on_txtPosX_valueChanged(double arg1);
    void on_txtPosY_valueChanged(double arg1);
    void on_txtWidth_valueChanged(double arg1);
    void on_txtHeight_valueChanged(double arg1);
    void on_txtRotX_valueChanged(double arg1);
    void on_txtRotY_valueChanged(double arg1);
    void on_txtRotZ_valueChanged(double arg1);
    void on_txtFontSize_valueChanged(double arg1);
    void on_txtFlag_valueChanged(int arg1);
    void on_rbLeft_toggled(bool checked);
    void on_rbHCenter_toggled(bool checked);
    void on_rbRight_toggled(bool checked);
    void on_rbTop_toggled(bool checked);
    void on_rbVCenter_toggled(bool checked);
    void on_rbBottom_toggled(bool checked);
    void on_txtAnchorHorizontal_valueChanged(double arg1);
    void on_txtAnchorVertical_valueChanged(double arg1);
    void on_chkPosition_toggled(bool checked);
    void on_chkRotation_toggled(bool checked);
    void on_chkScale_toggled(bool checked);
    void on_chkEnabled_toggled(bool checked);
    void on_rbTextAlignLeft_toggled(bool checked);
    void on_rbTextAlignCenter_toggled(bool checked);
    void on_rbTextAlignRight_toggled(bool checked);
    void on_btnTextColor_clicked();
    void on_cmbFont_currentIndexChanged(int index);
};

#endif // OBJECT2DINFO_H
