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

#ifndef UTILS_H
#define UTILS_H

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>

#define UTILS_DEFAULT_PATH Utils::getApplicationPath()
#define UTILS_NONE QObject::tr("<none>")
#define MAIN_LAYER_NAME QObject::tr("Main Layer")

class Utils : public QObject
{
public:
    Utils();

    static QString getApplicationPath();
    static QString getFolder(QString title, QString path);
    static bool confirm(QString title, QString text, QString question);
    static int confirmSave(QString title, QString text, QString question, bool cancel = true);
    static void message(QString title, QString text);
    static QColor getColor(QColor color);

    static QString getFilePath(QString file);
    static QString getFileName(QString file);

    static QString getText(QString title, QString text, bool spaces = true);
};

#endif // UTILS_H
