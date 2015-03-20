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

#ifndef MAGIC3DOBJECTPANEL_H
#define MAGIC3DOBJECTPANEL_H

#include <QPushButton>
#include "mainwindow.h"

namespace Magic3D
{
    class PhysicsObject;
    struct ColorRGBA;
}

bool lessNone(QString &s1, QString &s2);

class Magic3DObjectPanel : public QWidget
{
    Q_OBJECT
private:
    Magic3D::PhysicsObject* physicsObject;

    QColor getIdealTextColor(const QColor& rBackgroundColor);
protected:
    bool updating;
    MainWindow* mainWindow;

    void setButtonColor(QPushButton* button, const Magic3D::ColorRGBA& color);
    Magic3D::ColorRGBA chooseColor(Magic3D::ColorRGBA color);

    void updateBoneCombo(QComboBox* combo, std::string model);
    void listBones(Magic3D::Skeleton* bone, QStringList& items);
    Magic3D::Model* getModel(std::string name);
public:
    explicit Magic3DObjectPanel(MainWindow* mainWindow);

    void setPhysicsObject(Magic3D::PhysicsObject* physicsObject);
    Magic3D::PhysicsObject* getPhysicsObject();
    Magic3D::Object* getObject();
    Magic3D::Bone* getBone();

    virtual void update() = 0;
    virtual void updateSimulation();
    virtual void updateObject() = 0;

signals:

public slots:

};

#endif // MAGIC3DOBJECTPANEL_H
