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

#include "newprojectwindow.h"
#include "ui_newprojectwindow.h"

#include "utils.h"

NewProjectWindow::NewProjectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/MagicGameEditor/Icons/magic3d.ico"));
}

NewProjectWindow::~NewProjectWindow()
{
    delete ui;
}

void NewProjectWindow::setName(QString name)
{
    ui->txtProjectName->setText(name);
}

QString NewProjectWindow::getName()
{
    return ui->txtProjectName->text();
}

void NewProjectWindow::setData(QString data)
{
    ui->txtPathData->setText(data);
}

QString NewProjectWindow::getData()
{
    return ui->txtPathData->text();
}

void NewProjectWindow::on_btnDataFolder_clicked()
{
    ui->txtPathData->setText(Utils::getFolder(tr("Data Folder"), ui->txtPathData->text()));
}

void NewProjectWindow::on_btnOk_clicked()
{
    bool result = false;
    QString fullpath = ui->txtPathData->text() + ui->txtProjectName->text() + M3D_SCENE_FILE;
    QFile f(fullpath);
    if (f.exists())
    {
        if (Utils::confirm(tr("Confirm"), tr("Project Already Exists!"), tr("Do you want to replace it?")))
        {
            result = true;
        }
    }
    else
    {
        result = true;
    }

    if (ui->txtProjectName->text().isEmpty())
    {
        Utils::message(tr("Error"), tr("Invalid Project Name!"));
        result = false;
    }

    if (result)
    {
        accept();
    }
}

void NewProjectWindow::on_btnCancel_clicked()
{
    reject();
}
