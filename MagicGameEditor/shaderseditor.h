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

#ifndef SHADERSEDITOR_H
#define SHADERSEDITOR_H

#include <QWidget>
#include <QComboBox>
#include "magic3dobjectpanel.h"
#include "codeeditor.h"
#include "highlighter_glsl.h"
#include "utils.h"
#include "textfind.h"

namespace Ui {
class ShadersEditor;
}

class ShadersEditor : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit ShadersEditor(MainWindow* mainWindow);
    ~ShadersEditor();

    virtual void update();
    virtual void updateObject();

private:
    Ui::ShadersEditor *ui;

    CodeEditor* editorVertex;
    HighlighterGLSL* highlighterVertex;

    CodeEditor* editorFragment;
    HighlighterGLSL* highlighterFragment;

    QToolBar* toolBar;
    QComboBox* cmbShaders;

    TextFind* find;

    bool saved;

    void loadShadersList(bool none);
    void updateEditorStartLine(QString file);
    void loadShaders(QString file);
    void saveShaders(QString file);
    bool compileShaders(QString file, bool replace);

    void updateToolBar();

private slots:
    void cmbShaders_currentIndexChanged(int index);
    void Editor_textVertexChanged();
    void Editor_textFragmentChanged();

    void on_actNew_triggered();
    void on_actCompile_triggered();
    void on_actDelete_triggered();
};

#endif // SHADERSEDITOR_H
