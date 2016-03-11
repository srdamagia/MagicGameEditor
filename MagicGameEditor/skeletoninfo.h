/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2016 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2016
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

#ifndef SKELETONINFO_H
#define SKELETONINFO_H

#include "magic3dobjectpanel.h"
#include "AnimationTreeModel.h"

namespace Magic3D
{
    class Skeleton;
    class Bone;
    class Model;
}
namespace Ui {
    class SkeletonInfo;
}

class SkeletonInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit SkeletonInfo(MainWindow* mainWindow);
    ~SkeletonInfo();

    virtual void update();
    virtual void updateObject();
    virtual void updateSimulation();

    Magic3D::Model* getModel();

private:
    Ui::SkeletonInfo *ui;

    AnimationTreeModel* animationTreeModel;

private slots:
    void on_btnRemove_clicked();
    void on_treeAnimations_doubleClicked(QModelIndex index);
    void on_btnAdd_clicked();
    void on_sldAnimation_valueChanged(int value);
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
};

#endif // SKELETONINFO_H
