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

#ifndef SEQUENCEWINDOW_H
#define SEQUENCEWINDOW_H

#include <QDialog>

#include <magic3d/magic3d.h>

namespace Ui {
    class SequenceWindow;
}

class SequenceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SequenceWindow(QWidget *parent = 0);
    ~SequenceWindow();

    void setObject(Magic3D::Object* object);
    Magic3D::Object* getObject();

    void update();
    void updateObject();

private:
    Ui::SequenceWindow *ui;

    Magic3D::Object* object;
    bool editing;

    std::vector<Magic3D::Vector4> frameCopy;
    int fps;

    void addFrame(float x, float y, float width, float height);
    void cancel();

private slots:
    void on_txtFPS_valueChanged(int );
    void on_btnOk_clicked();
    void on_actionRemove_triggered();
    void on_actionAdd_triggered();
    void on_radioUV_toggled(bool checked);
    void on_radioPixels_toggled(bool checked);    
    void on_actionAction_Cancel_triggered();
};

#endif // SEQUENCEWINDOW_H
