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

#include "AnimationTreeItem.h"
#include "AnimationTreeModel.h"
#include <magic3d/magic3d.h>

AnimationTreeModel::AnimationTreeModel(const QString &data, QTreeView *tree, QObject *parent)
    : QAbstractItemModel(parent)
{
    if (data.isEmpty())
    {

    }
    this->tree = tree;
    QList<QVariant> rootData;
    rootData << "Animation";
    rootData << "Start";
    rootData << "End";
    rootData << "FPS";
    rootData << "Loop";
    rootItem = new AnimationTreeItem(rootData, HEADER);
}

AnimationTreeModel::~AnimationTreeModel()
{
    delete rootItem;
}

int AnimationTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<AnimationTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant AnimationTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();

    if (index.isValid())
    {
        AnimationTreeItem *item = static_cast<AnimationTreeItem*>(index.internalPointer());

        switch (role)
        {
            case Qt::TextColorRole:
            {
                result = QColor(169, 169, 169,255);
                break;
            }
            case Qt::DecorationRole:
            {
                break;
            }
            case Qt::DisplayRole:
            {
                result = item->data(index.column());
                break;
            }
        }
    }

    return result;
}

Qt::ItemFlags AnimationTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant AnimationTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex AnimationTreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    AnimationTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AnimationTreeItem*>(parent.internalPointer());

    AnimationTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex AnimationTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    AnimationTreeItem *childItem = static_cast<AnimationTreeItem*>(index.internalPointer());
    AnimationTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int AnimationTreeModel::rowCount(const QModelIndex &parent) const
{
    AnimationTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AnimationTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void AnimationTreeModel::setupModelData(const QStringList &lines, AnimationTreeItem *parent)
{
    QList<AnimationTreeItem*> parents;
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
            parents.last()->appendChild(new AnimationTreeItem(columnData, ANIMATION, parents.last()));
        }

        number++;
    }
}

AnimationTreeItem* AnimationTreeModel::getRoot()
{
    return rootItem;
}

void AnimationTreeModel::removeAll()
{
    beginResetModel();
    if (rootItem)
    {
        rootItem->removeAllChilds();
    }
    endResetModel();
}
