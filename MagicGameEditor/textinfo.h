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

#ifndef TEXTINFO_H
#define TEXTINFO_H

#include <QWidget>
#include "magic3dobjectpanel.h"

namespace Ui {
class TextInfo;
}

class TextInfo : public Magic3DObjectPanel
{
    Q_OBJECT
public:
    explicit TextInfo(MainWindow* mainWindow);
    ~TextInfo();

    void loadFontsList();
    virtual void update();
    virtual void updateObject();

private slots:
    void on_txtText_textChanged();

    void on_btnTextColor_clicked();

    void on_rbTextAlignLeft_toggled(bool checked);

    void on_rbTextAlignCenter_toggled(bool checked);

    void on_rbTextAlignRight_toggled(bool checked);

    void on_cmbFont_currentIndexChanged(int index);

    void on_txtFontSize_valueChanged(double arg1);

private:
    Ui::TextInfo *ui;

    bool eventFilter(QObject *object, QEvent *event);
    Magic3D::TextData* getTextData();
};

#endif // TEXTINFO_H
