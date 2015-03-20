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

#include <QtGui>

#include "SceneTreeItem.h"
#include "SceneTreeModel.h"
#include <magic3d/magic3d.h>

SceneTreeModel::SceneTreeModel(const QString &data, QTreeView *tree, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->tree = tree;
    QList<QVariant> rootData;
    rootData << "Layers";
    rootItem = new SceneTreeItem(rootData, HEADER);
    setupModelData(data.split(QString("\n")), rootItem);
}

SceneTreeModel::~SceneTreeModel()
{
    delete rootItem;
}

int SceneTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<SceneTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant SceneTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();

    if (index.isValid())
    {
        SceneTreeItem *item = static_cast<SceneTreeItem*>(index.internalPointer());

        switch (role)
        {
            case Qt::TextColorRole:
            {
                if (!item->isVisible() ||  (item->parent() && !item->parent()->isVisible()))
                {
                    result = QColor(128, 128, 128, 255);
                }
                else
                {
                    Magic3D::Object* object = item->getObject();

                    if (object && object->getParent())
                    {
                        result =  QColor(169, 169, 169, 255);
                    }
                    else
                    {
                        result =  QColor(225, 225, 225, 255);
                    }
                }

                break;
            }
            case Qt::DecorationRole:
            {
                bool icon = true;
                switch (item->getType())
                {
                    case ROOT: result = QIcon(":/MagicGameEditor/Images/world.png"); break;
                    case MAIN_LAYER:
                    case LAYER: result = QIcon(":/MagicGameEditor/Images/layer.png"); break;
                    case LAYER_2D: result = QIcon(":/MagicGameEditor/Images/image.png"); break;
                    case LAYER_3D: result = QIcon(":/MagicGameEditor/Images/models.png"); break;
                    case OBJECT:
                    {
                        Magic3D::Object* object = item->getObject();
                        if (object)
                        {
                            switch (object->getType())
                            {
                                case Magic3D::eOBJECT_CUSTOM:     result = QIcon(":/MagicGameEditor/Images/model.png"); break;
                                case Magic3D::eOBJECT_CAMERA:     result = QIcon(":/MagicGameEditor/Images/camera.png"); break;
                                case Magic3D::eOBJECT_SPRITE:     result = QIcon(":/MagicGameEditor/Images/sprite.png"); break;
                                case Magic3D::eOBJECT_MODEL:      result = QIcon(":/MagicGameEditor/Images/model.png"); break;
                                case Magic3D::eOBJECT_LIGHT:      result = QIcon(":/MagicGameEditor/Images/light.png"); break;
                                case Magic3D::eOBJECT_PARTICLES:  result = QIcon(":/MagicGameEditor/Images/particles.png"); break;
                                case Magic3D::eOBJECT_TERRAIN:    result = QIcon(":/MagicGameEditor/Images/terrain.png"); break;
                                case Magic3D::eOBJECT_WATER:      result = QIcon(":/MagicGameEditor/Images/water.png"); break;
                                case Magic3D::eOBJECT_VEGETATION: result = QIcon(":/MagicGameEditor/Images/tree.png"); break;
                                case Magic3D::eOBJECT_GUI_LABEL:  result = QIcon(":/MagicGameEditor/Images/text.png"); break;
                                case Magic3D::eOBJECT_GUI_WINDOW: result = QIcon(":/MagicGameEditor/Images/window.png"); break;
                                case Magic3D::eOBJECT_GUI_BUTTON: result = QIcon(":/MagicGameEditor/Images/button.png"); break;
                                case Magic3D::eOBJECT_GUI_SLIDER: result = QIcon(":/MagicGameEditor/Images/slider.png"); break;
                                case Magic3D::eOBJECT_SOUND:      result = QIcon(":/MagicGameEditor/Images/music.png"); break;
                                case Magic3D::eOBJECT_JOYSTICK:   result = QIcon(":/MagicGameEditor/Images/controller.png"); break;
                                case Magic3D::eOBJECT_INSTANCE:   result = QIcon(":/MagicGameEditor/Images/box.png"); break;
                            }
                        }
                        break;
                    }
                    case BONE: result = QIcon(":/MagicGameEditor/Images/bone.png"); break;
                    default: icon = false; break;
                }

                if (icon)
                {
                    if (!item->isVisible() || (item->parent() && !item->parent()->isVisible()))
                    {
                        QIcon* ico = (QIcon*)&result;
                        result = ico->pixmap(tree->iconSize().width(), tree->iconSize().height(), QIcon::Disabled);
                    }
                }
                break;
            }
            case Qt::DisplayRole:
            {
                Magic3D::Object* object = item->getObject();

                if (item->getType() != BONE && object && object->getParent())
                {
                    result = item->data(index.column()).toString() + " (" + QString::fromStdString(object->getParent()->getName()) + ")";
                }
                else
                {
                    result = item->data(index.column());
                }
                break;
            }
        }
    }

    return result;
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SceneTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    SceneTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SceneTreeItem*>(parent.internalPointer());

    SceneTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex SceneTreeModel::index(SceneTreeItem* item) const
{
    QModelIndex result = QModelIndex();
    if (item)
    {
        return createIndex(item->row(), 0, item);
    }

    return result;
}

QModelIndex SceneTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    SceneTreeItem *childItem = static_cast<SceneTreeItem*>(index.internalPointer());
    SceneTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SceneTreeModel::rowCount(const QModelIndex &parent) const
{
    SceneTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SceneTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void SceneTreeModel::setupModelData(const QStringList &lines, SceneTreeItem *parent)
{
    QList<SceneTreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new SceneTreeItem(columnData, ROOT, parents.last()));
        }

        number++;
    }
}

SceneTreeItem* SceneTreeModel::getRoot()
{
    return rootItem->child(0);
}

QModelIndex SceneTreeModel::findItem(QString& name, bool onlyObjects)
{
    QModelIndex result = QModelIndex();

    SceneTreeItem* item = rootItem->findChild(name, onlyObjects);
    if (item)
    {
        if (item->parent())
        {
            result = index(item);
        }
    }
    return result;
}
