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

#ifndef SCENETREEITEM_H
#define SCENETREEITEM_H

#include <QList>
#include <QVariant>

namespace Magic3D
{
    class Object;
    class Bone;
}

enum SceneTreeItemType
{
    HEADER = 0,
    ROOT,
    MAIN_LAYER,
    LAYER,
    LAYER_2D,
    LAYER_3D,
    OBJECT,
    BONE
};

class SceneTreeItem
{
public:
    SceneTreeItem(const QList<QVariant> &data, SceneTreeItemType type, SceneTreeItem *parent = 0);
    ~SceneTreeItem();

    void appendChild(SceneTreeItem *child);
    void removeChild(SceneTreeItem *child);
    void clear();

    SceneTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, QVariant value);
    int row() const;
    void order();
    SceneTreeItem *parent();

    void setVisible(bool visible);
    bool isVisible();

    SceneTreeItemType getType();

    void setObject(Magic3D::Object* object);
    Magic3D::Object* getObject();
    void setBone(Magic3D::Bone* bone);
    Magic3D::Bone* getBone();

    SceneTreeItem* findChild(QString& child, bool onlyObjects);

private:
    QList<SceneTreeItem*> childItems;
    QList<QVariant> itemData;
    SceneTreeItem *parentItem;

    SceneTreeItemType type;
    bool visible;

    Magic3D::Object* object;
    Magic3D::Bone* bone;
};

#endif // SCENETREEITEM_H
