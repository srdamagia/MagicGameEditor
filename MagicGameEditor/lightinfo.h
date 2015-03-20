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

#ifndef LIGHTINFO_H
#define LIGHTINFO_H

#include "magic3dobjectpanel.h"

namespace Ui {
    class LightInfo;
}

class LightInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit LightInfo(MainWindow* mainWindow);
    ~LightInfo();

    virtual void update();
    virtual void updateObject();

private:
    Ui::LightInfo *ui;

private slots:
    void on_sldSpotExponent_valueChanged(int value);
    void on_grpSpot_toggled(bool checked);
    void on_btnColorSpecular_clicked();
    void on_btnColorDiffuse_clicked();
    void on_btnColorAmbient_clicked();
    void on_chkLens_toggled(bool checked);
    void on_chkFlare_toggled(bool checked);
    void on_rbDirectional_toggled(bool checked);
    void on_rbPoint_toggled(bool checked);
    void on_rbSpot_toggled(bool checked);
    void on_txtAttenuationConstant_valueChanged(double arg1);
    void on_txtAttenuationLinear_valueChanged(double arg1);
    void on_txtAttenuationQuadratic_valueChanged(double arg1);
    void on_txtSpotCutoff_valueChanged(double arg1);
    void on_txtRadius_valueChanged(double arg1);
    void on_chkShadows_toggled(bool checked);
};

#endif // LIGHTINFO_H
