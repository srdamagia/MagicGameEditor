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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QItemSelectionModel>
#include <QToolButton>
#include <QComboBox>

#include "ui_mainwindow.h"
#include "MGE/mgeproject.h"
#include "MGE/magic3dwidget.h"
#include "SceneTreeModel.h"

#include "splashscreen.h"
#include "codeeditor.h"
#include "highlighter_lua.h"

#define LAYER_INDEX_2D 0
#define LAYER_INDEX_3D 1

#define TITLE "Magic Game Editor - Version "

namespace Ui {
    class MainWindow;
}

class CameraOptions;
class Object2DInfo;
class Object3DInfo;
class BoneInfo;
class PhysicsInfo;
class SkeletonInfo;
class TweensInfo;
class SceneInfo;
class MeshesInfo;
class ParticlesInfo;
class MaterialInfo;
class ShadersEditor;
class TerrainInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CameraOptions*  cameraoptions;
    Object2DInfo*   object2dinfo;
    Object3DInfo*   object3dinfo;
    BoneInfo*       boneinfo;
    PhysicsInfo*    physicsinfo;
    SkeletonInfo*   skeletoninfo;
    TweensInfo*     tweensinfo;
    SceneInfo*      sceneinfo;
    MeshesInfo*     meshesinfo;
    ParticlesInfo*  particlesinfo;
    MaterialInfo*   materialinfo;
    ShadersEditor*  shaderseditor;
    TerrainInfo*    terraininfo;

    QLabel cursor;
    QLabel info;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

    void update();
    void updateInfo();
    void updateMenus();
    void updateHelp();

    Ui::MainWindow* getUi();
    MGE::Magic3DWidget* getMagic3D();
    bool IsProjectChanged();
    bool IsProjectOpened();
    MGE::MGEProject* getProject();
    void setProjectChanged(bool changed);
    void setScriptChanged(bool changed);

    void selectObjectByName(QString& name, bool onlyObjects);
    void selectBoneByName(SceneTreeItem* item, QString& name);
    void duplicateObject(QString name, Magic3D::Object* object, bool clone);
    void spawnObject(Magic3D::Object* object);
    void updateScreen();

    void updateFromEngine();

    void setLoading(bool loading);

    CodeEditor* getEditorLua();

    void updateToolRender();
    void updateMaterialsList();
    QStringList& getMaterialList();
    void updateParentsList(bool is3D, bool tween, bool rigidbody);
    int getTweenCount(Magic3D::Object* object);
    QStringList& getParentsList();
    void setComboList(QStringList& list, QComboBox* combo);

    void setScriptObjectsText(QString text);

private:
    enum ORDER {
        eFRONT,
        eFORWARD,
        eBACKWARD,
        eBACK
    };

    Ui::MainWindow *ui;
    SplashScreen* splashScreen;
    SceneTreeModel* sceneTreeModel;
    MGE::MGEProject* project;
    MGE::Magic3DWidget* magic3dwidget;
    QLabel projectFile;
    QLabel scriptObjects;

    float screenWidth;
    float screenHeight;

    bool creating;
    bool loading;
    bool reloading;
    bool updating;

    bool resetLayout;

    QToolBar* toolBarScript;
    TextFind* findScript;
    CodeEditor* editorLua;
    HighlighterLua* highlighterLua;

    QMenu menuView;
    QMenu menuRenderMode;
    QToolButton* btnRenderMode;

    QString reloadProject;

    QStringList filesMaterial;
    QStringList parentsList;

    void init();

    SceneTreeItem* addLayer(QString name, bool main = false, bool engine = false);
    void addObject(int type, Magic3D::Object* object = NULL, bool clone = true);
    void addObject(QString name, int object, QPoint pos);
    bool orderItem(ORDER order);

    bool isLayerVisible(SceneTreeItem* item);
    void updateObjectsFromEngine(SceneTreeItem* layer, std::vector<Magic3D::Object*>* objects);
    void updateBonesFromEngine(SceneTreeItem* item, Magic3D::Object* object);
    void updateBonesFromSkeleton(SceneTreeItem* item, Magic3D::Bone* bone, Magic3D::Object* object);
    void updateChildsFromSpawn(Magic3D::Object* object);

    void setToolRender();

    void clearLog();
    void uncheckRenderMode();

private slots:
    void on_actionCloneObject_triggered();
    void on_actionFly_triggered();
    void on_actionRight_triggered();
    void on_actionLeft_triggered();
    void on_actionBack_triggered();
    void on_actionFront_triggered();
    void on_actionBottom_triggered();
    void on_actionTop_triggered();
    void on_actionShow_Mouse_Position_triggered();
    void on_actionShow_Information_triggered();
    void on_actionShow_FPS_triggered();
    void on_actionShow_Gizmos_triggered();
    void on_actionSimulate_triggered();
    void on_actionPerspective_triggered();
    void on_actionOrthographic_triggered();
    void on_actionDevice16_10_triggered();
    void on_actionDevice16_9_triggered();
    void on_actionDevice4_3_triggered();
    void on_actionFree_triggered();
    void on_actionLandscape_triggered();
    void on_actionPortrait_triggered();
    void on_actionWireframe_toggled(bool checked);
    void on_actionSolid_toggled(bool checked);
    void on_actionSendBackward_triggered();
    void on_actionBringForward_triggered();
    void on_actionSendToBack_triggered();
    void on_actionBringToFront_triggered();
    void on_actionRemoveObject_triggered();
    void on_actionParticles_triggered();
    void on_actionTerrain_triggered();
    void on_actionLight_triggered();
    void on_actionSound_triggered();
    void on_actionCamera_triggered();
    void on_actionAddModel_triggered();
    void on_actionAddSlider_triggered();
    void on_actionAddButton_triggered();
    void on_actionAddWindow_triggered();
    void on_actionAddText_triggered();
    void on_actionAddSprite_triggered();
    void on_actionRenameLayer_triggered();
    void on_actionRemoveLayer_triggered();
    void on_actionVisibleLayer_triggered();
    void on_actionAddLayer_triggered();
    void on_actionConfigure_Project_triggered();
    void on_actionClose_Project_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_As_triggered();
    void on_actionQuit_triggered();
    void on_treeScene_customContextMenuRequested(QPoint pos);
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void Editor_textScriptChanged();
    void on_treeHelp_expanded(const QModelIndex &index);
    void on_actionWater_triggered();
    void on_actionVegetation_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionJoystick_triggered();
    void on_treeScene_doubleClicked(const QModelIndex &index);
    void on_actionReset_Window_Layout_triggered();
    void on_dockMaterials_visibilityChanged(bool visible);
    void on_chkTRSX_toggled(bool checked);
    void on_chkTRSY_toggled(bool checked);
    void on_chkTRSZ_toggled(bool checked);
    void on_actionUpdate_Scene_Tree_View_triggered();
    void on_actionRenameObject_triggered();
    void on_actionShow_GizmosPhysics_triggered();
    void on_actionNormals_triggered();
    void on_actionUVs_triggered();
    void on_actionDepth_triggered();
    void on_actionScreenEffects_triggered();
    void on_actionAbout_triggered();
    void on_actionNewScript_triggered();
    void on_actionOpenScript_triggered();
    void on_actionPack_triggered();
    void on_actionInstance_triggered();
    void on_actionShow_Octree_triggered();
    void on_actionShow_Octree_Objects_triggered();
    void on_actionAddGuiText_triggered();
};

#endif // MAINWINDOW_H
