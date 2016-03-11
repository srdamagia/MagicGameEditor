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

#include <QPushButton>
#include "sequencewindow.h"
#include "ui_sequencewindow.h"
#include "utils.h"

SequenceWindow::SequenceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceWindow)
{

    object = NULL;
    editing = false;
    fps = 0;

    ui->setupUi(this);
}

SequenceWindow::~SequenceWindow()
{
    delete ui;
}

void SequenceWindow::setObject(Magic3D::Object* object)
{
    this->object = object;

    editing = false;
    if (this->object)
    {
        Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;
        editing = sprite->getCurrentSequence();
    }
    update();
}

Magic3D::Object* SequenceWindow::getObject()
{
    return object;
}

void SequenceWindow::update()
{
    Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;
    if (sprite)
    {
        if (sprite->getMeshes()->size() > 0)
        {
            if (sprite->getMeshes()->at(0)->getMaterials()->size() > 0)
            {
                Magic3D::Material* material = sprite->getMeshes()->at(0)->getMaterials()->at(0);
                Magic3D::Texture* texture = material->getTexture(0);

                if (material && texture)
                {
                    ui->txtImgWidth->setValue(texture->width);
                    ui->txtImgHeight->setValue(texture->height);

                    ui->txtImgWidth->setEnabled(false);
                    ui->txtImgHeight->setEnabled(false);
                }
            }
        }

        Magic3D::SpriteSequence* sequence = sprite->getCurrentSequence();
        if (sequence)
        {
            int count = sequence->getFrameCount();

            for (int i = 0; i < count; i++)
            {
                Magic3D::Vector4 frame = sequence->getFrame(i);
                addFrame(frame.getX(), frame.getY(), frame.getZ(), frame.getW());

                frameCopy.push_back(frame);
            }

            ui->txtName->setText(QString::fromStdString(sequence->getName()));
            ui->txtFPS->setValue(sequence->getFPS());
            fps = sequence->getFPS();

            ui->txtName->setEnabled(false);
        }
    }
}

void SequenceWindow::updateObject()
{
    Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;
    if (sprite)
    {
        bool nameError = false;
        Magic3D::SpriteSequence* sequence = sprite->getCurrentSequence();

        if (!editing && !ui->txtName->text().isEmpty())
        {
            if (!sprite->getSequence(ui->txtName->text().toStdString()))
            {
                if (sequence)
                {
                    sequence->setName(ui->txtName->text().toStdString());
                }
                else
                {
                    sequence = new Magic3D::SpriteSequence(ui->txtName->text().toStdString());
                    sprite->addSequence(sequence);
                    editing = true;
                }
            }
            else
            {
                nameError = true;
                Utils::message(tr("Invalid Name"), tr("Sequence named \"") + ui->txtName->text() + tr("\" already exists!"));
                ui->txtName->setFocus();
            }
        }

        if (sequence && !nameError)
        {
            sequence->setName(ui->txtName->text().toStdString());
            sequence->setFPS(ui->txtFPS->value());

            int count = ui->tblFrames->rowCount();

            sequence->removeAllFrames();

            for (int i = 0; i < count; i++)
            {
                float x = 0.0f;
                float y = 0.0f;
                float w = 0.0f;
                float h = 0.0f;

                bool ok;
                QTableWidgetItem* item = ui->tblFrames->item(i, 0);
                if (item)
                {
                    x = item->text().toFloat(&ok);
                }

                item = ui->tblFrames->item(i, 1);
                if (item)
                {
                    y = item->text().toFloat(&ok);
                }

                item = ui->tblFrames->item(i, 2);
                if (item)
                {
                    w = item->text().toFloat(&ok);
                }

                item = ui->tblFrames->item(i, 3);
                if (item)
                {
                    h = item->text().toFloat(&ok);
                }

                sequence->addFrame(Magic3D::Vector4(x, y, w, h));
            }
        }

        sprite->saveSpriteTexture();
        sprite->setCurrentSequence(sequence);
        sprite->setLooping(true);
        sprite->play();
    }
}

void SequenceWindow::addFrame(float x, float y, float width, float height)
{
    QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(x));

    int row = ui->tblFrames->rowCount();
    ui->tblFrames->insertRow(row);
    ui->tblFrames->setItem(row, 0, item);

    item = new QTableWidgetItem(tr("%1").arg(y));
    ui->tblFrames->setItem(row, 1, item);

    item = new QTableWidgetItem(tr("%1").arg(width));
    ui->tblFrames->setItem(row, 2, item);

    item = new QTableWidgetItem(tr("%1").arg(height));
    ui->tblFrames->setItem(row, 3, item);
}

void SequenceWindow::on_radioPixels_toggled(bool checked)
{
    if (checked)
    {

    }
    ui->lblX->setText(tr("Column:"));
    ui->lblY->setText(tr("Row:"));

    ui->pnlReference->setEnabled(true);

    ui->lblX->setFocus();
}

void SequenceWindow::on_radioUV_toggled(bool checked)
{
    if (checked)
    {

    }
    ui->lblX->setText(tr("X:"));
    ui->lblY->setText(tr("Y:"));

    ui->pnlReference->setEnabled(false);

    ui->lblX->setFocus();
}

void SequenceWindow::on_actionAdd_triggered()
{
    float x      = ui->txtX->value();
    float y      = ui->txtY->value();
    float width  = ui->txtWidth->value();
    float height = ui->txtHeight->value();

    if (ui->radioPixels->isChecked())
    {
        float texW = width / (float)ui->txtImgWidth->value();
        float texH = height / (float)ui->txtImgHeight->value();

        x = x * texW;
        y = 1.0f - y * texH;
        width = texW;
        height = texH;
    }


    addFrame(x, y, width, height);

    updateObject();

    ui->txtX->setFocus();
    ui->txtX->selectAll();
}

void SequenceWindow::on_actionRemove_triggered()
{
    if (ui->tblFrames->currentRow() >= 0)
    {
        ui->tblFrames->removeRow(ui->tblFrames->currentRow());
        updateObject();
    }
}

void SequenceWindow::on_btnOk_clicked()
{
    updateObject();
}

void SequenceWindow::on_txtFPS_valueChanged(int )
{
    updateObject();
}

void SequenceWindow::cancel()
{
    if (editing)
    {
        Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;
        if (sprite)
        {
            Magic3D::SpriteSequence* sequence = sprite->getCurrentSequence();

            if (sequence)
            {
                sequence->removeAllFrames();
                int count = frameCopy.size();

                for (int i = 0; i < count; i++)
                {
                    sequence->addFrame(frameCopy[i]);
                }
            }

            sequence->setFPS(fps);
        }
    }
    else
    {
        Magic3D::Sprite* sprite = (Magic3D::Sprite*)object;
        if (sprite)
        {
            Magic3D::SpriteSequence* sequence = sprite->getCurrentSequence();
            if (sequence)
            {
                sprite->removeSequence(sequence->getName());
                sprite->setCurrentSequence(NULL);
            }
        }
    }
}

void SequenceWindow::on_actionAction_Cancel_triggered()
{
    cancel();
}
