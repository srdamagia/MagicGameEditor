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
#include <QScreen>
#include <QAbstractItemView>
#include "magic3dwidget.h"
#include "mainwindow.h"
#include "cameraoptions.h"
#include "object2dinfo.h"
#include "object3dinfo.h"
#include "boneinfo.h"
#include "physicsinfo.h"
#include "skeletoninfo.h"
#include "meshesinfo.h"
#include "particlesinfo.h"
#include "shaderseditor.h"
#include "utils.h"
#include <magic3d/magic3d.h>

void MGE::Magic3DWidget::log(QString label, Magic3D::Vector3 vec)
{
    log(tr("%0 x: %1, y: %2, z: %3").arg(label).arg(vec.getX(), 0, 'f', 3).arg(vec.getY(), 0, 'f', 3).arg(vec.getZ(), 0, 'f', 3));
}

void MGE::Magic3DWidget::log(Magic3D::Matrix4 matrix)
{
    log(tr("row0", "%1, %2, %3, %4").arg(matrix.getCol0().getX(), 0, 'f', 3).arg(matrix.getCol1().getX(), 0, 'f', 3).arg(matrix.getCol2().getX(), 0, 'f', 3).arg(matrix.getCol3().getX(), 0, 'f', 3));
    log(tr("row1", "%1, %2, %3, %4").arg(matrix.getCol0().getY(), 0, 'f', 3).arg(matrix.getCol1().getY(), 0, 'f', 3).arg(matrix.getCol2().getY(), 0, 'f', 3).arg(matrix.getCol3().getY(), 0, 'f', 3));
    log(tr("row2", "%1, %2, %3, %4").arg(matrix.getCol0().getZ(), 0, 'f', 3).arg(matrix.getCol1().getZ(), 0, 'f', 3).arg(matrix.getCol2().getZ(), 0, 'f', 3).arg(matrix.getCol3().getZ(), 0, 'f', 3));
    log(tr("row3", "%1, %2, %3, %4").arg(matrix.getCol0().getW(), 0, 'f', 3).arg(matrix.getCol1().getW(), 0, 'f', 3).arg(matrix.getCol2().getW(), 0, 'f', 3).arg(matrix.getCol3().getW(), 0, 'f', 3));
}

void MGE::Magic3DWidget::log(QString log)
{
    if (mainWindow)
    {
        mainWindow->getUi()->txtLog->insertHtml("<FONT COLOR= \"#FFFFFF\"><PRE>" + log + "<FONT></PRE><BR>");
        mainWindow->getUi()->txtLog->moveCursor(QTextCursor::End);
    }
}

MGE::Magic3DWidget::Magic3DWidget(const QGLFormat& format, MainWindow *parent) : QGLWidget(format, parent)
{
    mainWindow = parent;
    selected = NULL;
    selectedBone = NULL;
    copied = NULL;

    logCallBack = new WidgetLogCallBack(mainWindow ? mainWindow->getUi()->txtLog : NULL);

    lastMenuPos = QPoint(0, 0);

    setAutoFillBackground(false);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    mouseDown = false;
    mouseMoved = false;
    selectedOffset = QPointF(0.0f, 0.0f);

    popup = new QMenu(this);

    keyUp = false;
    keyDown = false;
    keyLeft = false;
    keyRight = false;

    keyCtrl = false;
    keyShift = false;

    trsX = true;
    trsY = true;
    trsZ = true;

    axis = eAXIS_TYPE_CAMERA;

    loading = false;

    objectRestore.position = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    objectRestore.rotation = Magic3D::Quaternion::identity();
    objectRestore.scale = Magic3D::Vector3(1.0f, 1.0f, 1.0f);
    objectRestore.changed = false;

    cameraPosition = eCAMERA_POSITION_FLY;
    flyPosition = Magic3D::Vector3(0.0f, 0.0f, 10.0f);
    flyRotation = Magic3D::Vector3(0.0f, 0.0f, 0.0f);

    cameraTop    = Magic3D::Vector3(  0.0f,  10.0f,   0.0f);
    cameraBottom = Magic3D::Vector3(  0.0f, -10.0f,   0.0f);
    cameraFront  = Magic3D::Vector3(  0.0f,   0.0f,  10.0f);
    cameraBack   = Magic3D::Vector3(  0.0f,   0.0f, -10.0f);
    cameraLeft   = Magic3D::Vector3(-10.0f,   0.0f,   0.0f);
    cameraRight  = Magic3D::Vector3( 10.0f,   0.0f,   0.0f);

    translateCamera.setAllwaysUpdate(true);

    lastLayer = MAIN_LAYER_NAME;
}

MGE::Magic3DWidget::~Magic3DWidget()
{
    if (popup)
    {
        popup->clear();
        delete popup;
    }

    Magic3D::Magic3D::finish();

    if (logCallBack)
    {
        delete logCallBack;
        logCallBack = NULL;
    }
}

void MGE::Magic3DWidget::start()
{
    timer = startTimer(0);
}

void MGE::Magic3DWidget::stop()
{
    translateCamera.stop();
    translateCamera.setPhysicsObject(NULL);
    killTimer(timer);
}

void MGE::Magic3DWidget::save()
{
    if (mainWindow->getProject())
    {
        bool up = mainWindow->getProject()->isChangedScript();
        saveScript(true);
        if (up && !mainWindow->getProject()->isChangedScript())
        {
            mainWindow->updateHelp();
        }

        mainWindow->getProject()->setCameraTop(cameraTop);
        mainWindow->getProject()->setCameraBottom(cameraBottom);
        mainWindow->getProject()->setCameraFront(cameraFront);
        mainWindow->getProject()->setCameraBack(cameraBack);
        mainWindow->getProject()->setCameraLeft(cameraLeft);
        mainWindow->getProject()->setCameraRight(cameraRight);
    }
}

void MGE::Magic3DWidget::load()
{
    loading = true;

    Magic3D::Scene::getInstance()->clear(true);

    Magic3D::Scene::getInstance()->load(mainWindow->getProject()->getFile(), false);

    Magic3D::Physics::getInstance()->stop(false);
    Magic3D::Script::getInstance()->stop(false);

    if (mainWindow->getProject())
    {
        cameraTop    = mainWindow->getProject()->getCameraTop();
        cameraBottom = mainWindow->getProject()->getCameraBottom();
        cameraFront  = mainWindow->getProject()->getCameraFront();
        cameraBack   = mainWindow->getProject()->getCameraBack();
        cameraLeft   = mainWindow->getProject()->getCameraLeft();
        cameraRight  = mainWindow->getProject()->getCameraRight();
    }

    start();
}

void MGE::Magic3DWidget::setShowGizmos(bool show)
{
    Magic3D::Renderer::getInstance()->setShowGizmos(show);
}

bool MGE::Magic3DWidget::isShowingGizmos()
{
    return Magic3D::Renderer::getInstance()->isShowingGizmos();
}

void MGE::Magic3DWidget::setShowGizmosPhysics(bool show)
{
    Magic3D::Renderer::getInstance()->setShowGizmosPhysics(show);
}

bool MGE::Magic3DWidget::isShowingGizmosPhysics()
{
    return Magic3D::Renderer::getInstance()->isShowingGizmosPhysics();
}

void MGE::Magic3DWidget::setShowOctree(bool show)
{
    Magic3D::Renderer::getInstance()->setShowOctree(show);
}

bool MGE::Magic3DWidget::isShowingOctree()
{
    return Magic3D::Renderer::getInstance()->isShowingOctree();
}

void MGE::Magic3DWidget::setShowOctreeObjects(bool show)
{
    Magic3D::Renderer::getInstance()->setShowOctreeObjects(show);
}

bool MGE::Magic3DWidget::isShowingOctreeObjects()
{
    return Magic3D::Renderer::getInstance()->isShowingOctreeObjects();
}

