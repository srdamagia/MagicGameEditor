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

#include "magic3dobjectpanel.h"
#include "utils.h"

#include <magic3d/physics/physics_object.h>

bool lessNone(QString &s1, QString &s2)
{
    if (s1.toLower() == UTILS_NONE)
    {
        return true;
    }
    else if (s2.toLower() == UTILS_NONE)
    {
        return false;
    }
    else
    {
        return s1.toLower() < s2.toLower();
    }
}

Magic3DObjectPanel::Magic3DObjectPanel(MainWindow* mainWindow) :
    QWidget(mainWindow)
{
    updating = false;
    this->mainWindow = mainWindow;
    physicsObject = NULL;
}

void Magic3DObjectPanel::setPhysicsObject(Magic3D::PhysicsObject* physicsObject)
{
    this->physicsObject = physicsObject;
}

Magic3D::PhysicsObject* Magic3DObjectPanel::getPhysicsObject()
{
    return physicsObject;
}

Magic3D::Object* Magic3DObjectPanel::getObject()
{
    return physicsObject && physicsObject->getType() != Magic3D::eOBJECT_BONE ? static_cast<Magic3D::Object*>(physicsObject) : NULL;
}

Magic3D::Bone* Magic3DObjectPanel::getBone()
{
    return physicsObject && physicsObject->getType() == Magic3D::eOBJECT_BONE ? static_cast<Magic3D::Bone*>(physicsObject) : NULL;
}

void Magic3DObjectPanel::updateSimulation()
{

}

void Magic3DObjectPanel::setButtonColor(QPushButton* button, const Magic3D::ColorRGBA& color)
{
    QString COLOR_STYLE("* { background-color : %1; color : %2; }");

    QColor rgba(color.r * 255, color.g * 255, color.b * 255, color.a * 255);

    if (button)
    {
        button->setStyleSheet(COLOR_STYLE.arg(rgba.name()).arg(getIdealTextColor(rgba).name()));
        button->setText(tr("%1, %2, %3, %4").arg(rgba.red()).arg(rgba.green()).arg(rgba.blue()).arg(rgba.alpha()));
        button->setAutoFillBackground(true);
    }
}


Magic3D::ColorRGBA Magic3DObjectPanel::chooseColor(Magic3D::ColorRGBA color)
{
    float factor = 255.0f;
    QColor qColor(color.r * factor, color.g * factor, color.b * factor, color.a * factor);

    qColor = Utils::getColor(qColor);

    Magic3D::ColorRGBA result = Magic3D::ColorRGBA(qColor.red() / factor, qColor.green() / factor, qColor.blue() / factor, qColor.alpha() / factor);
    return result;
}

QColor Magic3DObjectPanel::getIdealTextColor(const QColor& rBackgroundColor)
{
    const int THRESHOLD = 105;
    int BackgroundDelta = (rBackgroundColor.red() * 0.299) + (rBackgroundColor.green() * 0.587) + (rBackgroundColor.blue() * 0.114);
    return QColor((255- BackgroundDelta < THRESHOLD) ? Qt::black : Qt::white);
}

void Magic3DObjectPanel::updateBoneCombo(QComboBox* combo, std::string model)
{
    //updating = true;
    QStringList items;
    items.append(UTILS_NONE);

    combo->clear();

    Magic3D::Model* m = getModel(model);

    if (m && m->getSkeleton())
    {
        listBones(m->getSkeleton(), items);
    }

    qSort(items.begin(), items.end(), lessNone);
    combo->addItems(items);
    //updating = false;
}

void Magic3DObjectPanel::listBones(Magic3D::Skeleton* bone, QStringList& items)
{
    std::vector<Magic3D::Bone*>* bones = bone->getBones();
    std::vector<Magic3D::Bone*>::const_iterator it_b = bones->begin();
    while (it_b != bones->end())
    {
        Magic3D::Bone* bone = *it_b++;

        items.append(QString::fromStdString(bone->getName()));
    }
}

Magic3D::Model* Magic3DObjectPanel::getModel(std::string name)
{
    Magic3D::Model* model = NULL;
    if (!name.empty())
    {
        Magic3D::Object* object = Magic3D::ResourceManager::getObjects()->get(name);


        if (object && object->getType() == Magic3D::eOBJECT_MODEL)
        {
            model = static_cast<Magic3D::Model*>(object);
        }
    }
    return model;
}
