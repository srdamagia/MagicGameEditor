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

#include "shaderseditor.h"
#include "ui_shaderseditor.h"
#include <QToolBar>
#include <QFile>
#include "magic3d/magic3d.h"
#include "materialinfo.h"

ShadersEditor::ShadersEditor(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::ShadersEditor)
{
    ui->setupUi(this);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editorVertex = new CodeEditor(this);
    editorVertex->setFont(font);
    ui->layoutVertex->addWidget(editorVertex);

    connect(editorVertex, SIGNAL(textChanged()), this, SLOT(Editor_textVertexChanged()));

    highlighterVertex = new HighlighterGLSL(editorVertex->document());

    editorFragment = new CodeEditor(this);
    editorFragment->setFont(font);
    ui->layoutFragment->addWidget(editorFragment);

    connect(editorFragment, SIGNAL(textChanged()), this, SLOT(Editor_textFragmentChanged()));

    highlighterFragment = new HighlighterGLSL(editorFragment->document());

    toolBar = new QToolBar(this);
    toolBar->addAction(ui->actNew);
    toolBar->addAction(ui->actCompile);
    toolBar->addSeparator();
    toolBar->addAction(ui->actDelete);
    toolBar->addSeparator();
    cmbShaders = new QComboBox(toolBar);
    cmbShaders->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(cmbShaders, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbShaders_currentIndexChanged(int)));

    toolBar->addWidget(cmbShaders);
    toolBar->addSeparator();

    ui->layoutToolBar->addWidget(toolBar);

    find = new TextFind(this);
    find->setFindEditor(editorVertex);
    ui->layoutToolBar->addWidget(find, 1, 0);

    editorVertex->setTextFind(find);
    editorFragment->setTextFind(find);

    saved = true;
}

ShadersEditor::~ShadersEditor()
{
    if (highlighterVertex)
    {
        delete highlighterVertex;
        highlighterVertex = NULL;
    }

    if (highlighterFragment)
    {
        delete highlighterFragment;
        highlighterFragment = NULL;
    }

    if (editorVertex)
    {
        delete editorVertex;
        editorVertex = NULL;
    }

    if (editorFragment)
    {
        delete editorFragment;
        editorFragment = NULL;
    }

    if (cmbShaders)
    {
        delete cmbShaders;
        cmbShaders = NULL;
    }

    if (find)
    {
        delete find;
        find = NULL;
    }

    if (toolBar)
    {
        delete toolBar;
        toolBar = NULL;
    }

    delete ui;
}

void ShadersEditor::update()
{
    loadShadersList(false);
}

void ShadersEditor::updateObject()
{

}

void ShadersEditor::cmbShaders_currentIndexChanged(int index)
{
    if (index > 0 && cmbShaders->count() > index)
    {
        loadShaders(cmbShaders->itemText(index));
    }
    else
    {
        loadShaders(UTILS_NONE);
    }
}

void ShadersEditor::Editor_textVertexChanged()
{
    saved = false;
    updateToolBar();
}

void ShadersEditor::Editor_textFragmentChanged()
{
    saved = false;
    updateToolBar();
}

void ShadersEditor::loadShadersList(bool none)
{
    updating = true;
    QString fileName = "*.vsh";
    QString path = tr(Magic3D::ResourceManager::getShaders()->getPath("").c_str());

    QDir currentDir = QDir(path);
    QStringList files;
    files = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    files.append(UTILS_NONE);

    for (int i = 0; i < files.count() - 1; i++)
    {
        QString file = files.at(i);
        files.replace(i, file.left(file.length() - 4));
    }

    qSort(files.begin(), files.end(), lessNone);
    if (none)
    {
        cmbShaders->clear();
        cmbShaders->addItems(files);
        cmbShaders->setCurrentIndex(cmbShaders->findText(UTILS_NONE));
    }
    else
    {
        mainWindow->setComboList(files, cmbShaders);
    }
    updating = false;
}

void ShadersEditor::updateEditorStartLine(QString file)
{
    Magic3D::Shader* shader = Magic3D::ResourceManager::getInstance()->getShaders()->get(file.toStdString());
    int defineCount = 0;
    QString fileName = QString::fromStdString(Magic3D::ResourceManager::getShaders()->getPath(M3D_DEFAULT_SHADER_DEFINES));
    QFile filed(fileName);
    if (filed.exists())
    {
        filed.open(QIODevice::ReadOnly);
        editorVertex->clear();
        editorVertex->appendPlainText(filed.readAll());
        editorVertex->moveCursor(QTextCursor::Start);
        filed.close();
        defineCount = editorVertex->getLineCount() - 1;
        editorVertex->clear();
    }
    defineCount += shader ? shader->defines : 0;

    fileName = QString::fromStdString(Magic3D::ResourceManager::getShaders()->getPath(M3D_DEFAULT_SHADER_VERTEX));
    QFile filev(fileName);
    if (filev.exists())
    {
        filev.open(QIODevice::ReadOnly);
        editorVertex->clear();
        editorVertex->appendPlainText(filev.readAll());
        editorVertex->moveCursor(QTextCursor::Start);
        filev.close();
        editorVertex->setStartLine(editorVertex->getLineCount() - 1 + defineCount);
        editorVertex->clear();
    }
    else
    {
        editorVertex->setStartLine(1 + defineCount);
    }

    fileName = QString::fromStdString(Magic3D::ResourceManager::getShaders()->getPath(M3D_DEFAULT_SHADER_FRAGMENT));
    QFile filef(fileName);
    if (filef.exists())
    {
        filef.open(QIODevice::ReadOnly);
        editorFragment->clear();
        editorFragment->appendPlainText(filef.readAll());
        editorFragment->moveCursor(QTextCursor::Start);
        filef.close();
        editorFragment->setStartLine(editorFragment->getLineCount() - 1 + defineCount);
        editorFragment->clear();
    }
    else
    {
        editorFragment->setStartLine(1 + defineCount);
    }
}

