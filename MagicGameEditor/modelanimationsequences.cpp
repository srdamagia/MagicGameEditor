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

#include "modelanimationsequences.h"
#include "ui_modelanimationsequences.h"

#include "utils.h"

ModelAnimationSequences::ModelAnimationSequences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelAnimationSequences)
{
    ui->setupUi(this);

    animation = NULL;
}

ModelAnimationSequences::~ModelAnimationSequences()
{
    delete ui;
}

void ModelAnimationSequences::setMaximumFrames(int maximum)
{
    ui->txtStart->setMaximum(maximum);
    ui->txtEnd->setMaximum(maximum);
}

void ModelAnimationSequences::setAnimation(Magic3D::Animation* animation)
{
    this->animation = animation;

    setMaximumFrames(animation->getFrameCount() - 1);
}

void ModelAnimationSequences::on_btnOk_clicked()
{
    if (animation)
    {
        if (animation->getSequence(ui->txtName->text().toStdString()))
        {
            Utils::message(tr("Invalid Name"), tr("Sequence named \"") + ui->txtName->text() + tr("\" already exists!"));
        }
        else
        {
            animation->addSequence(ui->txtName->text().toStdString(), ui->txtSpeed->value(), ui->txtStart->value(), ui->txtEnd->value(), ui->chkLoop->isChecked());
            animation->setCurrentSequence(ui->txtName->text().toStdString());
            animation->play();
        }
    }
}