void MGE::Magic3DWidget::setCameraPosition(CAMERA_POSITION position)
{
    Magic3D::Camera* camera = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getPerspective();    

    mainWindow->getUi()->actionTop->setChecked(false);
    mainWindow->getUi()->actionBottom->setChecked(false);
    mainWindow->getUi()->actionFront->setChecked(false);
    mainWindow->getUi()->actionBack->setChecked(false);
    mainWindow->getUi()->actionLeft->setChecked(false);
    mainWindow->getUi()->actionRight->setChecked(false);
    mainWindow->getUi()->actionFly->setChecked(false);

    cameraPosition = position;

    if (camera)
    {
        if (cameraPosition == eCAMERA_POSITION_FLY)
        {
            flyPosition = camera->getPosition();
            flyRotation = camera->getRotationEuler();
        }

        switch (cameraPosition)
        {
            case MGE::eCAMERA_POSITION_TOP:
            {
                camera->setPosition(cameraTop);
                camera->setRotationEuler(Magic3D::Vector3(90.0f, 180.0f, 0.0f));
                mainWindow->getUi()->actionTop->setChecked(true);
                break;
            }
            case MGE::eCAMERA_POSITION_BOTTOM:
            {
                camera->setPosition(cameraBottom);
                camera->setRotationEuler(Magic3D::Vector3(-90.0f, 180.0f, 0.0f));
                mainWindow->getUi()->actionBottom->setChecked(true);
                break;
            }
            case MGE::eCAMERA_POSITION_FRONT:
            {
                camera->setPosition(cameraFront);
                camera->setRotationEuler(Magic3D::Vector3(0.0f, 180.0f, 0.0f));
                mainWindow->getUi()->actionFront->setChecked(true);
                break;
            }
            case MGE::eCAMERA_POSITION_BACK:
            {
                camera->setPosition(cameraBack);
                camera->setRotationEuler(Magic3D::Vector3(0.0f, 0.0f, 0.0f));
                mainWindow->getUi()->actionBack->setChecked(true);
                break;
            }
            case MGE::eCAMERA_POSITION_LEFT:
            {
                camera->setPosition(cameraLeft);
                camera->setRotationEuler(Magic3D::Vector3(0.0f, -90.0f, 0.0f));
                mainWindow->getUi()->actionLeft->setChecked(true);
                break;
            }
            case MGE::eCAMERA_POSITION_RIGHT:
            {
                camera->setPosition(cameraRight);
                camera->setRotationEuler(Magic3D::Vector3(0.0f, 90.0f, 0.0f));
                mainWindow->getUi()->actionRight->setChecked(true);
                break;
            }
            case MGE::eCAMERA_POSITION_FLY:
            {
                camera->setPosition(flyPosition);
                camera->setRotationEuler(flyRotation);
                mainWindow->getUi()->actionFly->setChecked(true);
                break;
            }
        }
    }
}

MGE::CAMERA_POSITION MGE::Magic3DWidget::getCameraPosition()
{
    return cameraPosition;
}

QPoint MGE::Magic3DWidget::getLastMenuPos()
{
    return lastMenuPos;
}

void MGE::Magic3DWidget::initializeGL()
{
    if (Magic3D::Magic3D::start(UTILS_DEFAULT_PATH.toStdString(), logCallBack) && Magic3D::Renderer::getInstance()->createWindow(true))
    {
        Magic3D::Magic3D::play();
        Magic3D::Physics::getInstance()->stop(false);
        Magic3D::Script::getInstance()->stop(false);
        /*Magic3D::Magic3D::getInstance()->getRenderer()->removeAllViewPorts();
    Magic3D::Magic3D::getInstance()->getRenderer()->addViewPort(Magic3D::ViewPort(Magic3D::Vector4(0.0f, 0.5f, 0.5f, 0.5f), 0, 0));
    Magic3D::Magic3D::getInstance()->getRenderer()->addViewPort(Magic3D::ViewPort(Magic3D::Vector4(0.5f, 0.5f, 0.5f, 0.5f), 0, 0));
    Magic3D::Magic3D::getInstance()->getRenderer()->addViewPort(Magic3D::ViewPort(Magic3D::Vector4(0.0f, 0.0f, 0.5f, 0.5f), 0, 0));
    Magic3D::Magic3D::getInstance()->getRenderer()->addViewPort(Magic3D::ViewPort(Magic3D::Vector4(0.5f, 0.0f, 0.5f, 0.5f), 0, 0));*/

        QSettings settings;

        Magic3D::Scene::getInstance()->showFPSText(settings.value("showFPS").toBool());
        Magic3D::Scene::getInstance()->showINFOText(settings.value("showINFO").toBool());
        Magic3D::Scene::getInstance()->showWINDOWText(settings.value("showWINDOW").toBool());

        Magic3D::Renderer::getInstance()->setShowGizmos(settings.value("showGizmos").toBool());
        Magic3D::Renderer::getInstance()->setShowGizmosPhysics(settings.value("showGizmosPhysics").toBool());
        Magic3D::Renderer::getInstance()->setShowOctree(settings.value("showOctree").toBool());
        Magic3D::Renderer::getInstance()->setShowOctreeObjects(settings.value("showOctreeObjects").toBool());
    }
}

void MGE::Magic3DWidget::resizeGL(int w, int h)
{
    Magic3D::Renderer::getInstance()->resizeWindow(w, h);
    //updateInfo();

    mainWindow->cursor.setText(tr("width: %0 - height: %1 ").arg(w).arg(h));
}

/*void MGE::Magic3DWidget::paintGL()
{
    updateCamera();
    Magic3D::Magic3D::run();
}*/

void MGE::Magic3DWidget::paintEvent(QPaintEvent *event)
{
    if (event)
    {

    }

    if (translateCamera.isPlaying())
    {
        translateCamera.update();
        Magic3D::Vector3* cam = getCameraVector();
        *cam = translateCamera.getPhysicsObject()->getPosition();
    }
    else
    {
        updateCamera();
    }
    makeCurrent();

    if (!mainWindow->getProject())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);

    Magic3D::Magic3D::run();
    /*if (mainWindow && Magic3D::Renderer::getInstance()->lblINFO)
    {
        mainWindow->info.setText(QString::fromStdString(Magic3D::Renderer::getInstance()->lblINFO->getText()));
    }*/

    QPainter painter(this);

    painter.end();
    return;
}

Magic3D::Vector3* MGE::Magic3DWidget::getCameraVector()
{
    Magic3D::Vector3* cam = &flyPosition;
    switch (getCameraPosition())
    {
        case eCAMERA_POSITION_TOP:    cam = &cameraTop;    break;
        case eCAMERA_POSITION_BOTTOM: cam = &cameraBottom; break;
        case eCAMERA_POSITION_FRONT:  cam = &cameraFront;  break;
        case eCAMERA_POSITION_BACK:   cam = &cameraBack;   break;
        case eCAMERA_POSITION_LEFT:   cam = &cameraLeft;   break;
        case eCAMERA_POSITION_RIGHT:  cam = &cameraRight;  break;
        default: break;
    }

    return cam;
}

