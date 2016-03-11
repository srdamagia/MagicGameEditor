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

#ifndef TWEENSINFO_H
#define TWEENSINFO_H

#include <QToolBar>
#include "magic3dobjectpanel.h"
#include "tweeninfo.h"

namespace Ui {
class TweensInfo;
}

class TweensInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit TweensInfo(MainWindow* mainWindow);
    ~TweensInfo();

    virtual void update();
    virtual void updateObject();

    void saveObject(Magic3D::Tween* tween);
    void resetObject(bool update = false);

    QStringList& getParentsList();

private slots:
    void on_actionTranslate_triggered();

    void on_actionRotate_triggered();

    void on_actionScale_triggered();

    void on_actionAlpha_triggered();

    void on_actionGo_To_triggered();

    void on_actionLook_At_triggered();

    void on_actionDelay_triggered();

    void on_actionCustom_triggered();

private:
    Ui::TweensInfo *ui;

    QToolBar* toolBar;

    QList<TweenInfo*> tweenInfo;
    QStringList parentsList;

    Magic3D::Vector3 position;
    Magic3D::Quaternion rotation;
    Magic3D::Vector3 scale;
    Magic3D::Vector3 anchor;
    QList<float> alpha;
    Magic3D::Tween* tween;
    QString nextTweenObject;
    QString nextTweenBone;
    int nextTweenIndex;
};

#endif // TWEENSINFO_H
