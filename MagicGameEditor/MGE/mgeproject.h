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

#ifndef MGEPROJECT_H
#define MGEPROJECT_H

#include <magic3d/magic3d.h>

#define MGE_STRING_DEFAULT_SIZE 256
#define MGE_FILE_NAME_MAX_SIZE 1024

#define MGE_PROJECT_FILTER "MGE Project (*.mge)"
#define MGE_TEXTURE_FILTER "Textures (*.png *.tga)"
#define MGE_PARTICLES_FILTER "Particles (*.particles)"
#define MGE_SCRIPT_FILTER "Script (*.script)"

#define MGE_XML_PROJECT     "Project"
#define MGE_XML_DATA        "data"
#define MGE_XML_OPTIONS     "options"
#define MGE_XML_CAMERA      "camera"
#define MGE_XML_CAMERA_MOVE "move"
#define MGE_XML_CAMERA_LOOK "look"
#define MGE_XML_GRID2D      "grid2D"
#define MGE_XML_GRID3D      "grid3D"

#define MGE_XML_CAMERA_TOP    "cameraTop"
#define MGE_XML_CAMERA_BOTTOM "cameraBottom"
#define MGE_XML_CAMERA_FRONT  "cameraFront"
#define MGE_XML_CAMERA_BACK   "cameraBack"
#define MGE_XML_CAMERA_LEFT   "cameraLeft"
#define MGE_XML_CAMERA_RIGHT  "cameraRight"

#define MGE_XML_SELECTED_OBJECT "selected_object"
#define MGE_XML_SELECTED_BONE   "selected_bone"

#define MGE_XML_SCRIPT_CURSOR_POS "script_cursor_pos"

namespace MGE {

class MGEProject
{
public:
    MGEProject(const std::string& file, const std::string& path);
    ~MGEProject();

    void save();
    void load();
    void setChanged(bool changed);
    void setChangedScript(bool changed);
    bool isChanged();
    bool isChangedScript();

    const std::string& getName();
    void setName(const std::string& name);

    const std::string& getFile();
    const std::string& getPath();
    std::string getFullPath();

    const std::string& getPathData();
    void setPathData(const std::string& path);

    void setCameraMove(float speed);
    float getCameraMove();

    void setCameraLook(float speed);
    float getCameraLook();

    void setGrid2D(float x, float y);
    float getGrid2DX();
    float getGrid2DY();

    void setGrid3D(float x, float y, float z);
    float getGrid3DX();
    float getGrid3DY();
    float getGrid3DZ();

    void setCameraTop(Magic3D::Vector3 value);
    void setCameraBottom(Magic3D::Vector3 value);
    void setCameraFront(Magic3D::Vector3 value);
    void setCameraBack(Magic3D::Vector3 value);
    void setCameraLeft(Magic3D::Vector3 value);
    void setCameraRight(Magic3D::Vector3 value);

    const Magic3D::Vector3& getCameraTop();
    const Magic3D::Vector3& getCameraBottom();
    const Magic3D::Vector3& getCameraFront();
    const Magic3D::Vector3& getCameraBack();
    const Magic3D::Vector3& getCameraLeft();
    const Magic3D::Vector3& getCameraRight();

    const std::string& getSelectedObject();
    void setSelectedObject(const std::string& object);
    const std::string& getSelectedBone();
    void setSelectedBone(const std::string& bone);

    void setScriptCursorPos(int pos);
    int getScriptCursorPos();
private:
    bool changed;
    bool changedScript;

    std::string name;
    std::string file;
    std::string path;

    std::string pathData;

    float cameraMove;
    float cameraLook;

    float grid2D_x;
    float grid2D_y;

    float grid3D_x;
    float grid3D_y;
    float grid3D_z;

    Magic3D::Vector3 cameraTop;
    Magic3D::Vector3 cameraBottom;
    Magic3D::Vector3 cameraFront;
    Magic3D::Vector3 cameraBack;
    Magic3D::Vector3 cameraLeft;
    Magic3D::Vector3 cameraRight;

    std::string selectedObject;
    std::string selectedBone;

    int script_cursor_pos;
};
}

#endif // MGEPROJECT_H