void MGE::Magic3DWidget::updateCamera(float factor, bool force)
{
    Magic3D::Vector3* cam = getCameraVector();

    Magic3D::ViewPort* viewport = Magic3D::Renderer::getInstance()->getCurrentViewPort();
    Magic3D::Camera* camera = viewport->getPerspective();

    if (keyUp || keyDown || keyLeft || keyRight)
    {
        if (camera)
        {
            //float speed = Magic3D::Magic3D::getInstance()->getTicks() - elapsed;
            float speed = Magic3D::Magic3D::getInstance()->getElapsedTimeReal();
            float value = mainWindow->cameraoptions->getSpeedMove() * speed;

            if (keyShift && !keyCtrl)
            {
                value *= 4.0f;
            }

            if (keyCtrl && !keyShift)
            {
                value *= 0.25f;
            }

            if (keyUp)
            {
                *cam += camera->getDirectionFront() * value;
            }
            if (keyDown)
            {
                *cam += camera->getDirectionBack() * value;
            }
            if (keyLeft)
            {
                *cam += camera->getDirectionLeft() * value;
            }
            if (keyRight)
            {
                *cam += camera->getDirectionRight() * value;
            }

            camera->setPosition(*cam);
            mainWindow->object3dinfo->updateSimulation();

            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
    }
    else if (factor != 0.0f)
    {
        if (camera)
        {
            *cam += camera->getDirectionFront() * factor;

            camera->setPosition(*cam);
            mainWindow->object3dinfo->updateSimulation();

            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
    }

    if (force)
    {
        if (camera)
        {
            camera->setPosition(*cam);
            mainWindow->object3dinfo->updateSimulation();

            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
    }
}

void MGE::Magic3DWidget::timerEvent(QTimerEvent *)
{
    mainWindow->setWindowTitle(tr("%0 (%1 fps)").arg(TITLE + tr(MAGIC3D_VERSION)).arg(Magic3D::Magic3D::getInstance()->getFPS()));
    //mainWindow->setScriptObjectsText(tr("Script Objects: %0").arg(Magic3D::Script::scriptObjects));

    update();
    //repaint();

    if (loading && !Magic3D::Scene::getInstance()->isLoading())
    {
        loading = false;

        Magic3D::Physics::getInstance()->stop(false);
        Magic3D::Script::getInstance()->stop(false);
        mainWindow->update();
        mainWindow->updateFromEngine();
        mainWindow->getUi()->actionSimulate->setEnabled(true);

        mainWindow->setProjectChanged(false);
        mainWindow->update();

        mainWindow->setLoading(false);
        mainWindow->updateHelp();

        mainWindow->object2dinfo->update();
        mainWindow->object3dinfo->update();
        mainWindow->boneinfo->update();

        mainWindow->materialinfo->update();
        mainWindow->shaderseditor->update();

        setCameraPosition(eCAMERA_POSITION_FLY);
    }
    else
    {
        if (Magic3D::Physics::getInstance()->isPlaying())
        {
            mainWindow->object2dinfo->updateSimulation();
            mainWindow->object3dinfo->updateSimulation();
            mainWindow->boneinfo->updateSimulation();
        }

        Magic3D::Model* model = mainWindow->skeletoninfo->getModel();
        if (model && model->isPlaying())
        {
            mainWindow->skeletoninfo->updateSimulation();
        }
    }
}

void MGE::Magic3DWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (event)
    {

    }
    /*if (mainWindow->IsProjectOpened() && !mouseMoved)
    {
    lastMenuPos = mapFromGlobal(QCursor::pos());
    QMenu menu(this);
    menu.addAction(mainWindow->getUi()->menuAdd->menuAction());
    if (getSelectedObject())
    {
        menu.addSeparator();
        menu.addAction(mainWindow->getUi()->actionRemoveObject);
            if (getSelectedObject()->getType() == Magic3D::eOBJECT_MODEL || getSelectedObject()->getType() == Magic3D::eOBJECT_PARTICLES)
            {
                menu.addAction(mainWindow->getUi()->actionCloneObject);
            }
    }
    menu.exec(event->globalPos());
    }*/
}

void MGE::Magic3DWidget::setPath(std::string path)
{
    Magic3D::ResourceManager::getInstance()->setPath(path);
}

bool MGE::Magic3DWidget::addLayer(std::string name)
{
    Magic3D::Scene* scene = Magic3D::Scene::getInstance();

    bool result = scene->getLayer(name) == NULL;

    if (result)
    {
        scene->addLayer(new Magic3D::Layer(name));
    }

    return result;
}

Magic3D::Layer* MGE::Magic3DWidget::getLayer(std::string name)
{
    return Magic3D::Scene::getInstance()->getLayer(name);
}

void MGE::Magic3DWidget::removeLayer(std::string name)
{
    Magic3D::Layer* layer = Magic3D::Scene::getInstance()->getLayer(name);
    if (layer)
    {
        std::vector<Magic3D::Object*>* objects = layer->getObjects2D();

        while (!objects->empty())
        {
            Magic3D::Object* object = *objects->begin();
            objects->erase(objects->begin());
            Magic3D::ResourceManager::getObjects()->remove(object->getName());
        }

        objects = layer->getObjects3D();

        while (!objects->empty())
        {
            Magic3D::Object* object = *objects->begin();
            objects->erase(objects->begin());
            Magic3D::ResourceManager::getObjects()->remove(object->getName());
        }
    }
    Magic3D::Scene::getInstance()->removeLayer(name);
}

Magic3D::Object* MGE::Magic3DWidget::getObject(std::string name)
{
    return Magic3D::ResourceManager::getObjects()->get(name);
}

void MGE::Magic3DWidget::removeObject(std::string layer, std::string name)
{
    if (getCopiedObject() && name.compare(getCopiedObject()->getName()) == 0)
    {
        setCopiedObject(NULL);
    }

    Magic3D::Scene::getInstance()->removeObject(layer, name);
    Magic3D::ResourceManager::getObjects()->remove(name);
}

bool MGE::Magic3DWidget::renameObject(std::string name, std::string newName)
{
    bool result = false;
    Magic3D::Object* obj = Magic3D::ResourceManager::getObjects()->get(name);
    if (obj)
    {
        result = obj->setName(newName);
    }

    return result;
}

bool MGE::Magic3DWidget::cloneObject(Magic3D::Object* object, std::string name, std::string layer)
{
    bool result = false;
    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* obj = mngr->getObjects()->get(name);

    if (!obj && l)
    {
        Magic3D::Object* tmp = object;
        if (tmp->getType() == Magic3D::eOBJECT_MODEL || tmp->getType() == Magic3D::eOBJECT_PARTICLES)
        {
            /*obj = new Magic3D::Clone(object, name);

            mngr->getObjects()->add(obj);
            l->addObject(obj);*/

            result = true;
        }
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::spawnObject(Magic3D::Object* object, std::string name, std::string layer)
{
    keyCtrl = false;
    Magic3D::Object* result = NULL;
    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* obj = mngr->getObjects()->get(name);

    if (!obj && l)
    {
        result = object->spawn(name, l->getName(), true);
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

void MGE::Magic3DWidget::objectAlreadyExists(std::string name)
{
    Utils::message(tr("Invalid Name"), tr("Object named \"") + tr(name.c_str()) + tr("\" already exists!"));
}

Magic3D::Object* MGE::Magic3DWidget::addSprite(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addSprite(name, created);
        ((Magic3D::Sprite*)object)->addMaterial(Magic3D::ResourceManager::getMaterials()->get(M3D_DEFAULT_MATERIAL));
        Magic3D::Vector3 position = getPosition2D(pos);
        ((Magic3D::Sprite*)object)->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addParticles(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addParticles(name, created);

        Magic3D::Vector3 position = getPosition3D(pos);
        object->setPosition(position);
        ((Magic3D::Particles*)object)->play();

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addText(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addGUILabel(name, 32, created);
        Magic3D::Vector3 position = getPosition2D(pos);
        ((Magic3D::GUILabel*)object)->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));
        ((Magic3D::GUILabel*)object)->setText("Placeholder");

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addWindow(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addGUIWindow(name, created);
        Magic3D::Vector3 position = getPosition2D(pos);
        ((Magic3D::GUIWindow*)object)->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addButton(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addGUIButton(name, created);
        Magic3D::Vector3 position = getPosition2D(pos);
        ((Magic3D::GUIButton*)object)->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addSlider(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addGUILabel(name, 32, created);
        Magic3D::Vector3 position = getPosition2D(pos);
        ((Magic3D::GUILabel*)object)->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));
        ((Magic3D::GUILabel*)object)->setText("Placeholder");

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addModel(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addModel(name, created);
        Magic3D::Vector3 position = getPosition3D(pos);
        object->setPosition(position);

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addInstance(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addInstance(name, created);
        Magic3D::Vector3 position = getPosition3D(pos);
        object->setPosition(position);

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addLight(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addLight(name, created);
        Magic3D::Vector3 position = getPosition3D(pos);
        object->setPosition(position);

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addSound(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addSound(name, created);
        Magic3D::Vector3 position = getPosition3D(pos);
        object->setPosition(position);

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addCamera(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addCamera(name, created);
        object->setPosition(Magic3D::Vector3(0.0f, 0.0f, 0.0f));
        object->setRotationEuler(Magic3D::Vector3(0.0f, 180.0f, 0.0f));

        if (pos.isNull())
        {

        }

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Object* MGE::Magic3DWidget::addJoystick(std::string name, std::string layer, QPoint pos)
{
    Magic3D::Object* result = NULL;

    Magic3D::ResourceManager* mngr = Magic3D::ResourceManager::getInstance();
    Magic3D::Layer* l = Magic3D::Scene::getInstance()->getLayer(layer);

    Magic3D::Object* object = mngr->getObjects()->get(name);

    if (!object && l)
    {
        bool created = false;
        object = mngr->addJoystick(name, created);
        Magic3D::Vector3 position = getPosition2D(pos);
        ((Magic3D::Joystick*)object)->setPosition(Magic3D::Vector3(position.getX(), position.getY(), 0.0f));

        Magic3D::Scene::getInstance()->addObject(l, object);

        result = object;
    }
    else
    {
        objectAlreadyExists(name);
    }

    return result;
}

Magic3D::Vector3 MGE::Magic3DWidget::getPosition2D(QPoint pos)
{
    Magic3D::Vector3 result = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    Magic3D::ViewPort* viewport = Magic3D::Renderer::getInstance()->getCurrentViewPort();
    Magic3D::Camera* camera = viewport->getOrthographic();
    Magic3D::Window* window = Magic3D::Renderer::getInstance()->getWindow();
    Magic3D::Vector3 aspect = window->getWindowScreenAspect();

    if (camera)
    {
        result = Magic3D::Vector3(camera->getPosition().getX() + (pos.x() / (float)window->getWidth()) * aspect.getX(), camera->getPosition().getY() + (pos.y() / (float)window->getHeight()) * aspect.getY(), 0.0f);
    }

    return result;

}

Magic3D::Vector3 MGE::Magic3DWidget::getPosition3D(QPoint pos)
{
    Magic3D::Vector3 result = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    Magic3D::ViewPort* viewport = Magic3D::Renderer::getInstance()->getCurrentViewPort();
    Magic3D::Camera* camera = viewport->getPerspective();

    if (camera)
    {
        result = camera->getPosition3D((float)pos.x(), (float)pos.y(), 10.0f, viewport);
    }

    return result;

}

void MGE::Magic3DWidget::setSelection(Magic3D::Object* object, Magic3D::Bone* bone)
{
    if (this->selected)
    {
        this->selected->setSelected(false);
        this->selected->setShowGrid(false);
    }

    if (this->selectedBone)
    {
        this->selectedBone->setSelected(false);
    }

    this->selected = object;
    this->selectedBone = bone;

    mainWindow->object2dinfo->setPhysicsObject(this->selected);
    mainWindow->object3dinfo->setPhysicsObject(this->selected);
    mainWindow->boneinfo->setPhysicsObject(this->selectedBone);
    if (this->selectedBone)
    {
        mainWindow->tweensinfo->setPhysicsObject(this->selectedBone);
        mainWindow->physicsinfo->setPhysicsObject(this->selectedBone);
    }
    else
    {
        mainWindow->tweensinfo->setPhysicsObject(this->selected);
        mainWindow->physicsinfo->setPhysicsObject(this->selected);
        mainWindow->physicsinfo->updateMeshCombo();
    }
    mainWindow->skeletoninfo->setPhysicsObject(this->selected);
    mainWindow->meshesinfo->setPhysicsObject(this->selected);
    mainWindow->particlesinfo->setPhysicsObject(this->selected);
    mainWindow->materialinfo->setPhysicsObject(this->selected);
    mainWindow->shaderseditor->setPhysicsObject(this->selected);

    if (this->selected)
    {
        this->selected->setSelected(true);
        this->selected->setShowGrid(true);
    }

    if (this->selectedBone)
    {
        this->selectedBone->setSelected(true);
    }

    if (this->selected || this->selectedBone)
    {
        updateInfo();
    }

    bool onlyObject = this->selected && !this->selectedBone;
    mainWindow->object2dinfo->setVisible(onlyObject && this->selected->getRender() == Magic3D::eRENDER_2D);
    mainWindow->object3dinfo->setVisible(onlyObject && this->selected->getRender() == Magic3D::eRENDER_3D);
    mainWindow->boneinfo->setVisible(this->selectedBone);
    mainWindow->physicsinfo->setVisible(this->selectedBone || (this->selected && this->selected->getType() != Magic3D::eOBJECT_JOYSTICK));
    mainWindow->skeletoninfo->setVisible(this->selected && this->selected->getType() == Magic3D::eOBJECT_MODEL);
    mainWindow->tweensinfo->setVisible(this->selectedBone || (this->selected));
    mainWindow->meshesinfo->setVisible(onlyObject);
    mainWindow->particlesinfo->setVisible(onlyObject && this->selected->getType() == Magic3D::eOBJECT_PARTICLES);

    if (mainWindow->getProject())
    {
        mainWindow->getProject()->setSelectedObject(this->selected ? this->selected->getName() : "");
        mainWindow->getProject()->setSelectedBone(this->selectedBone ? this->selectedBone->getName() : "");
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

Magic3D::PhysicsObject* MGE::Magic3DWidget::getSelectedPhysicsObject()
{
    Magic3D::PhysicsObject* result = selected;
    if (selectedBone)
    {
        result = selectedBone;
    }
    return result;
}

Magic3D::Object* MGE::Magic3DWidget::getSelectedObject()
{
    return selected;
}

Magic3D::Bone* MGE::Magic3DWidget::getSelectedBone()
{
    return selectedBone;
}

void MGE::Magic3DWidget::setCopiedObject(Magic3D::Object* copied)
{
    this->copied = copied;
}

Magic3D::Object* MGE::Magic3DWidget::getCopiedObject()
{
    return copied;
}

void MGE::Magic3DWidget::updateInfo()
{
    if (selectedBone)
    {
        mainWindow->boneinfo->update();
        mainWindow->physicsinfo->update();
        mainWindow->skeletoninfo->update();
        mainWindow->tweensinfo->update();
    }
    else
    {
        mainWindow->object2dinfo->update();
        mainWindow->object3dinfo->update();
        mainWindow->physicsinfo->update();
        mainWindow->skeletoninfo->update();
        mainWindow->tweensinfo->update();
        mainWindow->meshesinfo->update();
        mainWindow->particlesinfo->update();
    }
}

void MGE::Magic3DWidget::keyPressEvent(QKeyEvent * event)
{
    if (Magic3D::Script::getInstance()->isPlaying())
    {
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_KEYBOARD, Magic3D::eEVENT_KEYBOARD_DOWN, event->key());
    }
    else
    {
        switch(event->key())
        {
            case Qt::Key_W: keyUp    = true; break;
            case Qt::Key_S: keyDown  = true; break;
            case Qt::Key_A: keyLeft  = true; break;
            case Qt::Key_D: keyRight = true; break;
            case Qt::Key_Control: keyCtrl = true; break;
            case Qt::Key_Shift: keyShift = true; break;
            case Qt::Key_F12: {
                QFile file(UTILS_DEFAULT_PATH + "mge.style");
                file.open(QFile::ReadOnly);
                QString styleSheet = QLatin1String(file.readAll());

                qApp->setStyleSheet(styleSheet);
                break;
            }
            case Qt::Key_F5: {

                break;
            }

            case Qt::Key_Delete:
            {
                if (getSelectedObject())
                {
                    mainWindow->getUi()->actionRemoveObject->trigger();
                }
                break;
            }

            case Qt::Key_X: setTRS(!isTRSX(), isTRSY(), isTRSZ()); break;
            case Qt::Key_Y: setTRS(isTRSX(), !isTRSY(), isTRSZ()); break;
            case Qt::Key_Z: setTRS(isTRSX(), isTRSY(), !isTRSZ()); break;

            case Qt::Key_Escape:
            {
                if (mouseDown && mouseMoved && getSelectedPhysicsObject())
                {
                    mouseDown = false;

                    getSelectedPhysicsObject()->setPosition(objectRestore.position);
                    getSelectedPhysicsObject()->setRotation(objectRestore.rotation);
                    getSelectedPhysicsObject()->setScale(objectRestore.scale);

                    getSelectedPhysicsObject()->resetPhysics();

                    mainWindow->object2dinfo->update();
                    mainWindow->object3dinfo->update();
                    mainWindow->boneinfo->update();

                    mainWindow->setProjectChanged(objectRestore.changed);
                    mainWindow->update();
                }
                break;
            }
        }
    }
}

void MGE::Magic3DWidget::keyReleaseEvent(QKeyEvent * event)
{
    if (Magic3D::Script::getInstance()->isPlaying())
    {
        if (!event->isAutoRepeat())
        {
            Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_KEYBOARD, Magic3D::eEVENT_KEYBOARD_UP, event->key());
        }
    }
    else
    {
        switch(event->key())
        {
            case Qt::Key_W: keyUp    = false; break;
            case Qt::Key_S: keyDown  = false; break;
            case Qt::Key_A: keyLeft  = false; break;
            case Qt::Key_D: keyRight = false; break;
            case Qt::Key_Control: keyCtrl = false; break;
            case Qt::Key_Shift: keyShift = false; break;
        }
    }
}

void MGE::Magic3DWidget::resetKeys()
{
    keyUp    = false;
    keyDown  = false;
    keyLeft  = false;
    keyRight = false;
    keyCtrl = false;
    keyShift = false;
}

void MGE::Magic3DWidget::mousePressEvent(QMouseEvent *event)
{
    mouseDown = true;
    mouseMoved = false;
    float x = event->pos().x();
    float y = event->pos().y();

    int button = 1;

    switch (event->button())
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::RightButton: button = 2; break;
        case Qt::MiddleButton: button = 3; break;
        default: button = 1; break;
    }

    if (Magic3D::Script::getInstance()->isPlaying())
    {
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_MOUSE, Magic3D::eEVENT_MOUSE_DOWN, (int)x, (int)y, button);
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_TOUCH, Magic3D::eEVENT_TOUCH_DOWN, (int)x, (int)y, button);
    }
    else
    {
        if (event->button() == Qt::LeftButton)
        {
            if (!raycast(x, y, false))
            {
                if (!raycast(x, y, true))
                {
                    mainWindow->selectObjectByName(lastLayer, false);
                }
            }

            if (getSelectedObject() && keyCtrl)
            {
                Magic3D::Object* obj = getSelectedObject();
                //mainWindow->duplicateObject(QString::fromStdString(obj->getCloneName()), getSelectedObject(), true);
                mainWindow->duplicateObject(QString::fromStdString(Magic3D::ResourceManager::getObjects()->getNewName(obj->getName())), obj, true);
            }
        }

        if (getSelectedPhysicsObject() && !keyCtrl)
        {
            objectRestore.position = getSelectedPhysicsObject()->getPosition();
            objectRestore.rotation = getSelectedPhysicsObject()->getRotation();
            objectRestore.scale = getSelectedPhysicsObject()->getScale();
            objectRestore.changed = mainWindow->getProject()->isChanged();

            if (getSelectedObject()->getType() != Magic3D::eOBJECT_LIGHT && getSelectedObject()->getType() != Magic3D::eOBJECT_SOUND)
            {
                getSelectedObject()->setSelected(false);
            }
        }
    }

    selectedOffset = QPointF(x, y);
}

bool MGE::Magic3DWidget::raycast(float x, float y, bool perspective)
{
    bool result = false;

    Magic3D::ViewPort* viewport = Magic3D::Renderer::getInstance()->getCurrentViewPort();
    Magic3D::Camera* camera = perspective ? viewport->getPerspective() : viewport->getOrthographic();

    /*Magic3D::Window* window = Magic3D::Renderer::getInstance()->getWindow();

    if (camera && window)
    {

        std::list<Magic3D::RenderObject>* objs;

        Magic3D::Matrix4 projection;
        if (perspective)
        {
            projection = camera->view3D((float)window->getWidth() / (float)window->getHeight());
            Magic3D::Scene::getInstance()->updateVisibleObjects3D(camera, true, true);
            objs = Magic3D::Scene::getInstance()->getVisibleObjects3D();
        }
        else
        {
            projection = camera->view2D(0.0f, window->getWidth(), window->getHeight(), 0.0f);
            Magic3D::Scene::getInstance()->updateVisibleObjects2D(camera, false);
            objs = Magic3D::Scene::getInstance()->getVisibleObjects2D();
        }


        float mx = x;
        float my = -y;

        if (perspective)
        {
            mx = x / window->getWidth();
            my = 1.0f - y / window->getHeight();
        }

        std::list<Magic3D::RenderObject>::const_iterator it_o = objs->end();

        while (it_o != objs->begin() && !result)
        {
            const Magic3D::RenderObject* object = &(*--it_o);
            Magic3D::Box box = object->object->getBoundingBox();

            Magic3D::Matrix4 matrix = camera->getView() * object->object->getMatrixFromParent();

            Magic3D::Vector3 n = Magic3D::Camera::unproject(Magic3D::Vector3(mx, my, 0.0f), matrix, projection, viewport);
            Magic3D::Vector3 f = Magic3D::Camera::unproject(Magic3D::Vector3(mx, my, 1.0f), matrix, projection, viewport);

            Magic3D::Vector3 dir = f - n;
            dir = normalize(dir);

            Magic3D::Vector4 mouse = inverse(matrix) * Magic3D::Vector4(mx, -my, object->object->getMaxSizeFromParent(), 1.0f);
            Magic3D::Vector3 pos = Magic3D::Vector3(mouse.getX(), mouse.getY(), mouse.getZ());
            if (perspective)
            {
                pos = n;
            }

            Magic3D::Ray ray = Magic3D::Ray(pos, dir);

            if (Magic3D::Math::collide(ray, camera->getNear(), camera->getFar(), box))
            {
                QString name = QString::fromStdString(object->object->getName());
                mainWindow->selectObjectByName(name, true);
                result = true;
            }
        }
    }*/

    if (camera)
    {
        Magic3D::Object* obj = camera->pick(x, y, 0, true);
        if (obj)
        {
            QString name = QString::fromStdString(obj->getName());
            mainWindow->selectObjectByName(name, true);
            result = true;
        }
    }

    return result;
}

void MGE::Magic3DWidget::mouseMoveEvent(QMouseEvent *event)
{
    bool moving = mouseMoved;
    mouseMoved = mouseDown;

    if (Magic3D::Renderer::getInstance()->getWindow())
    {
        Magic3D::Renderer::getInstance()->getWindow()->setCursorPosition(event->pos().x(), event->pos().y());
    }

    Magic3D::Object* object = getSelectedObject();
    bool centerCursor = false;

    float x = event->pos().x() - selectedOffset.x();
    float y = event->pos().y() - selectedOffset.y();

    bool updateOffset = true;

    int button = 1;

    switch (event->button())
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::RightButton: button = 2; break;
        case Qt::MiddleButton: button = 3; break;
        default: button = 1; break;
    }

    if (Magic3D::Script::getInstance()->isPlaying())
    {
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_MOUSE, Magic3D::eEVENT_MOUSE_MOVE, (int)event->pos().x(), (int)event->pos().y(), button);
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_TOUCH, Magic3D::eEVENT_TOUCH_MOVE, (int)event->pos().x(), (int)event->pos().y(), button);
    }
    else
    {
        Magic3D::Camera* camera = NULL;

        Magic3D::Vector3 up;
        Magic3D::Vector3 front;
        Magic3D::Vector3 right;

        Magic3D::Quaternion objRot = Magic3D::Quaternion::identity();

        if (object)
        {
            Magic3D::ViewPort* viewport = Magic3D::Renderer::getInstance()->getCurrentViewPort();
            camera = object->getRender() == Magic3D::eRENDER_3D ? viewport->getPerspective() : viewport->getOrthographic();

            if (getSelectedBone())
            {
                objRot = object->getRotationFromParent();
                if (getSelectedBone()->getParent())
                {
                    objRot *= getSelectedBone()->getParent()->getRotationFromParent();
                }

                objRot = Magic3D::Math::inverse(objRot);
            }
            else
            {
                if (object->getParent())
                {
                    objRot = Magic3D::Math::inverse(object->getParent()->getRotationFromParent());
                }
            }

            if (camera && mainWindow->getUi()->rbAxisCamera->isChecked())
            {
                up = camera->getDirectionUp();
                front = camera->getDirectionFront();
                right = camera->getDirectionRight();
            }
            else if (!camera || mainWindow->getUi()->rbAxisGlobal->isChecked())
            {
                up = Magic3D::Vector3(0.0f, 1.0f, 0.0f);
                front = Magic3D::Vector3(0.0f, 0.0f, 1.0f);
                right = Magic3D::Vector3(1.0f, 0.0f, 0.0f);
            }
            else if (mainWindow->getUi()->rbAxisLocal->isChecked())
            {
                if (getSelectedBone())
                {
                    up = getSelectedBone()->getMatrix().getCol1().getXYZ();
                    front = getSelectedBone()->getMatrix().getCol2().getXYZ();
                    right = cross(front, up);
                }
                else
                {
                    up = object->getDirectionUp();
                    front = object->getDirectionFront();
                    right = object->getDirectionRight();
                }
            }

            if (!mainWindow->getUi()->rbAxisLocal->isChecked())
            {
                up    = rotate(objRot, up);
                front = rotate(objRot, front);
                right = rotate(objRot, right);
            }
        }

        if (mouseDown && object && event->buttons() & Qt::LeftButton && event->buttons() & Qt::RightButton)
        {
            float max  = (fabs(y) > fabs(x) ? y : x) * 0.01f;

            float x = object->getScale().getX() + (trsX ? max : 0.0f);
            float y = object->getScale().getY() + (trsY ? max : 0.0f);
            float z = object->getScale().getZ() + (trsZ ? max : 0.0f);
            if (object->getRender() == Magic3D::eRENDER_2D)
            {
                z = 1.0f;
            }

            object->setScale(Magic3D::Vector3(x, y, z));

            mainWindow->object2dinfo->updateSimulation();
            mainWindow->object3dinfo->updateSimulation();
            mainWindow->boneinfo->update();

            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
        else if (mouseDown && object && object->getType() != Magic3D::eOBJECT_CAMERA && event->buttons() & Qt::MiddleButton)
        {
            Magic3D::Quaternion quat = Magic3D::Quaternion::identity();

            if (camera && object->getRender() == Magic3D::eRENDER_2D)
            {
                quat = Magic3D::Quaternion::rotation(Magic3D::Math::radians(-x), camera->getDirectionFront());
            }
            else
            {
                float max = fabs(x) > fabs(y) ? x : y;
                if (trsX)
                {
                    /*float angle;
                    Magic3D::Vector3 axis;

                    Magic3D::Math::toAxisAngle(camera->getRotation(), axis, angle);

                    axis = cross(axis, camera->getDirectionFront());
                    Magic3D::Quaternion q = Magic3D::Quaternion::rotation(angle, axis);

                    Magic3D::Vector4 rotationX = Magic3D::Matrix4(camera->getRotation(), Magic3D::Vector3(0.0f, 0.0f, 0.0f)) * Magic3D::Vector4(-x, 0.0f, 0.0f, 1.0f);
                    Magic3D::Vector4 rotationY = Magic3D::Matrix4(q, Magic3D::Vector3(0.0f, 0.0f, 0.0f)) * Magic3D::Vector4(0.0f, -y, 0.0f, 1.0f);

                    float rotation = 0.0f;
                    if (mainWindow->getUi()->rbAxisGlobal->isChecked())
                    {
                        rotation = fabs(rotationY.getY()) > fabs(rotationX.getZ()) ? rotationY.getY() : rotationX.getZ();
                    }
                    else if (mainWindow->getUi()->rbAxisLocal->isChecked())
                    {
                        rotation = fabs(rotationY.getY()) > fabs(rotationX.getZ()) ? rotationY.getY() : rotationX.getZ();
                    }
                    else
                    {
                        rotation = y;
                    }

                    q = Magic3D::Quaternion::rotation(Magic3D::Math::radians(rotation), Magic3D::Vector4(Magic3D::Vector4(right, 0.0f)).getXYZ());
                    quat = q*/;

                    float rotation = trsX && !trsY && !trsZ ? max : y;

                    Magic3D::Quaternion q = Magic3D::Quaternion::rotation(Magic3D::Math::radians(rotation), Magic3D::Vector4(Magic3D::Vector4(right, 0.0f)).getXYZ());
                    quat *= q;
                }
                if (trsY)
                {
                    float rotation = !trsX && trsY && !trsZ ? max : x;

                    Magic3D::Quaternion q = Magic3D::Quaternion::rotation(Magic3D::Math::radians(rotation), Magic3D::Vector4(Magic3D::Vector4(up, 0.0f)).getXYZ());
                    quat *= q;
                }
                if (trsZ && !trsY)
                {
                    float rotation = !trsX && !trsY && trsZ ? max : y;

                    Magic3D::Quaternion q = Magic3D::Quaternion::rotation(Magic3D::Math::radians(rotation), Magic3D::Vector4(Magic3D::Vector4(front, 0.0f)).getXYZ());
                    quat *= q;
                }
                /*else if (trsX && trsY && trsZ)
                {
                    quat = Magic3D::Quaternion::rotation(Magic3D::Math::radians(x), Magic3D::Vector4(objmtx * Magic3D::Vector4(camera->getDirectionUp(), 0.0f)).getXYZ()) * Magic3D::Quaternion::rotation(Magic3D::Math::radians(y), Magic3D::Vector4(objmtx * Magic3D::Vector4(camera->getDirectionRight(), 0.0f)).getXYZ());
                }*/
            }

            if (getSelectedBone())
            {
                Magic3D::Bone* bone = getSelectedBone();

                Magic3D::Matrix4 matrix = bone->getMatrix() * Magic3D::Matrix4(normalize(quat), Magic3D::Vector3(0.0f, 0.0f, 0.0f));

                bone->setMatrix(matrix);
            }
            else
            {
                object->setRotation(normalize(quat) * object->getRotation());
            }

            object->resetPhysics();

            mainWindow->object2dinfo->updateSimulation();
            mainWindow->object3dinfo->updateSimulation();
            mainWindow->boneinfo->update();

            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
        else if (event->buttons() & Qt::LeftButton && !(event->buttons() & Qt::RightButton))
        {
            if (mouseDown && object && !Magic3D::Physics::getInstance()->isPlaying())
            {
                Magic3D::Window* window = Magic3D::Renderer::getInstance()->getWindow();
                Magic3D::Vector3 aspect = window->getWindowScreenAspect();
                float x = (event->pos().x() - selectedOffset.x());
                float y = (event->pos().y() - selectedOffset.y());

                if (object->getRender() == Magic3D::eRENDER_2D)
                {
                    x = x / window->getWidth() * aspect.getX();
                    y = y / window->getHeight() * aspect.getY();
                    Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;

                    Magic3D::Matrix4 matrix = sprite->getMatrixFromParent();
                    //Magic3D::Vector3 finalPos = sprite->getPosition();
                    Magic3D::Vector3 finalPos = matrix.getTranslation();
                    Magic3D::Vector4 pos = Magic3D::Vector4(Magic3D::Vector3(x, y, 0.0f), 1.0f);

                    pos += Magic3D::Vector4(finalPos, 1.0f);

                    if (trsX)
                    {
                        finalPos.setX(pos.getX());
                    }
                    if (trsY)
                    {
                        finalPos.setY(pos.getY());
                    }

                    //Magic3D::Vector3 diff = (finalPos - sprite->getPosition());
                    Magic3D::Vector3 diff = (finalPos - matrix.getTranslation());

                    if (fabs(diff.getX()) >= mainWindow->getProject()->getGrid2DX() ||
                        fabs(diff.getY()) >= mainWindow->getProject()->getGrid2DY())
                    {
                        /*if (mainWindow->getProject()->getGrid2DX() > 0.0f)
                        {
                            diff.setX((int)(diff.getX() / mainWindow->getProject()->getGrid2DX()) * mainWindow->getProject()->getGrid2DX());
                        }
                        if (mainWindow->getProject()->getGrid2DY() > 0.0f)
                        {
                            diff.setY((int)(diff.getY() / mainWindow->getProject()->getGrid2DY()) * mainWindow->getProject()->getGrid2DY());
                        }

                        finalPos = sprite->getPosition() + diff;*/
                        finalPos += Magic3D::Vector3((finalPos.getX() > 0.0f ? 1.0 : -1.0f) * mainWindow->getProject()->getGrid2DX(), (finalPos.getY() > 0.0f ? 1.0 : -1.0f) * mainWindow->getProject()->getGrid2DY(), 0.0f) * 0.5f;

                        if (mainWindow->getProject()->getGrid2DX() > 0.0f)
                        {
                            finalPos.setX((int)(finalPos.getX() / mainWindow->getProject()->getGrid2DX()) * mainWindow->getProject()->getGrid2DX());
                        }
                        if (mainWindow->getProject()->getGrid2DY() > 0.0f)
                        {
                            finalPos.setY((int)(finalPos.getY() / mainWindow->getProject()->getGrid2DY()) * mainWindow->getProject()->getGrid2DY());
                        }

                        if (sprite->getParent())
                        {
                            Magic3D::Matrix4 inv = inverse(sprite->getParent()->getMatrixFromParent());
                            finalPos = (inv * Magic3D::Vector4(finalPos, 1.0f)).getXYZ();
                        }
                        sprite->setPosition(finalPos);
                        //sprite->setPosition((inverse(matrix) * Magic3D::Vector4(finalPos, 1.0f)).getXYZ());
                    }
                    else
                    {
                        updateOffset = false;
                    }

                    //sprite->setPosition(sprite->getX() + finalPos.getX(), sprite->getY() + finalPos.getY());
                    //sprite->setPosition(sprite->getX() + pos.getX(), sprite->getY() + pos.getY());
                    mainWindow->object2dinfo->updateSimulation();
                }
                else
                {
                    Magic3D::ViewPort* viewport = Magic3D::Renderer::getInstance()->getCurrentViewPort();
                    Magic3D::Camera* camera = viewport->getPerspective();
                    Magic3D::Window* window = Magic3D::Renderer::getInstance()->getWindow();

                    if (camera && window)
                    {
                        Magic3D::Matrix4 projection = camera->view3D((float)window->getWidth() / (float)window->getHeight());
                        Magic3D::Matrix4 matrix = camera->getView();

                        Magic3D::Vector3 finalPos = object->getPosition();

                        if (object->getParent())
                        {
                            matrix *= object->getParent()->getMatrixFromParent();
                        }

                        if (object->getParentBone())
                        {
                            matrix *= object->getParentBone()->getMatrixFromParent();
                        }

                        Magic3D::Vector3 pos;
                        pos = Magic3D::Camera::project(object->getPosition(), matrix, projection, viewport);

                        float sX = (pos.getX() * window->getWidth() + x) / window->getWidth();
                        float sY = 1.0f - (pos.getY() * window->getHeight() + y) / window->getHeight();

                        pos = Magic3D::Camera::unproject(Magic3D::Vector3(sX, sY, pos.getZ()), matrix, projection, viewport);

                        Magic3D::Vector3 diff = pos - object->getPosition();

                        float max = Magic3D::Math::maxAbs(Magic3D::Math::maxAbs(diff.getX(), diff.getY()), diff.getZ());

                        if (trsX)
                        {
                            if (mainWindow->getUi()->rbAxisCamera->isChecked())
                            {
                                finalPos.setX(pos.getX());

                            }
                            else
                            {
                                finalPos += right * max;
                            }
                        }
                        if (trsY)
                        {
                            if (mainWindow->getUi()->rbAxisCamera->isChecked())
                            {
                                finalPos.setY(pos.getY());

                            }
                            else
                            {
                                finalPos += up * max;
                            }
                        }
                        if (trsZ)
                        {
                            if (mainWindow->getUi()->rbAxisCamera->isChecked())
                            {
                                finalPos.setZ(pos.getZ());

                            }
                            else
                            {
                                finalPos += front * max;
                            }
                        }

                        diff = finalPos - object->getPosition();

                        if (fabs(diff.getX()) >= mainWindow->getProject()->getGrid3DX() ||
                            fabs(diff.getY()) >= mainWindow->getProject()->getGrid3DY() ||
                            fabs(diff.getZ()) >= mainWindow->getProject()->getGrid3DZ())
                        {
                            if (mainWindow->getProject()->getGrid3DX() > 0.0f)
                            {
                                diff.setX((int)(diff.getX() / mainWindow->getProject()->getGrid3DX()) * mainWindow->getProject()->getGrid3DX());
                            }
                            if (mainWindow->getProject()->getGrid3DY() > 0.0f)
                            {
                                diff.setY((int)(diff.getY() / mainWindow->getProject()->getGrid3DY()) * mainWindow->getProject()->getGrid3DY());
                            }
                            if (mainWindow->getProject()->getGrid3DZ() > 0.0f)
                            {
                                diff.setZ((int)(diff.getZ() / mainWindow->getProject()->getGrid3DZ()) * mainWindow->getProject()->getGrid3DZ());
                            }

                            finalPos = object->getPosition() + diff;

                            if (mainWindow->getProject()->getGrid3DX() > 0.0f)
                            {
                                finalPos.setX((int)(finalPos.getX() / mainWindow->getProject()->getGrid3DX()) * mainWindow->getProject()->getGrid3DX());
                            }
                            if (mainWindow->getProject()->getGrid3DY() > 0.0f)
                            {
                                finalPos.setY((int)(finalPos.getY() / mainWindow->getProject()->getGrid3DY()) * mainWindow->getProject()->getGrid3DY());
                            }
                            if (mainWindow->getProject()->getGrid3DZ() > 0.0f)
                            {
                                finalPos.setZ((int)(finalPos.getZ() / mainWindow->getProject()->getGrid3DZ()) * mainWindow->getProject()->getGrid3DZ());
                            }

                            object->setPosition(finalPos);
                        }
                        else
                        {
                            updateOffset = false;
                        }
                        mainWindow->object3dinfo->updateSimulation();
                        mainWindow->boneinfo->update();
                    }
                }
                Magic3D::Physics::getInstance()->reset(object);
                mainWindow->setProjectChanged(true);
                mainWindow->update();
            }
        }
        else if (event->buttons() & Qt::RightButton && !(event->buttons() & Qt::LeftButton))
        {
            Magic3D::Camera* camera = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getPerspective();

            if (camera)
            {
                if (getCameraPosition() == MGE::eCAMERA_POSITION_FLY)
                {
                    float angleX = y * mainWindow->cameraoptions->getSpeedLookFactor();
                    float angleY = -x * mainWindow->cameraoptions->getSpeedLookFactor();

                    Magic3D::Quaternion ax = Magic3D::Quaternion::rotationX(Magic3D::Math::radians(angleX));
                    Magic3D::Quaternion ay = Magic3D::Quaternion::rotationY(Magic3D::Math::radians(angleY));

                    camera->setRotation(ay * (camera->getRotation() * ax));
                }
                else
                {
                    float factorX = x * 0.5f;
                    float factorY = -y * 0.5f;

                    if (keyShift && !keyCtrl)
                    {
                        factorX *= 4.0f;
                        factorY *= 4.0f;
                    }

                    if (keyCtrl && !keyShift)
                    {
                        factorX *= 0.25f;
                        factorY *= 0.25f;
                    }

                    Magic3D::Vector3* cam = getCameraVector();
                    *cam += camera->getDirectionUp() * ((factorY) * mainWindow->cameraoptions->getSpeedLookFactor());
                    *cam += camera->getDirectionRight() * ((factorX) * mainWindow->cameraoptions->getSpeedLookFactor());
                    updateCamera(0.0f, true);
                }
            }
            else
            {
                camera = Magic3D::Renderer::getInstance()->getCurrentViewPort()->getOrthographic();
                if (camera)
                {
                    Magic3D::Vector3 pos = camera->getPosition();
                    camera->setPosition(Magic3D::Vector3(pos.getX() + x, pos.getY() + y, pos.getZ()));
                }
            }
            mainWindow->object3dinfo->updateSimulation();
            if (!moving)
            {
                qApp->setOverrideCursor( QCursor( Qt::BlankCursor ) );
            }
            centerCursor = true;

            mainWindow->setProjectChanged(true);
            mainWindow->update();
        }
    }

    float mX = event->pos().x();
    float mY = event->pos().y();

    if (centerCursor)
    {
        QPoint p = QPoint(selectedOffset.x(), selectedOffset.y());
        int pX = this->mapToGlobal(p).x();
        int pY = this->mapToGlobal(p).y();

        QCursor::setPos(pX, pY);
        centerCursor = false;

        mX = selectedOffset.x();
        mY = selectedOffset.y();
    }

    if (updateOffset)
    {
        selectedOffset = QPointF(mX, mY);
    }
}

void MGE::Magic3DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouseDown = false;

    int button = 1;

    switch (event->button())
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::RightButton: button = 2; break;
        case Qt::MiddleButton: button = 3; break;
        default: button = 1; break;
    }

    if (Magic3D::Script::getInstance()->isPlaying())
    {
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_MOUSE, Magic3D::eEVENT_MOUSE_UP, event->pos().x(), event->pos().y(), button);
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_TOUCH, Magic3D::eEVENT_TOUCH_UP, event->pos().x(), event->pos().y(), button);
    }
    else
    {
        while (qApp->overrideCursor() != 0)
        {
            qApp->restoreOverrideCursor();
        }
        if (getSelectedObject())
        {
            getSelectedObject()->setSelected(true);
        }
        if (mainWindow->IsProjectOpened() && !mouseMoved && event->button() & Qt::RightButton )
        {
            resetKeys();

            lastMenuPos = mapFromGlobal(QCursor::pos());

            /*popup->close();
            popup->clear();
            popup->addAction(mainWindow->getUi()->menuAdd->menuAction());
            if (getSelectedObject())
            {
                popup->addSeparator();
                popup->addAction(mainWindow->getUi()->actionRemoveObject);
                if (getSelectedObject()->getType() == Magic3D::eOBJECT_MODEL || getSelectedObject()->getType() == Magic3D::eOBJECT_PARTICLES || getSelectedObject()->getRender() == Magic3D::eRENDER_2D)
                {
                    popup->addAction(mainWindow->getUi()->actionCloneObject);
                }
                popup->addSeparator();
                popup->addAction(mainWindow->getUi()->actionCopy);
                popup->addAction(mainWindow->getUi()->actionPaste);
            }
            popup->popup(event->globalPos());*/
            mainWindow->getUi()->menuAdd->popup(event->globalPos());
        }
    }
}

void MGE::Magic3DWidget::wheelEvent(QWheelEvent *event)
{
    if (Magic3D::Script::getInstance()->isPlaying())
    {
        Magic3D::Input::getInstance()->dispatchEvent(Magic3D::eINPUT_MOUSE, Magic3D::eEVENT_MOUSE_WHEEL, event->pos().x(), event->pos().y(), event->delta());
    }
    else
    {
        float numDegrees = event->delta() / 8.0f;
        float numSteps = numDegrees / 15.0f;
        updateCamera(numSteps);
    }
}

void MGE::Magic3DWidget::saveScript(bool force)
{
    if (mainWindow->getProject() && mainWindow->getProject()->isChangedScript())
    {
        int ret = QMessageBox::Save;
        bool changed = mainWindow->getProject()->isChanged();

        if (!force)
            ret = Utils::confirmSave(tr("Save Script"), tr("The script has been modified."), tr("Do you want to save your changes?"), false);
        if (ret == QMessageBox::Save)
        {
            QString fileName = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData());
            fileName.append(M3D_PATH_SCRIPT);

            if (!QDir().exists(fileName))
            {
                QDir().mkpath(fileName);
            }


            bool empty = mainWindow->getEditorLua()->document()->toPlainText().trimmed().length() == 0;



            if (getSelectedObject())
            {
                fileName.append(getSelectedObject()->getScript().c_str());
            }
            else
            {
                fileName.append(Magic3D::Scene::getInstance()->getScript().c_str()).append(".main");

                changed = changed || Magic3D::Scene::getInstance()->isScripted() == empty;
                Magic3D::Scene::getInstance()->setScripted(!empty);
            }

            fileName.append(M3D_SCRIPT_FILE);

            QFile file(fileName);

            if (empty)
            {
                file.remove();
            }
            else
            {
                file.open(QIODevice::WriteOnly);
                file.write(mainWindow->getEditorLua()->document()->toPlainText().toLocal8Bit().data());
                file.close();
            }

            mainWindow->getUi()->dockScript->setWindowTitle("Script: " + fileName);
        }

        mainWindow->setScriptChanged(false);
        mainWindow->setProjectChanged(changed);
    }
}

void MGE::Magic3DWidget::loadScript()
{
    QString fileName = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData());
    fileName.append(M3D_PATH_SCRIPT);


    if (getSelectedObject())
    {
        fileName.append(getSelectedObject()->getScript().c_str());
    }
    else
    {
        fileName.append(Magic3D::Scene::getInstance()->getScript().c_str()).append(".main");
    }

    fileName.append(M3D_SCRIPT_FILE);

    QFile file(fileName);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        mainWindow->setLoading(true);
        mainWindow->getEditorLua()->clear();
        mainWindow->getEditorLua()->appendPlainText(file.readAll());
        mainWindow->getEditorLua()->moveCursor(QTextCursor::Start);
        mainWindow->setLoading(false);
        file.close();
    }
    else
    {
        QFile* tmp;
        if (getSelectedObject())
        {
            tmp = new QFile(UTILS_DEFAULT_PATH + "object.script");
        }
        else
        {
            tmp = new QFile(UTILS_DEFAULT_PATH + "scene.script");
        }
        tmp->open(QIODevice::ReadOnly);
        mainWindow->setLoading(true);
        mainWindow->getEditorLua()->clear();
        mainWindow->getEditorLua()->appendPlainText(tmp->readAll());
        mainWindow->getEditorLua()->moveCursor(QTextCursor::Start);
        mainWindow->setLoading(false);
        tmp->close();
        delete tmp;
    }
    mainWindow->getEditorLua()->setFileName(fileName);
    mainWindow->getUi()->dockScript->setWindowTitle("Script: " + fileName);
}

void MGE::Magic3DWidget::setScript(QString script)
{
    if (getSelectedObject())
    {
        getSelectedObject()->setScript(script.toStdString());
        loadScript();
    }
}

void MGE::Magic3DWidget::updateScriptView(bool  root)
{
    bool isScriptedObject = !getSelectedBone() && getSelectedObject() && getSelectedObject()->isScripted();
    mainWindow->getUi()->dockScript->setEnabled(root || isScriptedObject);

    bool simulating = Magic3D::Script::getInstance()->isPlaying();
    mainWindow->getUi()->actionNewScript->setEnabled(!root && !simulating && isScriptedObject);
    mainWindow->getUi()->actionOpenScript->setEnabled(!root && !simulating && isScriptedObject);
}

void MGE::Magic3DWidget::setTRS(bool x, bool y, bool z)
{
    trsX = x;
    trsY = y;
    trsZ = z;
    mainWindow->updateToolRender();
}

bool MGE::Magic3DWidget::isTRSX()
{
    return trsX;
}

bool MGE::Magic3DWidget::isTRSY()
{
    return trsY;
}

bool MGE::Magic3DWidget::isTRSZ()
{
    return trsZ;
}

Magic3D::TweenTranslate& MGE::Magic3DWidget::getTranslateCamera()
{
    return translateCamera;
}

void MGE::Magic3DWidget::setLastLayer(QString layer)
{
    if (layer.compare(M3D_MAIN_LAYER_NAME) == 0)
    {
        lastLayer = MAIN_LAYER_NAME;
    }
    else
    {
        lastLayer = layer;
    }
}

QString& MGE::Magic3DWidget::getLastLayer()
{
    return lastLayer;
}

bool MGE::Magic3DWidget::isSimulating()
{
    return Magic3D::Script::getInstance()->isPlaying() || Magic3D::Physics::getInstance()->isPlaying();
}
