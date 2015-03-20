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

#include "textfind.h"
#include "ui_textfind.h"

TextFind::TextFind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextFind)
{
    text = NULL;
    ui->setupUi(this);
}

TextFind::~TextFind()
{
    delete ui;
}

void TextFind::setFindEditor(QPlainTextEdit* editor)
{
    text = editor;
}

QPlainTextEdit* TextFind::getFindEditor()
{
    return text;
}

bool TextFind::isEmpty()
{
    return ui->txtFind->text().isEmpty();
}

void TextFind::findNext()
{
    ui->actionFind_Next->trigger();
}

void TextFind::findPrevious()
{
    ui->actionFind_Previous->trigger();
}

void TextFind::focusFind()
{
    ui->txtFind->setFocus();
    ui->txtFind->selectAll();
}

void TextFind::on_btnFindNext_clicked()
{
    findNext();
}

void TextFind::on_btnFindPrevious_clicked()
{
    findPrevious();
}

void TextFind::on_actionFind_Next_triggered()
{
    if (text)
    {
        text->find(ui->txtFind->text());
        if (!isEmpty())
        {
            text->setFocus();
        }
    }
}

void TextFind::on_actionFind_Previous_triggered()
{
    if (text)
    {
        text->find(ui->txtFind->text(), QTextDocument::FindBackward);
        if (!isEmpty())
        {
            text->setFocus();
        }
    }
}

void TextFind::on_txtFind_returnPressed()
{
    findNext();
}
