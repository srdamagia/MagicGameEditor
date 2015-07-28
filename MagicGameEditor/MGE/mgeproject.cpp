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

#include "mgeproject.h"
#include "utils.h"

MGE::MGEProject::MGEProject(const std::string file, const std::string path)
{
    changed = true;
    changedScript = false;

    setName(file.substr(0, file.rfind(M3D_SCENE_FILE)));

    this->file = name + M3D_SCENE_FILE;
    if (!path.empty())
    {
        this->path = path + (path.at(path.size() - 1) == '/' ? "" : "/");
    }

    pathData.clear();

    cameraMove = 0.0f;
    cameraLook = 0.0f;

    setGrid2D(0.01f, 0.01f);
    setGrid3D(1.0f, 1.0f, 1.0f);

    cameraTop    = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    cameraBottom = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    cameraFront  = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    cameraBack   = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    cameraLeft   = Magic3D::Vector3(0.0f, 0.0f, 0.0f);
    cameraRight  = Magic3D::Vector3(0.0f, 0.0f, 0.0f);

    selectedObject.clear();
    selectedBone.clear();

    script_cursor_pos = 0;
}

MGE::MGEProject::~MGEProject()
{
}

void MGE::MGEProject::save()
{
    std::string fullPath = getFullPath();
    if (changed && !fullPath.empty())
    {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLElement* config;
        tinyxml2::XMLDeclaration* decl = doc.NewDeclaration( MAGIC3D_VERSION );
        doc.LinkEndChild( decl );

        tinyxml2::XMLElement* root = doc.NewElement( "MGE" );
        doc.LinkEndChild( root );

        tinyxml2::XMLComment* comment = root->GetDocument()->NewComment(" MGE Project ");
        root->LinkEndChild( comment );

        tinyxml2::XMLElement* configs = root->GetDocument()->NewElement( MGE_XML_PROJECT );
        root->LinkEndChild( configs );

        if (!getPathData().empty())
        {
            config = configs->GetDocument()->NewElement( MGE_XML_DATA );
            config->LinkEndChild( configs->GetDocument()->NewText( getPathData().c_str() ));
            configs->LinkEndChild( config );
        }

        config = configs->GetDocument()->NewElement( MGE_XML_OPTIONS );

        Magic3D::Xml::saveString(config, MGE_XML_SELECTED_OBJECT, selectedObject);
        Magic3D::Xml::saveString(config, MGE_XML_SELECTED_BONE, selectedBone);

        Magic3D::Xml::saveInt(config, MGE_XML_SCRIPT_CURSOR_POS, script_cursor_pos);

        tinyxml2::XMLElement* opt = config->GetDocument()->NewElement( MGE_XML_CAMERA );
        tinyxml2::XMLElement* value = opt->GetDocument()->NewElement( MGE_XML_CAMERA_MOVE );
        std::stringstream s_move;
        s_move << getCameraMove();
        value->LinkEndChild( opt->GetDocument()->NewText( s_move.str().c_str() ));
        opt->LinkEndChild(value);

        value = opt->GetDocument()->NewElement( MGE_XML_CAMERA_LOOK );
        std::stringstream s_look;
        s_look << getCameraLook();
        value->LinkEndChild( opt->GetDocument()->NewText( s_look.str().c_str() ));
        opt->LinkEndChild(value);

        value = opt->GetDocument()->NewElement( MGE_XML_GRID2D );
        value->SetAttribute("x", getGrid2DX());
        value->SetAttribute("y", getGrid2DY());
        value->SetAttribute("z", 1.0f);
        opt->LinkEndChild(value);

        value = opt->GetDocument()->NewElement( MGE_XML_GRID3D );
        value->SetAttribute("x", getGrid3DX());
        value->SetAttribute("y", getGrid3DY());
        value->SetAttribute("z", getGrid3DZ());
        opt->LinkEndChild(value);

        Magic3D::Xml::saveVector3(opt, MGE_XML_CAMERA_TOP,    cameraTop);
        Magic3D::Xml::saveVector3(opt, MGE_XML_CAMERA_BOTTOM, cameraBottom);
        Magic3D::Xml::saveVector3(opt, MGE_XML_CAMERA_FRONT,  cameraFront);
        Magic3D::Xml::saveVector3(opt, MGE_XML_CAMERA_BACK,   cameraBack);
        Magic3D::Xml::saveVector3(opt, MGE_XML_CAMERA_LEFT,   cameraLeft);
        Magic3D::Xml::saveVector3(opt, MGE_XML_CAMERA_RIGHT,  cameraRight);

        config->LinkEndChild( opt );
        configs->LinkEndChild( config );

        Magic3D::Scene::getInstance()->save(root);

        doc.SaveFile( fullPath.c_str() );
    }
    changed = false;
}

