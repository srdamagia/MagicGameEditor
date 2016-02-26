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

#ifndef MAGIC3DWIDGET_H
#define MAGIC3DWIDGET_H

#include <magic3d/math.h>
#include <magic3d/log.h>
#include <magic3d/tween/tween_translate.h>
#include <QGLWidget>
#include <QTextBrowser>
#include <QMenu>
#include <QLabel>

class MainWindow;
namespace Magic3D
{
    class Object;
    class Layer;
    class Bone;
}

namespace MGE {

enum CAMERA_POSITION
{
    eCAMERA_POSITION_TOP,
    eCAMERA_POSITION_BOTTOM,
    eCAMERA_POSITION_FRONT,
    eCAMERA_POSITION_BACK,
    eCAMERA_POSITION_LEFT,
    eCAMERA_POSITION_RIGHT,
    eCAMERA_POSITION_FLY
};

enum AXIS_TYPE
{
    eAXIS_TYPE_CAMERA,
    eAXIS_TYPE_GLOBAL,
    eAXIS_TYPE_LOCAL
};

struct ObjectRestore
{
    Magic3D::Vector3 position;
    Magic3D::Quaternion rotation;
    Magic3D::Vector3 scale;
    bool changed;
};

class WidgetLogCallBack : public Magic3D::LogCallBack
{
private:
    QTextBrowser* widgetLog;

public:
    explicit WidgetLogCallBack(QTextBrowser* widgetLog)
    {
        this->widgetLog = widgetLog;
    }

    void log(Magic3D::LOG type, const char* text)
    {
        if (widgetLog)
        {
            if (type)
            {

            }
            widgetLog->insertHtml(QString::fromLocal8Bit(text, strlen(text)) + "<BR>");
            widgetLog->moveCursor(QTextCursor::End);
        }
    }
};

class Magic3DWidget : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots
private:
    MainWindow* mainWindow;

    QPoint lastMenuPos;

    WidgetLogCallBack* logCallBack;
    Magic3D::Object* selected;
    Magic3D::Bone* selectedBone;
    Magic3D::Object* copied;
    bool mouseDown;
    bool mouseMoved;
    QPointF selectedOffset;
    QString lastLayer;

    QMenu* popup;

    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;

    bool keyShift;
    bool keyCtrl;

    bool trsX;
    bool trsY;
    bool trsZ;

    AXIS_TYPE axis;

    int timer;

    bool loading;

    ObjectRestore objectRestore;

    CAMERA_POSITION cameraPosition;
    Magic3D::Vector3 flyPosition;
    Magic3D::Vector3 flyRotation;

    Magic3D::Vector3 cameraTop;
    Magic3D::Vector3 cameraBottom;
    Magic3D::Vector3 cameraFront;
    Magic3D::Vector3 cameraBack;
    Magic3D::Vector3 cameraLeft;
    Magic3D::Vector3 cameraRight;

    Magic3D::TweenTranslate translateCamera;

    void objectAlreadyExists(std::string name);
    void getViewBox(Magic3D::Object* object, Magic3D::Box& box1, Magic3D::Box& box2, bool back);
    void getViewLine(Magic3D::Object* object, Magic3D::Box& box);

    bool raycast(float x, float y, bool perspective);

    void log(QString label, Magic3D::Vector3 vec);
    void log(Magic3D::Matrix4 matrix);
    void log(QString log);

    void updateCamera(float factor = 0.0f, bool force = false);

    void capsule(QPainter *painter);

    void resetKeys();

    void editTerrain(Magic3D::Camera* camera, QMouseEvent *event);
public:
    Magic3DWidget(const QGLFormat& format, MainWindow *parent);
    ~Magic3DWidget();

    QPoint getLastMenuPos();

    void setPath(std::string path);
    bool addLayer(std::string name);
    Magic3D::Layer* getLayer(std::string name);
    void removeLayer(std::string name);
    Magic3D::Object* getObject(std::string name);
    void removeObject(std::string layer, std::string name);
    bool renameObject(std::string name, std::string newName);
    bool cloneObject(Magic3D::Object* object, std::string name, std::string layer);
    Magic3D::Object* spawnObject(Magic3D::Object* object, std::string name, std::string layer);

    Magic3D::Vector3* getCameraVector();

    Magic3D::Object* addSprite(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addParticles(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addText(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addWindow(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addButton(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addSlider(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addModel(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addTerrain(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addWater(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addVegetation(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addInstance(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addLight(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addSound(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addCamera(std::string name, std::string layer, QPoint pos);
    Magic3D::Object* addJoystick(std::string name, std::string layer, QPoint pos);

    Magic3D::Vector3 getPosition2D(QPoint pos);
    Magic3D::Vector3 getPosition3D(QPoint pos);

    void setSelection(Magic3D::Object* object, Magic3D::Bone* bone);
    Magic3D::PhysicsObject* getSelectedPhysicsObject();
    Magic3D::Object* getSelectedObject();
    Magic3D::Bone* getSelectedBone();

    void setCopiedObject(Magic3D::Object* copied);
    Magic3D::Object* getCopiedObject();

    void updateInfo();

    void start();
    void stop();
    void save();
    void load();

    void setShowGizmos(bool show);
    bool isShowingGizmos();

    void setShowGizmosPhysics(bool show);
    bool isShowingGizmosPhysics();

    void setShowOctree(bool show);
    bool isShowingOctree();
    void setShowOctreeObjects(bool show);
    bool isShowingOctreeObjects();

    void setCameraPosition(CAMERA_POSITION position);
    CAMERA_POSITION getCameraPosition();

    void saveScript(bool force);
    void loadScript();
    void setScript(QString script);
    void updateScriptView(bool  root);

    void setTRS(bool x, bool y, bool z);

    bool isTRSX();
    bool isTRSY();
    bool isTRSZ();

    Magic3D::TweenTranslate& getTranslateCamera();
    void setLastLayer(QString layer);
    QString& getLastLayer();

    bool isSimulating();
protected:

    void initializeGL();

    void resizeGL(int w, int h);

    //void paintGL();
    void paintEvent(QPaintEvent *event);

    void timerEvent(QTimerEvent *);

    void contextMenuEvent(QContextMenuEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};
}

#endif // MAGIC3DWIDGET_H
