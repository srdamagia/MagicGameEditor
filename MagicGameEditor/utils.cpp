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

#include <QIcon>
#include "utils.h"

Utils::Utils()
{
}

QString Utils::getApplicationPath()
{
    QDir appDir = QDir(QApplication::applicationDirPath());
    #if defined(__APPLE__) && defined(__MACH__)
    appDir.cdUp();
    appDir.cdUp();
    appDir.cdUp();
    #endif

    return appDir.absolutePath() + "/";
}

QString Utils::getFolder(QString title, QString path)
{
    QString newTitle = title;
    if (newTitle.isEmpty())
    {
        newTitle = tr("Open Folder");
    }

    QString newPath = path;
    if (newPath.isEmpty())
    {
        newPath = UTILS_DEFAULT_PATH;
    }

    QString dir = QFileDialog::getExistingDirectory(NULL, newTitle, newPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
    {
        dir = path;
    }

    dir = dir.replace('\\', '/');
    if (!dir.isEmpty() && !dir.endsWith("/"))
    {
        dir += "/";
    }
    return dir;
}

bool Utils::confirm(QString title, QString text, QString question)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/MagicGameEditor/Icons/magic3d.ico"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setInformativeText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec() == QMessageBox::Yes;
}

int Utils::confirmSave(QString title, QString text, QString question, bool cancel)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/MagicGameEditor/Icons/magic3d.ico"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setInformativeText(question);
    if (cancel)
    {
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    }
    else
    {
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
    }
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void Utils::message(QString title, QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/MagicGameEditor/Icons/magic3d.ico"));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

QColor Utils::getColor(QColor color)
{
    QColor result = color;
    QColorDialog colorDialog(color);

    colorDialog.setWindowIcon(QIcon(":/MagicGameEditor/Icons/magic3d.ico"));
    colorDialog.setOptions(QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
    colorDialog.setCurrentColor(color);

    int dialogResult = colorDialog.exec();
    if (dialogResult == QColorDialog::Accepted)
    {
        result = colorDialog.selectedColor();
    }

    return result;
}



QString Utils::getFilePath(QString file)
{
    QString result = QFileInfo(file).absolutePath();
    if (!result.isEmpty() && !result.endsWith("/"))
    {
        result += "/";
    }
    return result;
}

QString Utils::getFileName(QString file)
{
    QString result = QFileInfo(file).fileName();
    return result;
}

QString Utils::getText(QString title, QString text, bool spaces)
{
    bool ok;
    QString result = QInputDialog::getText(NULL, title, text, QLineEdit::Normal, tr(""), &ok);
    if (ok && !result.isEmpty() && (spaces || !result.contains(" ")))
    {

    }
    else if (ok)
    {
        result.clear();
        Utils::message(tr("Invalid Name"), tr("\"") + result + tr("\" is an invalid name!"));
    }

    return result;
}
