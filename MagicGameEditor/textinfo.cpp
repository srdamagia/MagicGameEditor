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

#include "textinfo.h"
#include "ui_textinfo.h"

#include <magic3d/gui/guilabel.h>
#include "utils.h"
#include <magic3d/magic3d.h>

TextInfo::TextInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::TextInfo)
{
    ui->setupUi(this);

    ui->txtText->installEventFilter(this);
}

TextInfo::~TextInfo()
{
    delete ui;
}

bool TextInfo::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusOut)
    {
        if (object == ui->txtText)
        {
            //updateObject();
        }
    }
    return false;
}

void TextInfo::loadFontsList()
{
    updating = true;
    QString fileName = "*.fnt";
    QString path = tr(Magic3D::ResourceManager::getFonts()->getPath("").c_str());

    QDir currentDir = QDir(path);
    QStringList files;
    files = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.count(); i++)
    {
        QString file = files.at(i);
        files.replace(i, file.left(file.length() - 4));
    }

    qSort(files.begin(), files.end(), lessNone);
    mainWindow->setComboList(files, ui->cmbFont);

    updating = false;
}

Magic3D::TextData* TextInfo::getTextData()
{
    Magic3D::TextData* result = NULL;
    if (getObject()->getType() == Magic3D::eOBJECT_GUI_LABEL)
    {
        result = static_cast<Magic3D::GUILabel*>(getObject())->getText();
    }
    if (getObject()->getType() == Magic3D::eOBJECT_TEXT)
    {
        result = static_cast<Magic3D::Text*>(getObject())->getText();
    }
    return result;
}

void TextInfo::update()
{
    updating = true;
    Magic3D::TextData* textData = getTextData();

    if (textData)
    {
        QTextCursor cursor = ui->txtText->textCursor();
        ui->txtText->setPlainText(QString::fromLatin1(textData->getText().c_str(), textData->getText().length()));
        ui->txtText->setTextCursor(cursor);

        switch (textData->getTextAlignment())
        {
            case Magic3D::eHORIZONTAL_ALIGN_LEFT:   ui->rbTextAlignLeft->setChecked(true); break;
            case Magic3D::eHORIZONTAL_ALIGN_CENTER: ui->rbTextAlignCenter->setChecked(true); break;
            case Magic3D::eHORIZONTAL_ALIGN_RIGHT:  ui->rbTextAlignRight->setChecked(true); break;
        }

        setButtonColor(ui->btnTextColor, textData->getTextColor());
        if (!ui->txtFontSize->hasFocus())
        {
            ui->txtFontSize->setValue(textData->getTextSize());
        }

        loadFontsList();
        ui->cmbFont->setCurrentIndex(ui->cmbFont->findText(QString::fromStdString(textData->getFont()->getName())));
    }

    updating = false;
}

void TextInfo::updateObject()
{
    Magic3D::TextData* textData = getTextData();
    if (!updating && textData)
    {        
        //label->setLabel(ui->txtText->toPlainText().toStdString());
        textData->setText(std::string(ui->txtText->toPlainText().toLatin1()));

        if (ui->rbTextAlignLeft->isChecked())
        {
            textData->setTextAlignment(Magic3D::eHORIZONTAL_ALIGN_LEFT);
        }
        else if (ui->rbTextAlignCenter->isChecked())
        {
            textData->setTextAlignment(Magic3D::eHORIZONTAL_ALIGN_CENTER);
        }
        else if (ui->rbTextAlignRight->isCheckable())
        {
            textData->setTextAlignment(Magic3D::eHORIZONTAL_ALIGN_RIGHT);
        }

        bool created;
        textData->setFont(Magic3D::ResourceManager::getFonts()->load(ui->cmbFont->currentText().toStdString(), created));
        textData->setTextSize(ui->txtFontSize->value());

        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void TextInfo::on_txtText_textChanged()
{
    Magic3D::TextData* textData = getTextData();
    if (!updating && textData)
    {
        textData->setText(std::string(ui->txtText->toPlainText().toLatin1()));

        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void TextInfo::on_btnTextColor_clicked()
{
    Magic3D::TextData* textData = getTextData();
    if (textData)
    {
        textData->setTextColor(chooseColor(textData->getTextColor()));

        updateObject();
    }
}

void TextInfo::on_rbTextAlignLeft_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void TextInfo::on_rbTextAlignCenter_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void TextInfo::on_rbTextAlignRight_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void TextInfo::on_cmbFont_currentIndexChanged(int index)
{
    if (index > 0)
    {

    }
    updateObject();
}

void TextInfo::on_txtFontSize_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}