void MGE::MGEProject::load()
{
    std::string fullPath = getFullPath();
    if (!fullPath.empty())
    {
        tinyxml2::XMLDocument doc;
        int error = doc.LoadFile(fullPath.c_str());
        if (error != tinyxml2::XML_SUCCESS)
        {
            Magic3D::Log::logFormat(Magic3D::eLOG_FAILURE, "xml error: %d", error);
            return;
        }

        tinyxml2::XMLElement* root = doc.FirstChildElement();

        // should always have a valid root but handle gracefully if it does
        if (!root)
        {
            return;
        }

        tinyxml2::XMLElement* pElem = root->FirstChildElement(MGE_XML_PROJECT)->FirstChildElement(MGE_XML_DATA);
        if (pElem)
        {
            setPathData(pElem->GetText());
        }

        pElem = root->FirstChildElement(MGE_XML_PROJECT)->FirstChildElement(MGE_XML_OPTIONS);
        if (pElem)
        {
            selectedObject = Magic3D::Xml::loadString(pElem, MGE_XML_SELECTED_OBJECT);
            selectedBone   = Magic3D::Xml::loadString(pElem, MGE_XML_SELECTED_BONE);

            script_cursor_pos = Magic3D::Xml::loadInt(pElem, MGE_XML_SCRIPT_CURSOR_POS);

            if (selectedObject.compare(M3D_XML_NULL) == 0)
            {
                selectedObject.clear();
            }
            if (selectedBone.compare(M3D_XML_NULL) == 0)
            {
                selectedBone.clear();
            }

            tinyxml2::XMLElement* option = pElem->FirstChildElement(MGE_XML_CAMERA);
            if (option)
            {
                tinyxml2::XMLElement* child;
                child = option->FirstChildElement(MGE_XML_CAMERA_MOVE);
                if (child)
                {
                    setCameraMove(atof(child->GetText()));
                }
                child = option->FirstChildElement(MGE_XML_CAMERA_LOOK);
                if (child)
                {
                    setCameraLook(atof(child->GetText()));
                }

                Magic3D::Vector3 grid2D = Magic3D::Xml::loadVector3(option, MGE_XML_GRID2D);
                setGrid2D(grid2D.getX(), grid2D.getY());
                Magic3D::Vector3 grid3D = Magic3D::Xml::loadVector3(option, MGE_XML_GRID3D);
                setGrid3D(grid3D.getX(), grid3D.getY(), grid3D.getZ());

                cameraTop    = Magic3D::Xml::loadVector3(option, MGE_XML_CAMERA_TOP);
                cameraBottom = Magic3D::Xml::loadVector3(option, MGE_XML_CAMERA_BOTTOM);
                cameraFront  = Magic3D::Xml::loadVector3(option, MGE_XML_CAMERA_FRONT);
                cameraBack   = Magic3D::Xml::loadVector3(option, MGE_XML_CAMERA_BACK);
                cameraLeft   = Magic3D::Xml::loadVector3(option, MGE_XML_CAMERA_LEFT);
                cameraRight  = Magic3D::Xml::loadVector3(option, MGE_XML_CAMERA_RIGHT);
            }
        }
    }
    changed = false;
    changedScript = false;
}

void MGE::MGEProject::setChanged(bool changed)
{
    this->changed = changed;
}

void MGE::MGEProject::setChangedScript(bool changed)
{
    this->changedScript = changed;
}

bool MGE::MGEProject::isChanged()
{
    return changed;
}

bool MGE::MGEProject::isChangedScript()
{
    return changedScript;
}

const std::string& MGE::MGEProject::getName()
{
    return name;
}

void MGE::MGEProject::setName(const std::string name)
{
    if (this->name.compare(name) != 0)
    {
        this->name = name;
        file = name + M3D_SCENE_FILE;

        changed = true;

        Magic3D::Scene::getInstance()->setScript(this->name);
    }
}

