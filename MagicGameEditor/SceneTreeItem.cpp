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

#include <QStringList>

#include "SceneTreeItem.h"
#include "utils.h"

bool lessMainLayer(SceneTreeItem *child1, SceneTreeItem *child2)
{
    if (child1->data(0).toString().toLower() == MAIN_LAYER_NAME.toLower())
    {
        return true;
    }
    else if (child2->data(0).toString().toLower() == MAIN_LAYER_NAME.toLower())
    {
        return false;
    }
    else
    {
        return child1->data(0).toString().toLower() < child2->data(0).toString().toLower();
    }
}

SceneTreeItem::SceneTreeItem(const QList<QVariant> &data, SceneTreeItemType type, SceneTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    this->type = type;

    visible = true;
    object = NULL;
    bone = NULL;
}

SceneTreeItem::~SceneTreeItem()
{
    qDeleteAll(childItems);
}

void SceneTreeItem::appendChild(SceneTreeItem *item)
{
    childItems.append(item);
    order();
}

void SceneTreeItem::removeChild(SceneTreeItem *child)
{
    childItems.removeOne(child);
}

void SceneTreeItem::clear()
{
    childItems.clear();
}

SceneTreeItem *SceneTreeItem::child(int row)
{
    return childItems.value(row);
}

int SceneTreeItem::childCount() const
{
    return childItems.count();
}

int SceneTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant SceneTreeItem::data(int column) const
{
    return itemData.value(column);
}

void SceneTreeItem::setData(int column, QVariant value)
{
    itemData.replace(column, value);
    if (parent())
    {
        parent()->order();
    }
}
void SceneTreeItem::order()
{
    qSort(childItems.begin(), childItems.end(), lessMainLayer);
}

SceneTreeItem *SceneTreeItem::parent()
{
    return parentItem;
}

int SceneTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<SceneTreeItem*>(this));

    return 0;
}

void SceneTreeItem::setVisible(bool visible)
{
    this->visible = visible;
}

bool SceneTreeItem::isVisible()
{
    return visible;
}

SceneTreeItemType SceneTreeItem::getType()
{
    return type;
}

void SceneTreeItem::setObject(Magic3D::Object* object)
{
    this->object = object;
}

Magic3D::Object* SceneTreeItem::getObject()
{
    return object;
}

void SceneTreeItem::setBone(Magic3D::Bone* bone)
{
    this->bone = bone;
}

Magic3D::Bone* SceneTreeItem::getBone()
{
    return bone;
}

SceneTreeItem* SceneTreeItem::findChild(QString& child, bool onlyObjects)
{
    SceneTreeItem* result = NULL;
    for (int i = 0; i < childItems.count(); i++)
    {
        if (childItems[i]->data(0).toString().compare(child) == 0 && (!onlyObjects || childItems[i]->getObject()))
        {
            result = childItems[i];
            break;
        }
        else
        {
            result = childItems[i]->findChild(child, onlyObjects);
            if (result)
            {
                break;
            }
        }
    }

    return result;
}
