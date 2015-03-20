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

#include "SequenceTreeItem.h"

bool less(SequenceTreeItem *child1, SequenceTreeItem *child2)
{
    return child1->data(0).toString().toLower() < child2->data(0).toString().toLower();
}

SequenceTreeItem::SequenceTreeItem(const QList<QVariant> &data, SequenceTreeItemType type, SequenceTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    this->type = type;
}

SequenceTreeItem::~SequenceTreeItem()
{
    qDeleteAll(childItems);
}

void SequenceTreeItem::appendChild(SequenceTreeItem *item)
{
    childItems.append(item);
    if (getType() == HEADER)
    {
        qSort(childItems.begin(), childItems.end(), less);
    }
}

void SequenceTreeItem::removeChild(SequenceTreeItem *child)
{
    childItems.removeOne(child);
}

void SequenceTreeItem::removeAllChilds()
{
    qDeleteAll(childItems);
    childItems.clear();
}

SequenceTreeItem *SequenceTreeItem::child(int row)
{
    return childItems.value(row);
}

int SequenceTreeItem::childCount() const
{
    return childItems.count();
}

int SequenceTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant SequenceTreeItem::data(int column) const
{
    return itemData.value(column);
}

void SequenceTreeItem::setData(int column, QVariant value)
{
    return itemData.replace(column, value);
}

SequenceTreeItem *SequenceTreeItem::parent()
{
    return parentItem;
}

int SequenceTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<SequenceTreeItem*>(this));

    return 0;
}

SequenceTreeItemType SequenceTreeItem::getType()
{
    return type;
}
