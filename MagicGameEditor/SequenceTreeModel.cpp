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

#include <magic3d/magic3d.h>
#include <QtGui>
#include "SequenceTreeItem.h"
#include "SequenceTreeModel.h"

SequenceTreeModel::SequenceTreeModel(const QString &data, QTreeView *tree, QObject *parent)
    : QAbstractItemModel(parent)
{
    if (data.isEmpty())
    {

    }
    this->tree = tree;
    QList<QVariant> rootData;
    rootData << "Sequence";
    rootData << "x";
    rootData << "y";
    rootData << "width";
    rootData << "height";
    rootItem = new SequenceTreeItem(rootData, HEADER);
}

SequenceTreeModel::~SequenceTreeModel()
{
    delete rootItem;
}

int SequenceTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<SequenceTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant SequenceTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();

    if (index.isValid())
    {
        SequenceTreeItem *item = static_cast<SequenceTreeItem*>(index.internalPointer());

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

Qt::ItemFlags SequenceTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SequenceTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex SequenceTreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    SequenceTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SequenceTreeItem*>(parent.internalPointer());

    SequenceTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex SequenceTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    SequenceTreeItem *childItem = static_cast<SequenceTreeItem*>(index.internalPointer());
    SequenceTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SequenceTreeModel::rowCount(const QModelIndex &parent) const
{
    SequenceTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SequenceTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void SequenceTreeModel::setupModelData(const QStringList &lines, SequenceTreeItem *parent)
{
    QList<SequenceTreeItem*> parents;
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
            parents.last()->appendChild(new SequenceTreeItem(columnData, SEQUENCE, parents.last()));
        }

        number++;
    }
}

SequenceTreeItem* SequenceTreeModel::getRoot()
{
    return rootItem;
}

void SequenceTreeModel::removeAll()
{
    beginResetModel();
    if (rootItem)
    {
        rootItem->removeAllChilds();
    }
    endResetModel();
}