const std::string& MGE::MGEProject::getFile()
{
    return file;
}

const std::string& MGE::MGEProject::getPath()
{
    return path;
}

std::string MGE::MGEProject::getFullPath()
{
    return getPath() + getPathData() + getFile();
}

const std::string& MGE::MGEProject::getPathData()
{
    return pathData;
}

void MGE::MGEProject::setPathData(const std::string path)
{
    if (this->pathData.compare(path) != 0)
    {
        pathData = path;
        if (!pathData.empty())
        {
            pathData = pathData + (pathData.at(pathData.size() - 1) == '/' ? "" : "/");
        }

        changed = true;

        Magic3D::ResourceManager::getInstance()->setPath(UTILS_DEFAULT_PATH.toStdString() + path);
    }
}

void MGE::MGEProject::setCameraMove(float speed)
{
    cameraMove = speed;
    changed = true;
}

float MGE::MGEProject::getCameraMove()
{
    return cameraMove;
}

void MGE::MGEProject::setCameraLook(float speed)
{
    cameraLook = speed;
    changed = true;
}

float MGE::MGEProject::getCameraLook()
{
    return cameraLook;
}

void MGE::MGEProject::setGrid2D(float x, float y)
{
    grid2D_x = x;
    grid2D_y = y;

    Magic3D::Renderer::getInstance()->setGrid2D(Magic3D::Vector3(grid2D_x, 1.0f, grid2D_y));
}

float MGE::MGEProject::getGrid2DX()
{
    return grid2D_x;
}

float MGE::MGEProject::getGrid2DY()
{
    return grid2D_y;
}

void MGE::MGEProject::setGrid3D(float x, float y, float z)
{
    grid3D_x = x;
    grid3D_y = y;
    grid3D_z = z;

    Magic3D::Renderer::getInstance()->setGrid3D(Magic3D::Vector3(grid3D_x, grid3D_y, grid3D_z));
}

float MGE::MGEProject::getGrid3DX()
{
    return grid3D_x;
}

float MGE::MGEProject::getGrid3DY()
{
    return grid3D_y;
}

float MGE::MGEProject::getGrid3DZ()
{
    return grid3D_z;
}

void MGE::MGEProject::setCameraTop(Magic3D::Vector3 value)
{
    cameraTop = value;
}

void MGE::MGEProject::setCameraBottom(Magic3D::Vector3 value)
{
    cameraBottom = value;
}

void MGE::MGEProject::setCameraFront(Magic3D::Vector3 value)
{
    cameraFront = value;
}

void MGE::MGEProject::setCameraBack(Magic3D::Vector3 value)
{
    cameraBack = value;
}

void MGE::MGEProject::setCameraLeft(Magic3D::Vector3 value)
{
    cameraLeft = value;
}

void MGE::MGEProject::setCameraRight(Magic3D::Vector3 value)
{
    cameraRight = value;
}

const Magic3D::Vector3& MGE::MGEProject::getCameraTop()
{
    return cameraTop;
}

const Magic3D::Vector3& MGE::MGEProject::getCameraBottom()
{
    return cameraBottom;
}

const Magic3D::Vector3& MGE::MGEProject::getCameraFront()
{
    return cameraFront;
}

const Magic3D::Vector3& MGE::MGEProject::getCameraBack()
{
    return cameraBack;
}

const Magic3D::Vector3& MGE::MGEProject::getCameraLeft()
{
    return cameraLeft;
}

const Magic3D::Vector3& MGE::MGEProject::getCameraRight()
{
    return cameraRight;
}

const std::string& MGE::MGEProject::getSelectedObject()
{
    return selectedObject;
}

void MGE::MGEProject::setSelectedObject(const std::string object)
{
    selectedObject = object;
}

const std::string& MGE::MGEProject::getSelectedBone()
{
    return selectedBone;
}

void MGE::MGEProject::setSelectedBone(const std::string bone)
{
    selectedBone = bone;
}

void MGE::MGEProject::setScriptCursorPos(int pos)
{
    script_cursor_pos = pos;
}

int MGE::MGEProject::getScriptCursorPos()
{
    return script_cursor_pos;
}
