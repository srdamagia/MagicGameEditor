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

#include <QFileDialog>
#include <QInputDialog>
#include <QToolButton>
#include <QScrollBar>
#include <QSettings>
#include <QProcess>
#include "utils.h"
#include "SceneTreeItem.h"
#include "SceneTreeModel.h"

#include "mainwindow.h"
#include "newprojectwindow.h"
#include "cameraoptions.h"
#include "object2dinfo.h"
#include "object3dinfo.h"
#include "boneinfo.h"
#include "physicsinfo.h"
#include "skeletoninfo.h"
#include "tweensinfo.h"
#include "sceneinfo.h"
#include "meshesinfo.h"
#include "particlesinfo.h"
#include "shaderseditor.h"

#include <magic3d/magic3d.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    creating = true;
    loading = false;
    reloading = false;
    updating = false;

    resetLayout = false;

    splashScreen = new SplashScreen(this);
    ui->setupUi(this);

    setWindowTitle(TITLE + tr(MAGIC3D_VERSION));
    setWindowIcon(QIcon(":/MagicGameEditor/Icons/magic3d.ico"));

    init();

    QSettings settings;

    if (!settings.value("reset").toBool())
    {
        restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    }
    else
    {
        creating = false;
    }

    int window = settings.value("windowType").toInt();

    switch (window)
    {
        case 0: ui->actionFree->trigger(); break;
        case 1: ui->actionDevice4_3->trigger(); break;
        case 2: ui->actionDevice16_9->trigger(); break;
        case 3: ui->actionDevice16_10->trigger(); break;
    }
}