void ShadersEditor::loadShaders(QString file)
{
    bool exists = false;
    if (file.toLower() != UTILS_NONE)
    {
        updateEditorStartLine(file);
        QString fileName = tr(Magic3D::ResourceManager::getShaders()->getPath(file.toStdString()).c_str());
        QFile filev(fileName + M3D_VERTEX_SHADER_FILE);
        if (filev.exists())
        {
            exists = true;
            filev.open(QIODevice::ReadOnly);
            editorVertex->clear();
            editorVertex->appendPlainText(filev.readAll());
            editorVertex->moveCursor(QTextCursor::Start);
            filev.close();
        }
        else
        {
            QFile* tmp = new QFile(UTILS_DEFAULT_PATH + "shader" + tr(M3D_VERTEX_SHADER_FILE));
            tmp->open(QIODevice::ReadOnly);
            editorVertex->clear();
            editorVertex->appendPlainText(tmp->readAll());
            editorVertex->moveCursor(QTextCursor::Start);
            tmp->close();
            delete tmp;
        }

        QFile filef(fileName + M3D_FRAGMENT_SHADER_FILE);
        if (filef.exists())
        {
            exists = true;
            filef.open(QIODevice::ReadOnly);
            editorFragment->clear();
            editorFragment->appendPlainText(filef.readAll());
            editorFragment->moveCursor(QTextCursor::Start);
            filef.close();
        }
        else
        {
            QFile* tmp = new QFile(UTILS_DEFAULT_PATH + "shader" + tr(M3D_FRAGMENT_SHADER_FILE));
            tmp->open(QIODevice::ReadOnly);
            editorFragment->clear();
            editorFragment->appendPlainText(tmp->readAll());
            editorFragment->moveCursor(QTextCursor::Start);
            tmp->close();
            delete tmp;
        }
    }
    else
    {
        editorVertex->clear();
        editorFragment->clear();
    }

    if (exists)
    {
        compileShaders(file, false);
    }

    saved = true;
    updateToolBar();
}

void ShadersEditor::saveShaders(QString file)
{
    QString fileName = tr(Magic3D::ResourceManager::getShaders()->getPath(file.toStdString()).c_str());
    QFile filev(fileName + tr(M3D_VERTEX_SHADER_FILE));
    filev.open(QIODevice::WriteOnly);
    filev.write(editorVertex->document()->toPlainText().toLocal8Bit().data());
    filev.close();

    QFile filef(fileName + tr(M3D_FRAGMENT_SHADER_FILE));
    filef.open(QIODevice::WriteOnly);
    filef.write(editorFragment->document()->toPlainText().toLocal8Bit().data());
    filef.close();

    saved = true;

    updateToolBar();
}

bool ShadersEditor::compileShaders(QString file, bool replace)
{
    bool created;
    Magic3D::Shader* shader = Magic3D::ResourceManager::getInstance()->addShader(file.toStdString(), created);
    if (shader)
    {
        if (replace && !created)
        {
            Magic3D::ResourceManager::getShaders()->compile(file.toStdString(), created);
        }
        created = true;
    }
    else
    {
        created = false;
        Utils::message(tr("Invalid MagicGameEditor File"), tr("Shaders files not found (\"shader.vsh\" and \"shader.fsh\")!"));
    }

    mainWindow->getUi()->txtLog->raise();

    return created;
}

void ShadersEditor::on_actNew_triggered()
{
    bool ok;
    QString result = QInputDialog::getText(NULL, "New Shader", "Name:", QLineEdit::Normal, tr(""), &ok);
    if (ok && !result.isEmpty() && !result.contains(" "))
    {
        if (!Magic3D::ResourceManager::getShaders()->get(result.toStdString()))
        {
            loadShaders(result);
            saveShaders(result);
            if (compileShaders(result, false))
            {
                loadShadersList(false);
                cmbShaders->setCurrentIndex(cmbShaders->findText(result));
            }
        }
        else
        {
            Utils::message(tr("Invalid Name"), tr("Shader named \"") + result + tr("\" already exists!"));
        }
    }
    else if (ok)
    {
        result.clear();
        Utils::message(tr("Invalid Name"), tr("\"") + result + tr("\" is an invalid name!"));
    }
}

void ShadersEditor::on_actCompile_triggered()
{
    QString current = cmbShaders->currentText();

    if (current != UTILS_NONE)
    {
        saveShaders(current);
        compileShaders(current, true);

        mainWindow->materialinfo->checkSaved(true);
    }
}

void ShadersEditor::on_actDelete_triggered()
{
    QString current = cmbShaders->currentText();

    if (current != UTILS_NONE && Utils::confirm("Delete Shader", tr("The shader will be deleted!"), tr("Do you want to continue?")))
    {
        QString fileName = QString::fromStdString(Magic3D::ResourceManager::getShaders()->getPath(current.toStdString()));
        QFile::remove(fileName + M3D_VERTEX_SHADER_FILE);
        QFile::remove(fileName + M3D_FRAGMENT_SHADER_FILE);
        Magic3D::ResourceManager::getShaders()->remove(current.toStdString());

        loadShadersList(true);
        mainWindow->getUi()->txtLog->raise();
    }
}

void ShadersEditor::updateToolBar()
{
    bool isNone = cmbShaders->currentText() == UTILS_NONE;
    ui->actNew->setEnabled(saved);
    ui->actDelete->setEnabled(!isNone);
    ui->actCompile->setEnabled(!isNone);
    cmbShaders->setEnabled(saved);
    editorVertex->setEnabled(!isNone);
    editorFragment->setEnabled(!isNone);
}
