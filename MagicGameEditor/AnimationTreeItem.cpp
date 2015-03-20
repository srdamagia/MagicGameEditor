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

#include "AnimationTreeItem.h"

bool less(AnimationTreeItem *child1, AnimationTreeItem *child2)
{
    return child1->data(0).toString().toLower() < child2->data(0).toString().toLower();
}

AnimationTreeItem::AnimationTreeItem(const QList<QVariant> &data, AnimationTreeItemType type, AnimationTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    this->type = type;
}

AnimationTreeItem::~AnimationTreeItem()
{
    qDeleteAll(childItems);
}

void AnimationTreeItem::appendChild(AnimationTreeItem *item)
{
    childItems.append(item);
    if (getType() == HEADER)
    {
        qSort(childItems.begin(), childItems.end(), less);
    }
}

void AnimationTreeItem::removeChild(AnimationTreeItem *child)
{
    childItems.removeOne(child);
}

void AnimationTreeItem::removeAllChilds()
{
    qDeleteAll(childItems);
    childItems.clear();
}

AnimationTreeItem *AnimationTreeItem::child(int row)
{
    return childItems.value(row);
}

int AnimationTreeItem::childCount() const
{
    return childItems.count();
}

int AnimationTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant AnimationTreeItem::data(int column) const
{
    return itemData.value(column);
}

void AnimationTreeItem::setData(int column, QVariant value)
{
    return itemData.replace(column, value);
}

AnimationTreeItem *AnimationTreeItem::parent()
{
    return parentItem;
}

int AnimationTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<AnimationTreeItem*>(this));

    return 0;
}

AnimationTreeItemType AnimationTreeItem::getType()
{
    return type;
}