MainWindow::~MainWindow()
{
    if (magic3dwidget)
    {
        delete magic3dwidget;
        magic3dwidget = NULL;
    }

    if (sceneTreeModel)
    {
        ui->treeScene->setModel(NULL);
        delete sceneTreeModel;
        sceneTreeModel = NULL;
    }

    if (cameraoptions)
    {
        delete cameraoptions;
        cameraoptions = NULL;
    }

    if (object2dinfo)
    {
        delete object2dinfo;
        object2dinfo = NULL;
    }

    if (object3dinfo)
    {
        delete object3dinfo;
        object3dinfo = NULL;
    }

    if (boneinfo)
    {
        delete boneinfo;
        boneinfo = NULL;
    }

    if (physicsinfo)
    {
        delete physicsinfo;
        physicsinfo = NULL;
    }

    if (skeletoninfo)
    {
        delete skeletoninfo;
        skeletoninfo = NULL;
    }

    if (tweensinfo)
    {
        delete tweensinfo;
        tweensinfo = NULL;
    }

    if (sceneinfo)
    {
        delete sceneinfo;
        sceneinfo = NULL;
    }

    if (meshesinfo)
    {
        delete meshesinfo;
        meshesinfo = NULL;
    }

    if (particlesinfo)
    {
        delete particlesinfo;
        particlesinfo = NULL;
    }

    if (materialinfo)
    {
        delete materialinfo;
        materialinfo = NULL;
    }

    if (highlighterLua)
    {
        delete highlighterLua;
        highlighterLua = NULL;
    }

    if (editorLua)
    {
        delete editorLua;
        editorLua = NULL;
    }

    if (findScript)
    {
        delete findScript;
        findScript = NULL;
    }

    if (shaderseditor)
    {
        delete shaderseditor;
        shaderseditor = NULL;
    }

    if (toolBarScript)
    {
        delete toolBarScript;
        toolBarScript = NULL;
    }

    if (splashScreen)
    {
        delete splashScreen;
        splashScreen = NULL;
    }

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (creating)
    {
        QSettings settings;
        QSize size = settings.value("windowSize").toSize();

        if (event && event->size() == size)
        {
            creating = false;
            restoreState(settings.value("mainWindowState").toByteArray());
            ui->splitterScript->restoreState(settings.value("splitterScript").toByteArray());
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->actionClose_Project->trigger();
    if (!project)
    {
        QSettings settings;
        settings.setValue("mainWindowGeometry", saveGeometry());
        settings.setValue("mainWindowState", saveState());

        int window = 0;
        if (ui->actionFree->isChecked())
            window = 0;
        else if (ui->actionDevice4_3->isChecked())
            window = 1;
        else if (ui->actionDevice16_9->isChecked())
            window = 2;
        else if (ui->actionDevice16_10->isChecked())
            window = 3;

        settings.setValue("windowType", window);
        settings.setValue("windowSize", size());
        settings.setValue("splitterScript", ui->splitterScript->saveState());

        settings.setValue("showGizmos", magic3dwidget->isShowingGizmos());
        settings.setValue("showGizmosPhysics", magic3dwidget->isShowingGizmosPhysics());
        settings.setValue("showOctree", magic3dwidget->isShowingOctree());
        settings.setValue("showOctreeObjects", magic3dwidget->isShowingOctreeObjects());
        settings.setValue("showFPS", Magic3D::Scene::getInstance()->isShowingFPSText());
        settings.setValue("showINFO", Magic3D::Scene::getInstance()->isShowingINFOText());
        settings.setValue("showWINDOW", Magic3D::Scene::getInstance()->isShowingWINDOWText());

        settings.setValue("reset", resetLayout);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

Ui::MainWindow* MainWindow::getUi()
{
    return ui;
}

MGE::Magic3DWidget* MainWindow::getMagic3D()
{
    return magic3dwidget;
}

void MainWindow::setLoading(bool loading)
{
    this->loading = loading;
    if (!loading)
    {
        reloading = false;
    }
}

CodeEditor* MainWindow::getEditorLua()
{
    return editorLua;
}

void MainWindow::uncheckRenderMode()
{
    ui->actionSolid->setChecked(false);
    ui->actionWireframe->setChecked(false);
    ui->actionNormals->setChecked(false);
    ui->actionUVs->setChecked(false);
    ui->actionDepth->setChecked(false);
    ui->actionScreenEffects->setChecked(false);
}

void MainWindow::init()
{
    project = NULL;
    sceneTreeModel = NULL;

    screenWidth = QWIDGETSIZE_MAX;
    screenHeight = QWIDGETSIZE_MAX;

    tabifyDockWidget(ui->dockProperties, ui->dockTerrain);
    tabifyDockWidget(ui->dockProperties, ui->dockParticles);
    tabifyDockWidget(ui->dockProperties, ui->dockMaterials);
    tabifyDockWidget(ui->dockProperties, ui->dockShaders);
    ui->dockProperties->raise();

    ui->dockTerrain->setVisible(false);

    tabifyDockWidget(ui->dockScript, ui->dockLog);
    ui->dockScript->raise();

    projectFile.setText("");
    statusBar()->addWidget(&projectFile);
    scriptObjects.setText("");
    statusBar()->addWidget(&scriptObjects);
    cursor.setText("x: 0, y: 0 / width: 0, height: 0");
    statusBar()->addPermanentWidget(&cursor);

    //ui->viewLayout->addWidget(&info);

    //magic3dwidget = NULL;
    QGLFormat glFormat = QGLFormat::defaultFormat();
    glFormat.setSwapInterval(0); // vsync
    QGLFormat::setDefaultFormat(glFormat);

    magic3dwidget = new MGE::Magic3DWidget(glFormat, this);
    ui->viewLayout->addWidget(magic3dwidget, 0, 1);

    object2dinfo = new Object2DInfo(this);
    ui->layoutObject->addWidget(object2dinfo);
    object2dinfo->setVisible(false);

    object3dinfo = new Object3DInfo(this);
    ui->layoutObject->addWidget(object3dinfo);
    object3dinfo->setVisible(false);

    boneinfo = new BoneInfo(this);
    ui->layoutObject->addWidget(boneinfo);
    boneinfo->setVisible(false);

    physicsinfo = new PhysicsInfo(this);
    ui->layoutPhysics->addWidget(physicsinfo);
    physicsinfo->setVisible(false);

    skeletoninfo = new SkeletonInfo(this);
    ui->layoutSkeleton->addWidget(skeletoninfo);
    skeletoninfo->setVisible(false);

    tweensinfo = new TweensInfo(this);
    ui->layoutTween->addWidget(tweensinfo);
    tweensinfo->setVisible(false);

    sceneinfo = new SceneInfo(this);
    ui->layoutObject->addWidget(sceneinfo);
    sceneinfo->setVisible(false);

    meshesinfo   = new MeshesInfo(this);
    ui->layoutMesh->addWidget(meshesinfo);
    meshesinfo->setVisible(false);

    particlesinfo   = new ParticlesInfo(this);
    ui->layoutParticles->addWidget(particlesinfo);
    particlesinfo->setVisible(false);

    materialinfo = new MaterialInfo(this);
    ui->layoutMaterials->addWidget(materialinfo);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editorLua = new CodeEditor(this);
    editorLua->setFont(font);
    editorLua->setTabStopWidth(editorLua->fontMetrics().width("    "));

    findScript = new TextFind(this);
    findScript->setFindEditor(editorLua);

    editorLua->setTextFind(findScript);

    toolBarScript = new QToolBar(ui->dockScript);
    toolBarScript->addAction(ui->actionNewScript);
    toolBarScript->addAction(ui->actionOpenScript);
    toolBarScript->addSeparator();
    toolBarScript->addWidget(findScript);

    ui->layoutScript->addWidget(toolBarScript);
    ui->layoutScript->addWidget(editorLua);

    connect(editorLua, SIGNAL(textChanged()), this, SLOT(Editor_textScriptChanged()));

    highlighterLua = new HighlighterLua(editorLua->document());

    //shaderseditor = NULL;
    shaderseditor = new ShadersEditor(this);
    ui->layoutShaders->addWidget(shaderseditor);

    menuRenderMode.addAction(ui->actionSolid);
    menuRenderMode.addAction(ui->actionWireframe);
    menuRenderMode.addAction(ui->actionNormals);
    menuRenderMode.addAction(ui->actionUVs);
    menuRenderMode.addAction(ui->actionDepth);
    menuRenderMode.addAction(ui->actionScreenEffects);

    btnRenderMode = new QToolButton();
    btnRenderMode->setIcon(ui->actionSolid->icon());
    btnRenderMode->setMenu(&menuRenderMode);
    btnRenderMode->setPopupMode(QToolButton::InstantPopup);
    ui->view->addWidget(btnRenderMode);

    menuView.addAction(ui->actionTop);
    menuView.addAction(ui->actionBottom);
    menuView.addAction(ui->actionFront);
    menuView.addAction(ui->actionBack);
    menuView.addAction(ui->actionLeft);
    menuView.addAction(ui->actionRight);
    menuView.addSeparator();
    menuView.addAction(ui->actionFly);
    ui->actionFly->setChecked(true);

    QToolButton* btnView = new QToolButton();
    btnView->setIcon(QIcon(":/MagicGameEditor/Images/camera.png"));
    btnView->setMenu(&menuView);
    btnView->setPopupMode(QToolButton::InstantPopup);
    ui->camera->addWidget(btnView);
    ui->camera->addSeparator();

    cameraoptions = new CameraOptions(this);
    ui->camera->addWidget(cameraoptions);

    ui->treeHelp->header()->setStretchLastSection(false);
    ui->treeHelp->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    update();
}

void MainWindow::update()
{
    if (updating)
    {
        return;
    }

    updating = true;
    bool enabled = project;
    bool save = IsProjectChanged();
    bool loading = Magic3D::Scene::getInstance() && Magic3D::Scene::getInstance()->isLoading();

    enabled = enabled && !loading;

    ui->mainToolBar->setEnabled(!loading);
    ui->menuBar->setEnabled(!loading);

    if (magic3dwidget)
    {
        magic3dwidget->setEnabled(enabled);
    }

    ui->actionSave->setEnabled(save);
    ui->actionSave_As->setEnabled(false);
    ui->actionSave_As->setVisible(false);
    ui->actionClose_Project->setEnabled(enabled);
    ui->actionConfigure_Project->setEnabled(enabled);
    ui->actionPack->setEnabled(enabled);

    ui->dockScene->setEnabled(enabled);
    ui->dockProperties->setEnabled(enabled);
    ui->dockTerrain->setEnabled(enabled);
    ui->dockParticles->setEnabled(enabled);
    ui->dockMaterials->setEnabled(enabled);
    ui->dockShaders->setEnabled(enabled);

    if (enabled)
    {
        if (!sceneTreeModel)
        {
            sceneTreeModel = new SceneTreeModel(project->getName().c_str(), ui->treeScene, this);
            ui->treeScene->setModel(sceneTreeModel);

            QItemSelectionModel *selectionModel= ui->treeScene->selectionModel();
            connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
                    this,           SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

            /*QModelIndex index = sceneTreeModel->index(sceneTreeModel->getRoot()->row(), 0);
            ui->treeScene->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);*/
        }

        if (project)
        {
            projectFile.setText(QString::fromStdString(project->getFullPath()));
        }
    }
    else
    {
        ui->dockScene->setWindowTitle(tr("Scene"));
        projectFile.setText(UTILS_DEFAULT_PATH);
    }

    ui->menuEdit->menuAction()->setVisible(enabled);
    ui->menuScene->menuAction()->setVisible(enabled);
    ui->menuView->menuAction()->setVisible(enabled);
    ui->menuWindow->menuAction()->setVisible(enabled);

    ui->view->setEnabled(enabled);
    ui->order->setEnabled(enabled);
    ui->physics->setEnabled(enabled);

    ui->camera->setEnabled(enabled);
    ui->actionOrthographic->setChecked(enabled && Magic3D::Renderer::getInstance()->getCurrentViewPort()->getOrthographic());
    ui->actionPerspective->setChecked(enabled && Magic3D::Renderer::getInstance()->getCurrentViewPort()->getPerspective());
    ui->actionSimulate->setChecked(enabled && Magic3D::Physics::getInstance()->isPlaying());

    bool physics = enabled && Magic3D::Physics::getInstance()->isPlaying();

    ui->actionShow_Gizmos->setChecked(enabled && magic3dwidget->isShowingGizmos());
    ui->actionShow_GizmosPhysics->setChecked(enabled && magic3dwidget->isShowingGizmosPhysics());
    ui->actionShow_Octree->setChecked(enabled && magic3dwidget->isShowingOctree());
    ui->actionShow_Octree_Objects->setChecked(enabled && magic3dwidget->isShowingOctreeObjects());
    ui->actionShow_FPS->setChecked(enabled && Magic3D::Scene::getInstance()->isShowingFPSText());
    ui->actionShow_Information->setChecked(enabled && Magic3D::Scene::getInstance()->isShowingINFOText());
    ui->actionShow_Mouse_Position->setChecked(enabled && Magic3D::Scene::getInstance()->isShowingWINDOWText());

    updateMenus();
    updateScreen();

    updating = false;
}

void MainWindow::updateHelp()
{
    bool playing = false;
    if (Magic3D::Script::getInstance()->isPlaying())
    {
        playing = true;
    }
    else
    {
        Magic3D::Script::getInstance()->play(false);
    }
    std::vector<Magic3D::ScriptHelpItems> list;
    Magic3D::Script::getInstance()->help(&list);

    //Magic3D::Script::getInstance()->dumpStack();

    ui->treeHelp->clear();

    int size = list.size();
    for (int h = 0; h < size; h++)
    {
        QTreeWidgetItem *help = new QTreeWidgetItem(ui->treeHelp);
        help->setText(0, (QString::fromStdString(list[h].name)));

        std::vector<Magic3D::ScriptHelpItem>::iterator it = list[h].items.begin();

        while (it != list[h].items.end())
        {
            Magic3D::ScriptHelpItem item = *it++;
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(help);
            if (item.prototype.compare("") == 0)
            {
                treeItem->setText(0, QString::fromStdString(item.func));
            }
            else
            {
                treeItem->setText(0, QString::fromStdString(item.prototype));
            }
            if (item.help.compare("") != 0)
            {
                treeItem->setToolTip(0, QString::fromStdString(item.help));
            }
        }
    }

    //ui->treeHelp->sortItems(0, Qt::AscendingOrder);
    if (!playing)
    {
        Magic3D::Script::getInstance()->stop(false);
    }
}

void MainWindow::updateInfo()
{
    if (!loading && project)
    {
        setProjectChanged(true);
    }
    object2dinfo->updateParentCombo();
    object3dinfo->updateParentCombo();
    sceneinfo->updateCamerasCombo();
    magic3dwidget->updateInfo();
    update();
}

void MainWindow::updateMenus()
{
    if (magic3dwidget)
    {
        ui->actionCopy->setEnabled(magic3dwidget->getSelectedObject());
        ui->actionPaste->setEnabled(magic3dwidget->getCopiedObject());
    }
}

bool MainWindow::IsProjectChanged()
{
    return project && (project->isChanged() || project->isChangedScript());
}

bool MainWindow::IsProjectOpened()
{
    return project;
}

MGE::MGEProject* MainWindow::getProject()
{
    return project;
}

void MainWindow::setProjectChanged(bool changed)
{
    if (project && !Magic3D::Script::getInstance()->isPlaying() && !Magic3D::Physics::getInstance()->isPlaying())
    {
        project->setChanged(changed);
    }
}

void MainWindow::setScriptChanged(bool changed)
{
    if (project && !Magic3D::Script::getInstance()->isPlaying() && !Magic3D::Physics::getInstance()->isPlaying())
    {
        project->setChangedScript(changed);
    }
}

void MainWindow::selectObjectByName(QString& name, bool onlyObjects)
{
    if (sceneTreeModel)
    {
        ui->treeScene->setCurrentIndex(sceneTreeModel->findItem(name, onlyObjects));
    }
}

void MainWindow::selectBoneByName(SceneTreeItem* item, QString& name)
{
    if (sceneTreeModel && item)
    {
        QModelIndex result = QModelIndex();
        SceneTreeItem* root = item->findChild(name, false);
        if (root)
        {
            if (root->parent())
            {
                result = sceneTreeModel->index(root);
            }
        }

        ui->treeScene->setCurrentIndex(result);
    }
}

SceneTreeItem* MainWindow::addLayer(QString name, bool main, bool engine)
{
    SceneTreeItem* result = NULL;

    if (!engine && ((name.compare(MAIN_LAYER_NAME) == 0 && !main) || magic3dwidget->getLayer(name.toStdString())))
    {
        Utils::message(tr("Invalid Name"), tr("Layer named \"") + name + tr("\" already exists!"));
    }
    else if (main || engine || magic3dwidget->addLayer(name.toStdString()))
    {
        SceneTreeItem* root = sceneTreeModel->getRoot();
        QList<QVariant> itemData;
        itemData << name;

        result = new SceneTreeItem(itemData, main ? MAIN_LAYER : LAYER, root);
        root->appendChild(result);

        itemData.clear();
        itemData << "2D";

        result->appendChild(new SceneTreeItem(itemData, LAYER_2D, result));

        itemData.clear();
        itemData << "3D";

        result->appendChild(new SceneTreeItem(itemData, LAYER_3D, result));

        if (!engine)
        {
            QModelIndex rootIndex = sceneTreeModel->index(root->row(), 0);

            ui->treeScene->collapse(rootIndex);
            ui->treeScene->expand(rootIndex);

            ui->treeScene->selectionModel()->setCurrentIndex(sceneTreeModel->index(result->row(), 0, rootIndex), QItemSelectionModel::ClearAndSelect);
        }

        updateInfo();
    }

    return result;
}

void MainWindow::addObject(int type, Magic3D::Object* object, bool clone)
{
    QString title = "";

    int objType = type;

    if (object)
    {
        objType = object->getType();
    }

    switch (objType)
    {
        case Magic3D::eOBJECT_CUSTOM:     title = "Custom"; break;
        case Magic3D::eOBJECT_CAMERA:     title = "Camera"; break;
        case Magic3D::eOBJECT_SPRITE:     title = "Sprite"; break;
        case Magic3D::eOBJECT_MODEL:      title = "Model"; break;
        case Magic3D::eOBJECT_LIGHT:      title = "Light"; break;
        case Magic3D::eOBJECT_PARTICLES:  title = "Particles"; break;
        case Magic3D::eOBJECT_TERRAIN:    title = "Terrain"; break;
        case Magic3D::eOBJECT_WATER:      title = "Water"; break;
        case Magic3D::eOBJECT_VEGETATION: title = "Vegetation"; break;
        case Magic3D::eOBJECT_GUI_LABEL:  title = "Text"; break;
        case Magic3D::eOBJECT_GUI_WINDOW: title = "GUI Window"; break;
        case Magic3D::eOBJECT_GUI_BUTTON: title = "GUI Button"; break;
        case Magic3D::eOBJECT_GUI_SLIDER: title = "GUI Slider"; break;
        case Magic3D::eOBJECT_SOUND:      title = "Sound"; break;
        case Magic3D::eOBJECT_JOYSTICK:   title = "Joystick"; break;
        case Magic3D::eOBJECT_INSTANCE:   title = "Instance"; break;
    }

    bool ok;
    std::string newName = Magic3D::ResourceManager::getObjects()->getNewName(object ? object->getName().c_str() : "");
    QString text = QInputDialog::getText(this, tr("New ") + title, title + tr(" name:"), QLineEdit::Normal, QString::fromStdString(newName), &ok);
    if (ok && !text.isEmpty())
    {
        if (object)
        {
            duplicateObject(text, object, clone);
        }
        else
        {
            addObject(text, type, magic3dwidget->getLastMenuPos());
        }
    }
    else if (ok)
    {
        Utils::message(tr("Invalid Name"), tr("\"") + text + tr("\" is an invalid name!"));
    }
}

void MainWindow::addObject(QString name, int object, QPoint pos)
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        switch (item->getType())
        {
            case ROOT:
            {
                //item = item->child(0);
                item = static_cast<SceneTreeItem*>(sceneTreeModel->findItem(magic3dwidget->getLastLayer(), false).internalPointer());
                break;
            }
            case LAYER_2D:
            case LAYER_3D:
            {
                item = item->parent();
                break;
            }
            case OBJECT:
            {
                item = item->parent()->parent();
                break;
            }
            default: break;
        }

        QList<QVariant> itemData;
        itemData << name;

        SceneTreeItem* child = item;
        SceneTreeItem* obj = NULL;

        QString layerName = item->data(0).toString();

        if (item->getType() == MAIN_LAYER)
        {
            layerName = M3D_MAIN_LAYER_NAME;
        }

        switch (object)
        {
            case Magic3D::eOBJECT_CUSTOM: break;
            case Magic3D::eOBJECT_CAMERA:
            {
                if (magic3dwidget->addCamera(name.toStdString(), pos))
                {
                    item = sceneTreeModel->getRoot()->child(0);
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }

                break;
            }
            case Magic3D::eOBJECT_SPRITE:
            {
                if (magic3dwidget->addSprite(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_2D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }

                break;
            }
            case Magic3D::eOBJECT_MODEL:
            {
                if (magic3dwidget->addModel(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_INSTANCE:
            {
                if (magic3dwidget->addInstance(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_LIGHT:
            {
                if (magic3dwidget->addLight(name.toStdString(), pos))
                {
                    item = sceneTreeModel->getRoot()->child(0);
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }

                break;
            }
            case Magic3D::eOBJECT_PARTICLES:
            {
                if (magic3dwidget->addParticles(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_TERRAIN:
            {
                //if (magic3dwidget->addTerrain(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_WATER:
            {
                //if (magic3dwidget->addTerrain(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_VEGETATION:
            {
                //if (magic3dwidget->addTerrain(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_GUI_LABEL:
            {
                if (magic3dwidget->addText(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_2D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_GUI_WINDOW:
            {
                if (magic3dwidget->addWindow(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_2D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_GUI_BUTTON:
            {
                if (magic3dwidget->addButton(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_2D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_GUI_SLIDER:
            {
                if (magic3dwidget->addSlider(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_2D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
            case Magic3D::eOBJECT_SOUND:
            {
                if (magic3dwidget->addSound(name.toStdString(), pos))
                {
                    item = sceneTreeModel->getRoot()->child(0);
                    child = item->child(LAYER_INDEX_3D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }

                break;
            }
            case Magic3D::eOBJECT_JOYSTICK:
            {
                if (magic3dwidget->addJoystick(name.toStdString(), layerName.toStdString(), pos))
                {
                    child = item->child(LAYER_INDEX_2D);

                    obj = new SceneTreeItem(itemData, OBJECT, child);
                    child->appendChild(obj);
                }
                break;
            }
        }

        if (child && obj)
        {
            obj->setObject(magic3dwidget->getObject(name.toStdString()));
            QModelIndex root = sceneTreeModel->index(item->parent()->row(), 0);
            QModelIndex index = sceneTreeModel->index(item->row(), 0, root);
            QModelIndex childIndex = sceneTreeModel->index(child->row(), 0, index);
            ui->treeScene->collapse(root);
            ui->treeScene->expand(root);
            ui->treeScene->collapse(childIndex);
            ui->treeScene->collapse(index);
            ui->treeScene->expand(index);

            QModelIndex objIndex = sceneTreeModel->index(obj->row(), 0, childIndex);
            ui->treeScene->selectionModel()->setCurrentIndex(objIndex, QItemSelectionModel::ClearAndSelect);
            magic3dwidget->setSelection(obj->getObject(), NULL);
        }

        updateMenus();
        updateInfo();
    }
}

void MainWindow::duplicateObject(QString name, Magic3D::Object* object, bool clone)
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        switch (item->getType())
        {
            case ROOT:
            {
                //item = item->child(0);
                item = static_cast<SceneTreeItem*>(sceneTreeModel->findItem(magic3dwidget->getLastLayer(), false).internalPointer());
                break;
            }
            case LAYER_2D:
            case LAYER_3D:
            {
                item = item->parent();
                break;
            }
            case OBJECT:
            {
                item = item->parent()->parent();
                break;
            }
            default: break;
        }

        QList<QVariant> itemData;
        itemData << name;

        SceneTreeItem* child = item;
        SceneTreeItem* obj = NULL;

        QString layerName = item->data(0).toString();

        if (item->getType() == MAIN_LAYER)
        {
            layerName = M3D_MAIN_LAYER_NAME;
        }

        Magic3D::Object* duplicate = NULL;

        /*if (clone)
        {
            duplicate = magic3dwidget->cloneObject(object, name.toStdString(), layerName.toStdString());
        }
        else*/
        {
            duplicate = magic3dwidget->spawnObject(object, name.toStdString(), layerName.toStdString());
        }

        if (duplicate)
        {
            if (duplicate->getRender() == Magic3D::eRENDER_2D)
            {
                child = item->child(LAYER_INDEX_2D);
            }
            else
            {
                child = item->child(LAYER_INDEX_3D);
            }

            obj = new SceneTreeItem(itemData, OBJECT, child);
            child->appendChild(obj);

            updateBonesFromEngine(obj, duplicate);

            updateChildsFromSpawn(duplicate);
        }

        if (child && obj)
        {
            obj->setObject(magic3dwidget->getObject(name.toStdString()));
            QModelIndex root = sceneTreeModel->index(item->parent()->row(), 0);
            QModelIndex index = sceneTreeModel->index(item->row(), 0, root);
            QModelIndex childIndex = sceneTreeModel->index(child->row(), 0, index);
            ui->treeScene->collapse(root);
            ui->treeScene->expand(root);
            ui->treeScene->collapse(childIndex);
            ui->treeScene->collapse(index);
            ui->treeScene->expand(index);

            QModelIndex objIndex = sceneTreeModel->index(obj->row(), 0, childIndex);
            ui->treeScene->selectionModel()->setCurrentIndex(objIndex, QItemSelectionModel::ClearAndSelect);
        }

        updateInfo();
    }

}

void MainWindow::updateChildsFromSpawn(Magic3D::Object* object)
{
    const std::vector<Magic3D::Object*>* children = object->getChildren();
    std::vector<Magic3D::Object*>::const_iterator it_c = children->begin();
    while (it_c != children->end())
    {
        Magic3D::Object* c = *it_c++;
        if (c->getLayer())
        {
            QString childLayerName = QString::fromStdString(c->getLayer()->getName());
            if (childLayerName.compare(M3D_MAIN_LAYER_NAME) == 0)
            {
                childLayerName = MAIN_LAYER_NAME;
            }
            SceneTreeItem* itemChild = static_cast<SceneTreeItem*>(sceneTreeModel->findItem(childLayerName, false).internalPointer());
            if (c->getRender() == Magic3D::eRENDER_2D)
            {
                itemChild = itemChild->child(LAYER_INDEX_2D);
            }
            else
            {
                itemChild = itemChild->child(LAYER_INDEX_3D);
            }
            QList<QVariant> childName;
            childName << QString::fromStdString(c->getName());

            SceneTreeItem* cItem = new SceneTreeItem(childName, OBJECT, itemChild);
            cItem->setObject(c);
            itemChild->appendChild(cItem);

            updateBonesFromEngine(cItem, c);
        }
        updateChildsFromSpawn(c);
    }
}

void MainWindow::spawnObject(Magic3D::Object* object)
{
    if (object)
    {
        addObject(object->getType(), object, false);
    }
}

bool MainWindow::orderItem(ORDER order)
{
    bool result = false;
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        switch (item->getType())
        {
            case LAYER:
            {
                std::vector<Magic3D::Layer*>* layers = Magic3D::Scene::getInstance()->getLayers();
                int index = Magic3D::Scene::getInstance()->getLayerIndex(item->data(0).toString().toStdString());
                Magic3D::Layer* tmp = NULL;
                if (index > 0)
                {
                    tmp = layers->at(index);
                }
                switch (order)
                {
                    case eFRONT:
                    {
                        if (tmp && index > 0)
                        {
                            layers->erase(layers->begin() + index);
                            layers->push_back(tmp);
                            result = true;
                        }
                        break;
                    }
                    case eFORWARD:
                    {
                        int size = layers->size() - 1;
                        if (tmp && index > 0 && index < size)
                        {
                            layers->erase(layers->begin() + index);
                            layers->insert(layers->begin() + index + 1, tmp);
                            result = true;
                        }
                        break;
                    }
                    case eBACKWARD:
                    {
                        if (tmp && index > 1)
                        {
                            layers->erase(layers->begin() + index);
                            layers->insert(layers->begin() + index - 1, tmp);
                            result = true;
                        }
                        break;
                    }
                    case eBACK:
                    {
                        if (tmp && index > 1)
                        {
                            layers->erase(layers->begin() + index);
                            layers->insert(layers->begin() + 1, tmp);
                            result = true;
                        }
                        break;
                    }
                }

                break;
            }
            case OBJECT:
            {
                std::string name = item->parent()->parent()->data(0).toString().toStdString();
                if (name.compare(MAIN_LAYER_NAME.toStdString()) == 0)
                {
                    name = M3D_MAIN_LAYER_NAME;
                }
                Magic3D::Layer* layer = Magic3D::Scene::getInstance()->getLayer(name);
                name = item->data(0).toString().toStdString();
                Magic3D::Object* object = Magic3D::ResourceManager::getObjects()->get(name);
                if (object && layer)
                {
                    std::vector<Magic3D::Object*>* list = object->getRender() == Magic3D::eRENDER_2D ? layer->getObjects2D() : layer->getObjects3D();
                    int index = layer->getObjectIndex(name, list);
                    switch (order)
                    {
                        case eFRONT:
                        {
                            if (index >= 0)
                            {
                                list->erase(list->begin() + index);
                                list->push_back(object);
                                result = true;
                            }
                            break;
                        }
                        case eFORWARD:
                        {
                            int size = list->size() - 1;
                            if (index >= 0 && index < size)
                            {
                                list->erase(list->begin() + index);
                                list->insert(list->begin() + index + 1, object);
                                result = true;
                            }
                            break;
                        }
                        case eBACKWARD:
                        {
                            if (index > 0)
                            {
                                list->erase(list->begin() + index);
                                list->insert(list->begin() + index - 1, object);
                                result = true;
                            }
                            break;
                        }
                        case eBACK:
                        {
                            if (index > 0)
                            {
                                list->erase(list->begin() + index);
                                list->insert(list->begin(), object);
                                result = true;
                            }
                            break;
                        }
                    }
                }

                break;
            }
            default: break;
        }
    }

    if (result)
    {
        updateInfo();
    }

    return result;
}

bool MainWindow::isLayerVisible(SceneTreeItem* item)
{
    bool visible = true;

    if (item && item->getType() == LAYER)
    {
        QString layerName = item->data(0).toString();
        Magic3D::Layer* layer = magic3dwidget->getLayer(layerName.toStdString());
        if (layer && !layer->isVisible())
        {
            visible = false;
        }
    }

    return visible;
}

void MainWindow::updateObjectsFromEngine(SceneTreeItem* layer, std::vector<Magic3D::Object*>* objects)
{
    std::vector<Magic3D::Object*>::const_iterator it_o = objects->begin();
    while (it_o != objects->end())
    {
        Magic3D::Object* object = *it_o++;

        QList<QVariant> itemData;
        itemData << QString::fromStdString(object->getName());

        SceneTreeItem* child = layer->child(object->getRender() == Magic3D::eRENDER_2D ? LAYER_INDEX_2D : LAYER_INDEX_3D);
        SceneTreeItem* objItem = new SceneTreeItem(itemData, OBJECT, child);
        objItem->setObject(object);
        child->appendChild(objItem);

        updateBonesFromEngine(objItem, object);
    }
}

void MainWindow::updateBonesFromEngine(SceneTreeItem* item, Magic3D::Object* object)
{
    if (object && object->getType() == Magic3D::eOBJECT_MODEL)
    {
        Magic3D::Model* model = static_cast<Magic3D::Model*>(object);
        Magic3D::Skeleton* skeleton = model->getSkeleton();
        if (skeleton)
        {
            for (int i = 0; i < skeleton->getBoneCount(); i++)
            {
                Magic3D::Bone* bone = skeleton->getBone(i);
                if (bone && !bone->getParent())
                {
                    QList<QVariant> itemData;
                    itemData << QString::fromStdString(bone->getName());

                    SceneTreeItem* boneItem = new SceneTreeItem(itemData, BONE, item);
                    boneItem->setObject(object);
                    boneItem->setBone(bone);
                    item->appendChild(boneItem);

                    updateBonesFromSkeleton(boneItem, bone, object);
                }
            }
        }
    }
}

void MainWindow::updateBonesFromSkeleton(SceneTreeItem* item, Magic3D::Bone* bone, Magic3D::Object* object)
{
    if (bone)
    {
        std::vector<int>* childs = bone->getChilds();
        int size = childs->size();
        for (int i = 0; i < size; i++)
        {
            Magic3D::Bone* b = bone->getSkeleton()->getBone(childs->at(i));
            if (b && b->getParent())
            {
                QList<QVariant> itemData;
                itemData << QString::fromStdString(b->getName());

                SceneTreeItem* boneItem = new SceneTreeItem(itemData, BONE, item);
                boneItem->setObject(object);
                boneItem->setBone(b);
                item->appendChild(boneItem);

                updateBonesFromSkeleton(boneItem, b, object);
            }
        }
    }
}

void MainWindow::updateFromEngine()
{
    /*if (reloading)
    {
        return;
    }*/
    sceneTreeModel->getRoot()->clear();
    addLayer(MAIN_LAYER_NAME, true);

    Magic3D::Scene* scene = Magic3D::Scene::getInstance();

    std::vector<Magic3D::Layer*>* layers = scene->getLayers();

    std::vector<Magic3D::Layer*>::const_iterator it_l = layers->begin();
    while (it_l != layers->end())
    {
        Magic3D::Layer* layer = *it_l++;

        SceneTreeItem* item = NULL;

        if (layer->getName().compare(M3D_MAIN_LAYER_NAME) == 0)
        {
            item = sceneTreeModel->getRoot()->child(0);
        }
        else
        {
            item = addLayer(QString::fromStdString(layer->getName()), false, true);
        }

        if (item)
        {
            item->setVisible(layer->isVisible());
            updateObjectsFromEngine(item, layer->getObjects2D());
            updateObjectsFromEngine(item, layer->getObjects3D());
            updateObjectsFromEngine(item, layer->getLights());
        }
    }

    QModelIndex root = sceneTreeModel->index(sceneTreeModel->getRoot()->row(), 0);
    ui->treeScene->collapse(root);
    ui->treeScene->expand(root);
    ui->treeScene->expand(sceneTreeModel->index(sceneTreeModel->getRoot()->child(0)));
    ui->treeScene->expand(sceneTreeModel->index(sceneTreeModel->getRoot()->child(0)->child(0)));
    ui->treeScene->expand(sceneTreeModel->index(sceneTreeModel->getRoot()->child(0)->child(1)));

    bool l = loading;
    loading = false;
    if (getProject() && !getProject()->getSelectedObject().empty())
    {
        if (getProject()->getSelectedBone().empty())
        {
            QString name = QString::fromStdString(getProject()->getSelectedObject());
            selectObjectByName(name, true);
        }
        else
        {
            QString name = QString::fromStdString(getProject()->getSelectedObject());
            SceneTreeItem* item = sceneTreeModel->getRoot()->findChild(name, true);
            if (item)
            {
                name = QString::fromStdString(getProject()->getSelectedBone());
                selectBoneByName(item, name);
            }
        }
    }
    else
    {
        ui->treeScene->selectionModel()->setCurrentIndex(root, QItemSelectionModel::ClearAndSelect);
    }

    if (editorLua->getFileName().compare("") != 0)
    {
        editorLua->setFocus();

        QSettings settings;
        if (settings.value("script_file").toString().compare(editorLua->getFileName()) == 0)
        {
            editorLua->setCursorPosition(settings.value("script_cursor_pos").toInt());
        }
    }
    loading = l;

    updateInfo();
}

void MainWindow::selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    if (loading)
    {
        return;
    }
    if (oldSelection.isEmpty())
    {

    }
    if (!newSelection.isEmpty())
    {
        QModelIndex index = newSelection.indexes().front();
        if (index.isValid())
        {
            SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
            ui->actionRemoveLayer->setEnabled(item->getType() == LAYER);
            ui->actionRenameLayer->setEnabled(item->getType() == LAYER);
            ui->actionVisibleLayer->setEnabled(item->getType() == LAYER);
            ui->actionVisibleLayer->setChecked(isLayerVisible(item));
            ui->actionRemoveObject->setEnabled(item->getType() == OBJECT);
            ui->actionRenameObject->setEnabled(item->getType() == OBJECT);
            ui->actionCloneObject->setVisible(item->getType() == OBJECT && (item->getObject() && (item->getObject()->getType() == Magic3D::eOBJECT_MODEL || item->getObject()->getType() == Magic3D::eOBJECT_PARTICLES || item->getObject()->getRender() == Magic3D::eRENDER_2D)));


            magic3dwidget->saveScript(false);
            magic3dwidget->setSelection(item->getObject(), item->getBone());

            bool isRoot = item->getType() == ROOT;
            sceneinfo->setVisible(isRoot);
            if (isRoot)
            {
                sceneinfo->update();
            }

            if (isRoot || (item->getType() == OBJECT && item->getObject()))
            {
                magic3dwidget->loadScript();
            }
            else
            {
                loading = true;
                editorLua->clear();
                editorLua->setFileName("");
                loading = false;
                ui->dockScript->setWindowTitle("Script");
            }

            switch (item->getType())
            {
                case MAIN_LAYER: magic3dwidget->setLastLayer(MAIN_LAYER_NAME); break;
                case LAYER: magic3dwidget->setLastLayer(item->data(0).toString()); break;
                case OBJECT:
                case BONE: magic3dwidget->setLastLayer(item->getObject() && item->getObject()->getLayer() ? QString::fromStdString(item->getObject()->getLayer()->getName()) : MAIN_LAYER_NAME); break;
                default: break;
            }

            if (magic3dwidget)
            {
                magic3dwidget->updateScriptView(isRoot);
            }

            bool enabled = item->getType() == LAYER || (item->getType() == OBJECT && item->getObject() && item->getObject()->getRender() == Magic3D::eRENDER_2D);
            ui->actionBringToFront->setEnabled(enabled);
            ui->actionBringForward->setEnabled(enabled);
            ui->actionSendBackward->setEnabled(enabled);
            ui->actionSendToBack->setEnabled(enabled);

            if (item->getType() == OBJECT && item->getObject() && item->getObject()->getType() == Magic3D::eOBJECT_CAMERA)
            {
                Magic3D::Camera* camera = (Magic3D::Camera*)item->getObject();

                if (camera->getProjectionType() == Magic3D::ePROJECTION_PERSPECTIVE)
                {
                    Magic3D::Renderer::getInstance()->getCurrentViewPort()->setPerspective(camera);
                }
                else
                {
                    Magic3D::Renderer::getInstance()->getCurrentViewPort()->setOrthographic(camera);
                }
                update();
            }

            updateMenus();
        }
    }
}

void MainWindow::updateToolRender()
{
    if (!updating)
    {
        updating = true;
        ui->chkTRSX->setChecked(magic3dwidget->isTRSX());
        ui->chkTRSY->setChecked(magic3dwidget->isTRSY());
        ui->chkTRSZ->setChecked(magic3dwidget->isTRSZ());
        updating = false;
    }
}

void MainWindow::setToolRender()
{
    if (!updating)
    {
        magic3dwidget->setTRS(ui->chkTRSX->isChecked(), ui->chkTRSY->isChecked(), ui->chkTRSZ->isChecked());
    }
}

void MainWindow::clearLog()
{
    ui->txtLog->setHtml("<html>\n<body bgcolor=\"#000000\">\n</body>\n</html>");
}

void MainWindow::updateMaterialsList()
{
    QString fileName = "*.material";
    QString path = tr(Magic3D::ResourceManager::getMaterials()->getPath("").c_str());

    QDir currentDir = QDir(path);
    filesMaterial = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    filesMaterial.append(UTILS_NONE);

    for (int i = 0; i < filesMaterial.count() - 1; i++)
    {
        QString file = filesMaterial.at(i);
        filesMaterial.replace(i, file.left(file.length() - 9));
    }

    qSort(filesMaterial.begin(), filesMaterial.end(), lessNone);
}

QStringList& MainWindow::getMaterialList()
{
    return filesMaterial;
}

void MainWindow::updateParentsList(bool tween, bool rigidbody)
{
    parentsList.clear();
    parentsList.append(UTILS_NONE);

    std::vector<Magic3D::Layer*>* layers = Magic3D::Scene::getInstance()->getLayers();
    std::vector<Magic3D::Layer*>::const_iterator it_l = layers->begin();
    while (it_l != layers->end())
    {
        Magic3D::Layer* layer = *it_l++;

        std::vector<Magic3D::Object*>* objects = layer->getObjects3D();
        std::vector<Magic3D::Object*>::const_iterator it_o = objects->begin();
        while (it_o != objects->end())
        {
            Magic3D::Object* obj = *it_o++;

            bool add = (!tween || getTweenCount(obj)) && (!rigidbody || obj->getRigidBody()) ;
            if (obj->getType() == Magic3D::eOBJECT_CAMERA)
            {
                Magic3D::Camera* camera = static_cast<Magic3D::Camera*>(obj);

                add = add && camera->getProjectionType() != Magic3D::ePROJECTION_ORTHOGRAPHIC;
            }
            if (add)
            {
                parentsList.append(QString::fromStdString(obj->getName()));
            }
        }
    }

    qSort(parentsList.begin(), parentsList.end(), lessNone);
}


int MainWindow::getTweenCount(Magic3D::Object* object)
{
    int count = 0;

    if (object)
    {
        count += object->getTweenCount();

        if (object->getType() == Magic3D::eOBJECT_MODEL)
        {
            Magic3D::Model* model = static_cast<Magic3D::Model*>(object);
            std::vector<Magic3D::Bone*>* bones = model->getSkeleton(true)->getBones();
            std::vector<Magic3D::Bone*>::const_iterator it_b = bones->begin();
            while (it_b != bones->end())
            {
                Magic3D::Bone* bone = *it_b++;

                count += bone->getTweenCount();
            }
        }
    }

    return count;
}

QStringList& MainWindow::getParentsList()
{
    return parentsList;
}

void MainWindow::setComboList(QStringList& list, QComboBox* combo)
{
    QString current = combo->currentText();
    combo->clear();

    combo->addItems(list);

    if (current.isEmpty())
    {
        combo->setCurrentIndex(0);
    }
    else
    {
        combo->setCurrentIndex(combo->findText(current));
    }
}

void MainWindow::setScriptObjectsText(QString text)
{
    scriptObjects.setText(text);
}


void MainWindow::on_actionNew_triggered()
{
    ui->actionClose_Project->trigger();
    if (!project)
    {
        NewProjectWindow newProjectWindow;
        if (newProjectWindow.exec() == QDialog::Accepted)
        {
            project = new MGE::MGEProject(newProjectWindow.getName().toStdString(), UTILS_DEFAULT_PATH.toStdString());
            project->setPathData(newProjectWindow.getData().toStdString());

            Magic3D::Physics::getInstance()->stop(false);
            Magic3D::Script::getInstance()->stop(false);

            update();
            updateFromEngine();

            addObject("orthographic", Magic3D::eOBJECT_CAMERA, QPoint(0, 0));
            addObject("perspective", Magic3D::eOBJECT_CAMERA, QPoint(0, 0));

            Magic3D::Camera* cameraPerspective = (Magic3D::Camera*)Magic3D::ResourceManager::getObjects()->get("perspective");
            Magic3D::Camera* cameraOrthographic = (Magic3D::Camera*)Magic3D::ResourceManager::getObjects()->get("orthographic");
            cameraOrthographic->setProjectionType(Magic3D::ePROJECTION_ORTHOGRAPHIC);

            Magic3D::Renderer::getInstance()->getCurrentViewPort()->setOrthographic(cameraOrthographic);
            Magic3D::Renderer::getInstance()->getCurrentViewPort()->setPerspective(cameraPerspective);

            cameraoptions->setSpeedMoveFactor(0.25f);
            cameraoptions->setSpeedLookFactor(0.25f);

            update();

            magic3dwidget->start();
            updateHelp();
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    //if (!project)
    {
        QString fileName;
        if (reloadProject.isEmpty())
        {
            fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), UTILS_DEFAULT_PATH + M3D_PATH_DEFAULT, tr(MGE_PROJECT_FILTER));
        }
        else
        {
            fileName = reloadProject;
            reloadProject.clear();
        }
        if (!fileName.isEmpty())
        {
            ui->actionClose_Project->trigger();
            if (!project)
            {
                loading = true;
                project = new MGE::MGEProject(Utils::getFileName(fileName).toStdString(), UTILS_DEFAULT_PATH.toStdString());
                project->setPathData(M3D_PATH_DEFAULT);
                project->load();                
                magic3dwidget->load();
                cameraoptions->setSpeedMoveFactor(project->getCameraMove());
                cameraoptions->setSpeedLookFactor(project->getCameraLook());                
            }
        }
    }
}


void MainWindow::on_actionSave_triggered()
{
    if (project)
    {
        tweensinfo->resetObject(true);
        magic3dwidget->save();
        project->save();
        update();
    }
    /*else
    {
        ui->actionSave_As->trigger();
    }*/
}

void MainWindow::on_actionSave_As_triggered()
{
    /*QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), QDir::currentPath(), tr(MGE_PROJECT_FILTER));
    if (project && !fileName.isEmpty())
    {
        magic3dwidget->saveScript(true);
        project->save(Utils::getFileName(fileName).toLocal8Bit().data(), Utils::getFilePath(fileName).toLocal8Bit().data());
        update();
    }*/
}

void MainWindow::on_actionQuit_triggered()
{
    ui->actionClose_Project->trigger();
    if (!project)
    {
        close();
    }
}


void MainWindow::on_actionClose_Project_triggered()
{
    if (project)
    {
        bool close = false;
        if (project->isChanged() || project->isChangedScript())
        {
            int ret = Utils::confirmSave(tr("Save Project"), tr("The project has been modified."), tr("Do you want to save your changes?"));
            if (ret != QMessageBox::Cancel)
            {
                if (ret == QMessageBox::Save)
                {
                    ui->actionSave->trigger();
                    if (!project->isChanged() && !project->isChangedScript())
                    {
                        close = true;
                    }
                }
                else
                {
                    close = true;
                }
            }
        }
        else
        {
            close = true;
        }

        if (close)
        {
            materialinfo->checkSaved(false);
            materialinfo->cancel();

            loading = true;
            magic3dwidget->stop();
            magic3dwidget->setSelection(NULL, NULL);
            magic3dwidget->setLastLayer(MAIN_LAYER_NAME);

            delete sceneTreeModel;
            sceneTreeModel = NULL;

            delete project;
            project = NULL;

            Magic3D::Scene::getInstance()->clear(true);
            editorLua->clear();
            clearLog();            
            ui->dockScript->setWindowTitle("Script");
            ui->dockScript->setEnabled(false);
            loading = false;
        }
    }
    update();
}

void MainWindow::on_actionConfigure_Project_triggered()
{
    if (project)
    {
        NewProjectWindow newProjectWindow;
        newProjectWindow.setName(QString::fromStdString(project->getName()));
        newProjectWindow.setData(QString::fromStdString(project->getPathData()));
        if (newProjectWindow.exec() == QDialog::Accepted)
        {
            project->setName(newProjectWindow.getName().toStdString());
            project->setPathData(newProjectWindow.getData().toStdString());
            update();
        }
    }

}

void MainWindow::on_actionAddLayer_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New Layer"), tr("Layer name:"), QLineEdit::Normal, tr(""), &ok);
    if (ok && !text.isEmpty())
    {
        addLayer(text);
    }
    else if (ok)
    {
        Utils::message(tr("Invalid Name"), tr("\"") + text + tr("\" is an invalid name!"));
    }
}

void MainWindow::on_treeScene_customContextMenuRequested(QPoint pos)
{
    if (IsProjectOpened())
    {
        QMenu menu(this);
        menu.addActions(ui->menuLayers->actions());
        QModelIndex index = ui->treeScene->currentIndex();
        if (index.isValid())
        {
            SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
            ui->actionVisibleLayer->setChecked(isLayerVisible(item));

            if (item->getType() == OBJECT)
            {
                menu.addSeparator();
                menu.addAction(ui->actionRemoveObject);
                if (item->getObject() && (item->getObject()->getType() == Magic3D::eOBJECT_MODEL || item->getObject()->getType() == Magic3D::eOBJECT_PARTICLES))
                {
                    menu.addAction(ui->actionCloneObject);
                }
                menu.addSeparator();
                menu.addAction(ui->actionCopy);
            }
            else
            {
                menu.addSeparator();
            }
            menu.addAction(ui->actionPaste);
        }

        menu.exec(ui->treeScene->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::on_actionVisibleLayer_triggered()
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        Magic3D::Layer* layer = magic3dwidget->getLayer(item->data(0).toString().toStdString());
        if (layer)
        {
            bool visible = layer->isVisible();
            layer->setVisible(!visible);
            item->setVisible(!visible);

            updateInfo();
        }
    }
}

void MainWindow::on_actionRemoveLayer_triggered()
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        SceneTreeItem* root = sceneTreeModel->getRoot();
        magic3dwidget->removeLayer(item->data(0).toString().toStdString());
        root->removeChild(item);
        QModelIndex rootIndex = sceneTreeModel->index(root->row(), 0);
        ui->treeScene->selectionModel()->setCurrentIndex(rootIndex, QItemSelectionModel::ClearAndSelect);
        ui->treeScene->collapse(rootIndex);
        ui->treeScene->expand(rootIndex);

        updateInfo();
    }
}

void MainWindow::on_actionRenameLayer_triggered()
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
        bool ok;
        QString text = QInputDialog::getText(this, tr("Edit Layer"), tr("Layer name:"), QLineEdit::Normal, item->data(0).toString(), &ok);
        if (ok && !text.isEmpty())
        {
            if (text.compare(MAIN_LAYER_NAME) == 0 || magic3dwidget->getLayer(text.toStdString()))
            {
                Utils::message(tr("Invalid Name"), tr("Layer named \"") + text + tr("\" already exists!"));
            }
            else
            {
                Magic3D::Scene::getInstance()->getLayer(item->data(0).toString().toStdString())->setName(text.toStdString());
                item->setData(0, QVariant(text));

                SceneTreeItem* root = sceneTreeModel->getRoot();
                QModelIndex rootIndex = sceneTreeModel->index(root->row(), 0);
                ui->treeScene->collapse(rootIndex);
                ui->treeScene->expand(rootIndex);
            }
        }
        else if (ok)
        {
            Utils::message(tr("Invalid Name"), tr("\"") + text + tr("\" is an invalid name!"));
        }

        updateInfo();
    }
}

void MainWindow::on_actionRemoveObject_triggered()
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
        SceneTreeItem* root = item->parent();

        if (item->getObject())
        {
            selectObjectByName(magic3dwidget->getLastLayer(), false);
            if (item->getObject()->getType() == Magic3D::eOBJECT_CAMERA)
            {
                Magic3D::Camera* camera = (Magic3D::Camera*)item->getObject();
                if (camera->getProjectionType() == Magic3D::ePROJECTION_PERSPECTIVE)
                {
                    Magic3D::Renderer::getInstance()->getCurrentViewPort()->setPerspective(NULL);
                }
                else
                {
                    Magic3D::Renderer::getInstance()->getCurrentViewPort()->setOrthographic(NULL);
                }
            }
            if (root->parent()->getType() == MAIN_LAYER)
            {
                magic3dwidget->removeObject(M3D_MAIN_LAYER_NAME, item->data(0).toString().toStdString());
            }
            else
            {
                magic3dwidget->removeObject(root->parent()->data(0).toString().toStdString(), item->data(0).toString().toStdString());
            }
        }

        QModelIndex index = sceneTreeModel->index(root);
        root->removeChild(item);
        ui->treeScene->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
        ui->treeScene->collapse(index);
        ui->treeScene->expand(index);

        updateInfo();
    }
}

void MainWindow::on_actionAddSprite_triggered()
{
    addObject(Magic3D::eOBJECT_SPRITE);
}

void MainWindow::on_actionAddText_triggered()
{
    addObject(Magic3D::eOBJECT_GUI_LABEL);
}

void MainWindow::on_actionAddWindow_triggered()
{
    addObject(Magic3D::eOBJECT_GUI_WINDOW);
}

void MainWindow::on_actionAddButton_triggered()
{
    addObject(Magic3D::eOBJECT_GUI_BUTTON);
}

void MainWindow::on_actionAddSlider_triggered()
{
    addObject(Magic3D::eOBJECT_GUI_SLIDER);
}

void MainWindow::on_actionAddModel_triggered()
{
    addObject(Magic3D::eOBJECT_MODEL);
}

void MainWindow::on_actionInstance_triggered()
{
    addObject(Magic3D::eOBJECT_INSTANCE);
}

void MainWindow::on_actionCamera_triggered()
{
    addObject(Magic3D::eOBJECT_CAMERA);
}

void MainWindow::on_actionLight_triggered()
{
    addObject(Magic3D::eOBJECT_LIGHT);
}

void MainWindow::on_actionSound_triggered()
{
    addObject(Magic3D::eOBJECT_SOUND);
}

void MainWindow::on_actionTerrain_triggered()
{
    addObject(Magic3D::eOBJECT_TERRAIN);
}

void MainWindow::on_actionParticles_triggered()
{
    addObject(Magic3D::eOBJECT_PARTICLES);
}

void MainWindow::on_actionWater_triggered()
{
    addObject(Magic3D::eOBJECT_WATER);
}

void MainWindow::on_actionVegetation_triggered()
{
    addObject(Magic3D::eOBJECT_VEGETATION);
}

void MainWindow::on_actionJoystick_triggered()
{
    addObject(Magic3D::eOBJECT_JOYSTICK);
}

void MainWindow::on_actionBringToFront_triggered()
{
    orderItem(eFRONT);
}

void MainWindow::on_actionSendToBack_triggered()
{
    orderItem(eBACK);
}

void MainWindow::on_actionBringForward_triggered()
{
    orderItem(eFORWARD);
}

void MainWindow::on_actionSendBackward_triggered()
{
    orderItem(eBACKWARD);
}

void MainWindow::on_actionSolid_toggled(bool checked)
{
    if (!updating)
    {
        if (checked)
        {

        }
        updating = true;
        uncheckRenderMode();
        ui->actionSolid->setChecked(true);
        Magic3D::Renderer::getInstance()->setRenderMode(Magic3D::eRENDER_MODE_SOLID);
        btnRenderMode->setIcon(ui->actionSolid->icon());
        updating = false;
    }
}

void MainWindow::on_actionWireframe_toggled(bool checked)
{
    if (!updating)
    {
        if (checked)
        {

        }
        updating = true;
        uncheckRenderMode();
        ui->actionWireframe->setChecked(true);
        Magic3D::Renderer::getInstance()->setRenderMode(Magic3D::eRENDER_MODE_WIREFRAME);
        btnRenderMode->setIcon(ui->actionWireframe->icon());
        updating = false;
    }
}

void MainWindow::on_actionPortrait_triggered()
{
    ui->actionLandscape->setChecked(false);
    ui->actionPortrait->setChecked(true);
    updateScreen();
}

void MainWindow::on_actionLandscape_triggered()
{
    ui->actionPortrait->setChecked(false);
    ui->actionLandscape->setChecked(true);
    updateScreen();
}

void MainWindow::on_actionFree_triggered()
{
    ui->actionPortrait->setChecked(false);
    ui->actionLandscape->setChecked(false);

    ui->actionDevice4_3->setChecked(false);
    ui->actionDevice16_9->setChecked(false);
    ui->actionDevice16_10->setChecked(false);
    ui->actionFree->setChecked(true);

    screenWidth = QWIDGETSIZE_MAX;
    screenHeight = QWIDGETSIZE_MAX;
    updateScreen();
}

void MainWindow::on_actionDevice4_3_triggered()
{
    screenWidth = 640.0f;
    screenHeight = 480.0f;

    ui->actionFree->setChecked(false);
    ui->actionDevice4_3->setChecked(true);
    ui->actionDevice16_9->setChecked(false);
    ui->actionDevice16_10->setChecked(false);

    updateScreen();
}

void MainWindow::on_actionDevice16_9_triggered()
{
    screenWidth = 852.0f;
    screenHeight = 480.0f;

    ui->actionFree->setChecked(false);
    ui->actionDevice4_3->setChecked(false);
    ui->actionDevice16_9->setChecked(true);
    ui->actionDevice16_10->setChecked(false);

    updateScreen();
}

void MainWindow::on_actionDevice16_10_triggered()
{
    screenWidth = 768.0f;
    screenHeight = 480.0f;

    ui->actionFree->setChecked(false);
    ui->actionDevice4_3->setChecked(false);
    ui->actionDevice16_9->setChecked(false);
    ui->actionDevice16_10->setChecked(true);

    updateScreen();
}

void MainWindow::updateScreen()
{
    if (ui->actionPortrait->isChecked() || (screenWidth == 0.0f || screenHeight == 0.0f))
    {
        if (magic3dwidget)
        {
            magic3dwidget->setFixedSize(screenHeight, screenWidth);
        }
    }
    else
    {
        ui->actionLandscape->setChecked(true);
        if (magic3dwidget)
        {
            magic3dwidget->setFixedSize(screenWidth, screenHeight);
        }
    }
}

void MainWindow::on_actionOrthographic_triggered()
{
    Magic3D::Renderer::getInstance()->getCurrentViewPort()->setOrthographic(NULL);
    update();
}

void MainWindow::on_actionPerspective_triggered()
{
    Magic3D::Renderer::getInstance()->getCurrentViewPort()->setPerspective(NULL);
    update();
}

void MainWindow::on_actionSimulate_triggered()
{
    if (!Magic3D::Physics::getInstance()->isPlaying())
    {
        QSettings settings;
        settings.setValue("script_file", editorLua->getFileName());
        settings.setValue("script_cursor_pos", editorLua->getCursorPosition());
        reloadProject = QString::fromStdString(getProject()->getFullPath());
        clearLog();
        ui->actionSave->trigger();

        Magic3D::Physics::getInstance()->play(false);
        Magic3D::Script::getInstance()->play();
        Magic3D::Scene::getInstance()->playObjectsTweens();
        Magic3D::Scene::getInstance()->spawnInstances();

        magic3dwidget->setSelection(NULL, NULL);
        updateFromEngine();

        bool isRoot = false;
        QModelIndex index = ui->treeScene->currentIndex();
        if (index.isValid())
        {
            SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
            isRoot = item->getType() == ROOT;
        }

        magic3dwidget->updateScriptView(isRoot);
        ui->actionUpdate_Scene_Tree_View->setVisible(true);
        magic3dwidget->setFocus();
    }
    else
    {
        magic3dwidget->setSelection(NULL, NULL);
        ui->actionSimulate->setEnabled(false);
        ui->actionOpen->trigger();
    }
}

void MainWindow::on_actionShow_Gizmos_triggered()
{
    magic3dwidget->setShowGizmos(!magic3dwidget->isShowingGizmos());
    update();
}

void MainWindow::on_actionShow_Octree_triggered()
{
    magic3dwidget->setShowOctree(!magic3dwidget->isShowingOctree());
    update();
}

void MainWindow::on_actionShow_Octree_Objects_triggered()
{
    magic3dwidget->setShowOctreeObjects(!magic3dwidget->isShowingOctreeObjects());
    update();
}

void MainWindow::on_actionShow_FPS_triggered()
{
    Magic3D::Scene::getInstance()->showFPSText(!Magic3D::Scene::getInstance()->isShowingFPSText());
    update();
}

void MainWindow::on_actionShow_Information_triggered()
{
    Magic3D::Scene::getInstance()->showINFOText(!Magic3D::Scene::getInstance()->isShowingINFOText());
    update();
}

void MainWindow::on_actionShow_Mouse_Position_triggered()
{
    Magic3D::Scene::getInstance()->showWINDOWText(!Magic3D::Scene::getInstance()->isShowingWINDOWText());
    update();
}

void MainWindow::on_actionTop_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_TOP);
}

void MainWindow::on_actionBottom_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_BOTTOM);
}

void MainWindow::on_actionFront_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_FRONT);
}

void MainWindow::on_actionBack_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_BACK);
}

void MainWindow::on_actionLeft_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_LEFT);
}

void MainWindow::on_actionRight_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_RIGHT);
}

void MainWindow::on_actionFly_triggered()
{
    magic3dwidget->setCameraPosition(MGE::eCAMERA_POSITION_FLY);
}

void MainWindow::on_actionCloneObject_triggered()
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        if (item->getObject())
        {
            addObject(item->getObject()->getType(), item->getObject());
        }
    }
}


void MainWindow::Editor_textScriptChanged()
{
    if (!loading && project)
    {
        if (!ui->dockScript->windowTitle().endsWith("*"))
        {
            ui->dockScript->setWindowTitle(ui->dockScript->windowTitle() + "*");
        }
        setScriptChanged(true);
        update();
    }
}

void MainWindow::on_treeHelp_expanded(const QModelIndex &index)
{
    if (index.isValid())
    {
    }
}

void MainWindow::on_actionCopy_triggered()
{
    if (magic3dwidget->getSelectedObject())
    {
        magic3dwidget->setCopiedObject(magic3dwidget->getSelectedObject());
        updateMenus();
    }
}

void MainWindow::on_actionPaste_triggered()
{
    //spawnObject(magic3dwidget->getCopiedObject());
    Magic3D::Object* obj = magic3dwidget->getCopiedObject();
    if (obj)
    {
        duplicateObject(QString::fromStdString(Magic3D::ResourceManager::getObjects()->getNewName(obj->getName())), obj, false);
    }
}

void MainWindow::on_treeScene_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        if (item->getType() == OBJECT && item->getObject() && item->getObject()->getType() != Magic3D::eOBJECT_CAMERA)
        {
            if (item->getObject()->getRender() == Magic3D::eRENDER_2D)
            {
                Magic3D::Camera* camera = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getOrthographic();
                if (!item->getObject()->isInFrustum())
                {
                    Magic3D::Window* window = Magic3D::Renderer::getInstance()->getWindow();
                    Magic3D::Vector3 pos = item->getObject()->getPositionFromParent() + Magic3D::Vector3(-window->getWidth() * 0.5f, -window->getHeight() * 0.5f, 0.0f);

                    magic3dwidget->getTranslateCamera().setDuration(0.5f);
                    magic3dwidget->getTranslateCamera().setPhysicsObject(camera);
                    magic3dwidget->getTranslateCamera().setPosition(pos - camera->getPosition());
                    magic3dwidget->getTranslateCamera().play();
                }
            }
            else
            {
                Magic3D::Camera* camera = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getPerspective();

                Magic3D::Box b = item->getObject()->getBoundingBox();
                Magic3D::Vector3 pos = (item->getObject()->getMatrixFromParent() * Magic3D::Vector4(b.getCenter(), 1.0f)).getXYZ() + camera->getDirectionBack() * item->getObject()->getMaxSizeFromParent() * 2.0f;

                magic3dwidget->getTranslateCamera().setDuration(0.5f);
                magic3dwidget->getTranslateCamera().setPhysicsObject(camera);
                magic3dwidget->getTranslateCamera().setPosition(pos - camera->getPosition());
                magic3dwidget->getTranslateCamera().play();
            }
        }
    }
}

void MainWindow::on_actionReset_Window_Layout_triggered()
{
    resetLayout = true;
    Utils::message("Warning", "The window layout will reset on next time you open the editor!");
}

void MainWindow::on_dockMaterials_visibilityChanged(bool visible)
{
    if (visible)
    {

    }
    if (materialinfo && materialinfo->checkSaved(false))
    {
        materialinfo->update();
    }
}

void MainWindow::on_chkTRSX_toggled(bool checked)
{
    if (checked)
    {

    }
    setToolRender();
}

void MainWindow::on_chkTRSY_toggled(bool checked)
{
    if (checked)
    {

    }
    setToolRender();
}

void MainWindow::on_chkTRSZ_toggled(bool checked)
{
    if (checked)
    {

    }
    setToolRender();
}

void MainWindow::on_actionUpdate_Scene_Tree_View_triggered()
{
    updateFromEngine();
}

void MainWindow::on_actionRenameObject_triggered()
{
    QModelIndex index = ui->treeScene->currentIndex();
    if (index.isValid())
    {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
        SceneTreeItem* root = item->parent();

        if (item->getObject())
        {
            bool ok;
            QString text = QInputDialog::getText(this, tr("Edit Object"), tr("Object name:"), QLineEdit::Normal, item->data(0).toString(), &ok);
            if (ok && !text.isEmpty())
            {
                if (magic3dwidget->getObject(text.toStdString()))
                {
                    Utils::message(tr("Invalid Name"), tr("Object named \"") + text + tr("\" already exists!"));
                }
                else
                {
                    if (magic3dwidget->renameObject(item->data(0).toString().toStdString(), text.toStdString()))
                    {
                        item->setData(0, QVariant(text));

                        QModelIndex index = sceneTreeModel->index(root);
                        ui->treeScene->selectionModel()->setCurrentIndex(sceneTreeModel->index(item), QItemSelectionModel::ClearAndSelect);
                        ui->treeScene->collapse(index);
                        ui->treeScene->expand(index);

                        updateInfo();
                    }
                    else
                    {
                        Utils::message(tr("Invalid Name"), tr("Object named \"") + text + tr("\" already exists!"));
                    }
                }
            }
            else if (ok)
            {
                Utils::message(tr("Invalid Name"), tr("\"") + text + tr("\" is an invalid name!"));
            }
        }
    }
}

void MainWindow::on_actionShow_GizmosPhysics_triggered()
{
    magic3dwidget->setShowGizmosPhysics(!magic3dwidget->isShowingGizmosPhysics());
    update();
}

void MainWindow::on_actionNormals_triggered()
{
    if (!updating)
    {
        updating = true;
        uncheckRenderMode();
        ui->actionNormals->setChecked(true);
        Magic3D::Renderer::getInstance()->setRenderMode(Magic3D::eRENDER_MODE_NORMALS);
        btnRenderMode->setIcon(ui->actionNormals->icon());
        updating = false;
    }
}

void MainWindow::on_actionUVs_triggered()
{
    if (!updating)
    {
        updating = true;
        uncheckRenderMode();
        ui->actionUVs->setChecked(true);
        Magic3D::Renderer::getInstance()->setRenderMode(Magic3D::eRENDER_MODE_UVS);
        btnRenderMode->setIcon(ui->actionUVs->icon());
        updating = false;
    }
}

void MainWindow::on_actionDepth_triggered()
{
    if (!updating)
    {
        updating = true;
        uncheckRenderMode();
        ui->actionDepth->setChecked(true);
        Magic3D::Renderer::getInstance()->setRenderMode(Magic3D::eRENDER_MODE_DEPTH);
        btnRenderMode->setIcon(ui->actionDepth->icon());
        updating = false;
    }
}

void MainWindow::on_actionScreenEffects_triggered()
{
    if (!updating)
    {
        updating = true;
        uncheckRenderMode();
        ui->actionScreenEffects->setChecked(true);
        Magic3D::Renderer::getInstance()->setRenderMode(Magic3D::eRENDER_MODE_SCREEN_WIREFRAME);
        btnRenderMode->setIcon(ui->actionScreenEffects->icon());
        updating = false;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    splashScreen->setModal(true);
    splashScreen->show();
}

void MainWindow::on_actionNewScript_triggered()
{
    if (magic3dwidget->getSelectedObject() && !magic3dwidget->getSelectedBone())
    {
        magic3dwidget->saveScript(false);

        QString scriptPath = M3D_PATH_DEFAULT;
        scriptPath.append(M3D_PATH_SCRIPT);
        QString fileName = QFileDialog::getSaveFileName(this, tr("New Script"), UTILS_DEFAULT_PATH + scriptPath, tr(MGE_SCRIPT_FILTER));

        bool error = fileName.endsWith(".main.script");
        if (!fileName.isEmpty() && !error)
        {
            QString relativePath = Utils::getFilePath(fileName);
            fileName = Utils::getFileName(fileName);
            fileName = fileName.left(fileName.length() - QString(M3D_SCRIPT_FILE).length());
            relativePath = relativePath.mid(relativePath.indexOf(scriptPath) + scriptPath.length());

            relativePath.append(fileName);

            magic3dwidget->getSelectedObject()->setScript(relativePath.toStdString());
            magic3dwidget->loadScript();
            setScriptChanged(true);
            magic3dwidget->saveScript(true);

            setProjectChanged(true);
            update();
        }

        if (error)
        {
            Utils::message("Error", "Can't use scene scripts in objects!");
        }
    }
}

void MainWindow::on_actionOpenScript_triggered()
{
    if (magic3dwidget->getSelectedObject() && !magic3dwidget->getSelectedBone())
    {
        magic3dwidget->saveScript(false);

        QString scriptPath = M3D_PATH_DEFAULT;
        scriptPath.append(M3D_PATH_SCRIPT);
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Script"), UTILS_DEFAULT_PATH + scriptPath, tr(MGE_SCRIPT_FILTER));

        bool error = fileName.endsWith(".main.script");
        if (!fileName.isEmpty() && !error)
        {
            QString relativePath = Utils::getFilePath(fileName);
            fileName = Utils::getFileName(fileName);
            fileName = fileName.left(fileName.length() - QString(M3D_SCRIPT_FILE).length());
            relativePath = relativePath.mid(relativePath.indexOf(scriptPath) + scriptPath.length());

            relativePath.append(fileName);

            magic3dwidget->getSelectedObject()->setScript(relativePath.toStdString());
            magic3dwidget->loadScript();

            setScriptChanged(true);
            setProjectChanged(true);
            update();
        }

        if (error)
        {
            Utils::message("Error", "Can't use scene scripts in objects!");
        }
    }
}

void MainWindow::on_actionPack_triggered()
{
#if (defined(__APPLE__) && defined(__MACH__))
    QString textureToolFile = "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/texturetool";
    QFile textureTool(textureToolFile);
    if (textureTool.exists())
    {
        int errors = 0;
        QString fileName = "*.png";

        QDirIterator it(QString::fromStdString(Magic3D::ResourceManager::getInstance()->getPath()), QStringList() << fileName, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString texture = it.next();
            QFile::remove(texture + ".pvr");

            QImage tmp;
            tmp.load(texture);

            QImage flipped = tmp.mirrored(false, true);
            flipped.save(texture + ".bkp", "PNG");

            QString compression;
            if (tmp.depth() > 8)
            {
                compression = "--bits-per-pixel-4";
            }
            else
            {
                compression = "--bits-per-pixel-2";
            }
            QString params = " -e PVRTC --channel-weighting-linear " + compression + " -f PVR -m -o \"" + texture + ".pvr\" \"" + texture + ".bkp\"";
            QProcess convert;
            convert.start(textureToolFile + params);
            convert.waitForFinished(-1);
            QString error = QString(convert.readAllStandardError());
            if (!error.isEmpty())
            {
                errors++;
                Magic3D::Log::logFormat(Magic3D::eLOG_FAILURE, "PVRTC Error: %s - %s", texture.toStdString().c_str(), error.toStdString().c_str());
            }
            else
            {
                Magic3D::Log::logFormat(Magic3D::eLOG_SUCCESS, "PVRTC: %s - Ok! - %s", texture.toStdString().c_str(), compression.toStdString().c_str());
            }
            convert.close();
            QFile::remove(texture + ".bkp");

            QApplication::processEvents();
        }

        Magic3D::Log::logFormat(Magic3D::eLOG_RENDERER, "Pack Finished! - %d Errors.", errors);
    }
    else
    {
        Utils::message("Error!", "Can't find PVRTC texture tool!");
    }
#endif
}
