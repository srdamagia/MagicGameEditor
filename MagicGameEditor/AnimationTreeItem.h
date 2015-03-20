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

#ifndef ANIMATIONTREEITEM_H
#define ANIMATIONTREEITEM_H

#include <QList>
#include <QVariant>

#define MAIN_LAYER_NAME QObject::tr("Main Layer")
namespace Magic3D
{
    class Object;
}

enum AnimationTreeItemType
{
    HEADER = 0,
    ANIMATION,
    FRAME
};

class AnimationTreeItem
{
public:
    AnimationTreeItem(const QList<QVariant> &data, AnimationTreeItemType type, AnimationTreeItem *parent = 0);
    ~AnimationTreeItem();

    void appendChild(AnimationTreeItem *child);
    void removeChild(AnimationTreeItem *child);
    void removeAllChilds();

    AnimationTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, QVariant value);
    int row() const;
    AnimationTreeItem *parent();

    AnimationTreeItemType getType();
private:
    QList<AnimationTreeItem*> childItems;
    QList<QVariant> itemData;
    AnimationTreeItem *parentItem;

    AnimationTreeItemType type;
};

#endif // ANIMATIONTREEITEM_H
