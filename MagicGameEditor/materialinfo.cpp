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

#include "materialinfo.h"
#include "ui_materialinfo.h"
#include "utils.h"
#include "object2dinfo.h"

#include <QInputDialog>
#include <QPixmapCache>
#include <QBuffer>
#include <QImageReader>
#include <QIODevice>

#include <magic3d/magic3d.h>
#include <magic3d/image/image_tga.h>
#include <magic3d/image/image_png.h>

#include "materialvar_boolean.h"
#include "materialvar_float.h"
#include "materialvar_integer.h"
#include "materialvar_matrix.h"
#include "materialvar_texture.h"

LoadThread::LoadThread(QPushButton* button, QString fileName, Magic3D::Texture* texture, int index) : QThread()
{
    this->button = button;
    this->fileName = fileName;
    this->texture = texture;
    this->index = index;
}

LoadThread::~LoadThread()
{

}

void LoadThread::run()
{
    QImage* image = NULL;

    if (fileName.toLower().endsWith(tr("tga")))
    {
        image = getImage(fileName.toStdString(), false);
    }
    else if (fileName.toLower().endsWith(tr("png")))
    {
        image = getImage(fileName.toStdString(), true);
    }

    emit(finished(image, button, texture, index));
}

MaterialInfo::MaterialInfo(MainWindow* mainWindow) :
    Magic3DObjectPanel(mainWindow),
    ui(new Ui::MaterialInfo)
{
    ui->setupUi(this);

    tga1 = NULL;
    tga2 = NULL;

    toolbar = new QToolBar(this);
    toolbar->addAction(ui->actNew);
    toolbar->addAction(ui->actSave);
    toolbar->addSeparator();
    toolbar->addAction(ui->actDelete);
    toolbar->addSeparator();
    cmbMaterials = new QComboBox(toolbar);
    cmbMaterials->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(cmbMaterials, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbMaterials_currentIndexChanged(int)));

    toolbar->addWidget(cmbMaterials);
    toolbar->addSeparator();

    ui->layoutToolBar->addWidget(toolbar);
}

MaterialInfo::~MaterialInfo()
{
    if (tga1)
    {
        tga1->wait();
        delete tga1;
        tga1 = NULL;
    }

    if (tga2)
    {
        tga2->wait();
        delete tga2;
        tga2 = NULL;
    }

    clearVariables();

    if (cmbMaterials)
    {
        delete cmbMaterials;
        cmbMaterials = NULL;
    }
    if (toolbar)
    {
        delete toolbar;
        toolbar = NULL;
    }

    delete ui;
}

void MaterialInfo::updateTextureCombo()
{
    updating = true;
    texturesList.clear();
    texturesList.append(UTILS_NONE);

    ui->cmbTexture1->clear();
    ui->cmbTexture2->clear();

    Magic3D::ResourcesTexture* textures = Magic3D::ResourceManager::getTextures();
    std::map<std::string, Magic3D::Texture*>::const_iterator it_l = textures->begin();
    std::map<std::string, Magic3D::Texture*>::const_iterator it_end = textures->end();
    while (it_l != it_end)
    {
        texturesList.append(QString::fromStdString((*it_l++).first));
    }

    qSort(texturesList.begin(), texturesList.end(), lessNone);
    ui->cmbTexture1->addItems(texturesList);
    ui->cmbTexture2->addItems(texturesList);
    updating = false;
}

void MaterialInfo::updateShaderCombo()
{
    updating = true;
    QStringList items;
    items.append(UTILS_NONE);

    ui->cmbShader->clear();

    Magic3D::ResourcesShader* shaders = Magic3D::ResourceManager::getShaders();
    std::map<std::string, Magic3D::Shader*>::const_iterator it_s = shaders->begin();
    while (it_s != shaders->end())
    {
        items.append(QString::fromStdString((*it_s++).first));
    }

    qSort(items.begin(), items.end(), lessNone);
    ui->cmbShader->addItems(items);
    updating = false;
}

void MaterialInfo::needSave()
{
    ui->actSave->setEnabled(true);
}

void MaterialInfo::cancel()
{
    ui->actSave->setEnabled(false);
    cmbMaterials->setCurrentIndex(0);
}

void MaterialInfo::update()
{
    if (mainWindow->getProject())
    {
        updating = true;
        updateTextureCombo();
        updateShaderCombo();
        mainWindow->updateMaterialsList();

        mainWindow->setComboList(mainWindow->getMaterialList(), cmbMaterials);
        updating = false;
    }
}

void MaterialInfo::loadMaterial()
{
    updating = true;
    if (material)
    {
        ui->scrollArea->setEnabled(true);

        setButtonColor(ui->btnColorAmbient,  material->getColorAmbient());
        setButtonColor(ui->btnColorDiffuse,  material->getColorDiffuse());
        setButtonColor(ui->btnColorEmission, material->getColorEmission());
        setButtonColor(ui->btnColorSpecular, material->getColorSpecular());

        ui->sldShininess->setValue(material->getShininess());

        if (!ui->txtAlphaTest->hasFocus())
        {
            ui->txtAlphaTest->setValue(material->getAlphaTest());
        }

        ui->imgTexture1->setIcon(QIcon());
        ui->imgTexture2->setIcon(QIcon());

        QPixmapCache::clear();

        Magic3D::Texture* texture = material->getTexture(0);
        if (texture)
        {
            QString file = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData()) +  QString::fromStdString(Magic3D::ResourceManager::getTextures()->getPrefix(texture->type)) + QString::fromStdString(texture->file);

            if (tga1 && tga1->isFinished())
            {
                tga1->wait();
                delete tga1;
                tga1 = NULL;
            }
            if (!tga1)
            {
                tga1 = new LoadThread(ui->imgTexture1, file, texture, 0);

                connect(tga1, SIGNAL(finished(QImage*,QPushButton*,Magic3D::Texture*,int)),
                        SLOT(loadFinished(QImage*,QPushButton*,Magic3D::Texture*,int)));

                tga1->start(QThread::LowestPriority);
            }

            ui->imgTexture1->setAutoFillBackground(false);
            ui->cmbTexture1->setCurrentIndex(ui->cmbTexture1->findText(QString::fromStdString(texture->getName())));

            ui->rbClamp1->setChecked(texture->clamp);
            ui->rbRepeat1->setChecked(!texture->clamp);
        }
        else
        {
            ui->imgTexture1->setStyleSheet(tr("* {background-color : %1}").arg(QColor(128, 128, 128, 255).name()));
            ui->imgTexture1->setAutoFillBackground(true);

            ui->cmbTexture1->setCurrentIndex(ui->cmbTexture1->findText(UTILS_NONE));
        }

        texture = material->getTexture(1);
        if (texture)
        {
            QString file = UTILS_DEFAULT_PATH + QString::fromStdString(mainWindow->getProject()->getPathData()) + QString::fromStdString(Magic3D::ResourceManager::getTextures()->getPrefix(texture->type)) + QString::fromStdString(texture->file);

            if (tga2 && tga2->isFinished())
            {
                tga2->wait();
                delete tga2;
                tga2 = NULL;
            }
            if (!tga2)
            {
                tga2 = new LoadThread(ui->imgTexture2, file, texture, 1);

                connect(tga2, SIGNAL(finished(QImage*,QPushButton*,Magic3D::Texture*,int)),
                        SLOT(loadFinished(QImage*,QPushButton*,Magic3D::Texture*,int)));

                tga2->start(QThread::LowestPriority);
            }

            ui->imgTexture2->setAutoFillBackground(false);
            ui->cmbTexture2->setCurrentIndex(ui->cmbTexture2->findText(QString::fromStdString(texture->getName())));

            ui->rbClamp2->setChecked(texture->clamp);
            ui->rbRepeat2->setChecked(!texture->clamp);
        }
        else
        {
            ui->imgTexture2->setStyleSheet(tr("* {background-color : %1}").arg(QColor(128, 128, 128, 255).name()));
            ui->imgTexture2->setAutoFillBackground(true);

            ui->cmbTexture2->setCurrentIndex(ui->cmbTexture2->findText(UTILS_NONE));
        }

        Magic3D::Shader* shader = material->getShader();
        if (shader)
        {
            ui->cmbShader->setCurrentIndex(ui->cmbShader->findText(QString::fromStdString(shader->getName())));
        }
        else
        {
            ui->cmbShader->setCurrentIndex(ui->cmbShader->findText(UTILS_NONE));
        }

        switch (material->getBlend())
        {
            case Magic3D::eBLEND_ALPHA:    ui->rbAlpha->setChecked(true); break;
            case Magic3D::eBLEND_ADD:      ui->rbAdd->setChecked(true); break;
            case Magic3D::eBLEND_MULTIPLY: ui->rbMultiply->setChecked(true); break;
            case Magic3D::eBLEND_DIVIDE:   ui->rbDivide->setChecked(true); break;
            case Magic3D::eBLEND_INVERT:   ui->rbInvert->setChecked(true); break;
            case Magic3D::eBLEND_COLOR:    ui->rbColor->setChecked(true); break;
            default: break;
        }

        clearVariables();

        ui->gbShaderVars->setVisible(material->getVarCount() > 0);

        for (int i = 0; i < material->getVarCount(); i++)
        {
            Magic3D::MaterialVar* variable = material->getVar(i);

            MaterialVar* pnl = NULL;

            switch (variable->getType())
            {
                case Magic3D::eSHADER_VAR_FLOAT:
                case Magic3D::eSHADER_VAR_FLOAT_VEC2:
                case Magic3D::eSHADER_VAR_FLOAT_VEC3:
                case Magic3D::eSHADER_VAR_FLOAT_VEC4:
                {
                    pnl = new MaterialVar_Float(mainWindow);
                    break;
                }
                case Magic3D::eSHADER_VAR_INT:
                case Magic3D::eSHADER_VAR_INT_VEC2:
                case Magic3D::eSHADER_VAR_INT_VEC3:
                case Magic3D::eSHADER_VAR_INT_VEC4:
                {
                    pnl = new MaterialVar_Integer(mainWindow);
                    break;
                }
                case Magic3D::eSHADER_VAR_BOOL:
                case Magic3D::eSHADER_VAR_BOOL_VEC2:
                case Magic3D::eSHADER_VAR_BOOL_VEC3:
                case Magic3D::eSHADER_VAR_BOOL_VEC4:
                {
                    pnl = new MaterialVar_Boolean(mainWindow);
                    break;
                }
                case Magic3D::eSHADER_VAR_FLOAT_MAT2:
                case Magic3D::eSHADER_VAR_FLOAT_MAT3:
                case Magic3D::eSHADER_VAR_FLOAT_MAT4:
                {
                    pnl = new MaterialVar_Matrix(mainWindow);
                    break;
                }
                case Magic3D::eSHADER_VAR_SAMPLER_2D:
                case Magic3D::eSHADER_VAR_SAMPLER_CUBE:
                {
                    pnl = new MaterialVar_Texture(mainWindow);
                    ((MaterialVar_Texture*)pnl)->updateTextureCombo(texturesList);
                    break;
                }

                default: break;
            }

            if (pnl)
            {
                pnl->setMaterial(material);
                pnl->setMaterialVarIndex(i);
                pnl->setMaterialInfo(this);
                pnl->update();
                variables.append(pnl);
                ui->layoutShaderVars->addWidget(pnl);
            }
        }
    }
    else
    {
        ui->scrollArea->setEnabled(false);

        setButtonColor(ui->btnColorAmbient,  Magic3D::ColorRGBA());
        setButtonColor(ui->btnColorDiffuse,  Magic3D::ColorRGBA());
        setButtonColor(ui->btnColorEmission, Magic3D::ColorRGBA());
        setButtonColor(ui->btnColorSpecular, Magic3D::ColorRGBA());

        ui->sldShininess->setValue(0);

        ui->imgTexture1->setIcon(QIcon());
        ui->imgTexture2->setIcon(QIcon());

        ui->cmbTexture1->setCurrentIndex(0);
        ui->cmbTexture2->setCurrentIndex(0);

        ui->cmbShader->setCurrentIndex(0);

        ui->txtAlphaTest->setValue(0.0f);

        clearVariables();

        ui->gbShaderVars->setVisible(false);
    }

    ui->actDelete->setEnabled(material);
    updating = false;
}

void MaterialInfo::loadFinished(QImage* image, QPushButton* button, Magic3D::Texture* texture, int index)
{
    if (image)
    {
        if (material)
        {
            Magic3D::Texture* tex = material->getTexture(index);

            if (texture && tex && texture->id == tex->id)
            {
                QIcon ico = QIcon(QPixmap::fromImage(*image).scaled(button->width() - 2, button->height() - 2, Qt::KeepAspectRatio));
                button->setIcon(ico);
                delete image;
                if (texture->type == Magic3D::eTEXTURE_FONT)
                {
                    button->setStyleSheet(tr("* {background-color : %1}").arg(QColor(0, 0, 0, 255).name()));
                }
                else
                {
                    button->setStyleSheet(tr("* {background-color : %1}").arg(QColor(128, 128, 128, 255).name()));
                }
                button->setAutoFillBackground(true);
            }
        }
    }
}

QImage* /*MaterialInfo::*/getImage(std::string fileName, bool png)
{
    Magic3D::Image* image = NULL;
    QImage* img = NULL;
    Magic3D::File* io = new Magic3D::File();
    if (io->open(fileName.c_str(), "rb"))
    {
        if (png)
        {
            image = new Magic3D::PNG();
        }
        else
        {
            image = new Magic3D::TGA();
        }
        image->decode(io);
    }
    delete io;

    if (image)
    {
        img = new QImage( image->getWidth(), image->getHeight(), QImage::Format_RGB32);

        int y_start, y_step, y_end;
        /*if( tga.flags & TGA_ORIGIN_UPPER ) {
         y_start = 0;
         y_step = 1;
         y_end = image->getHeight();
     }*/
        //else {
        y_start = image->getHeight() - 1;
        y_step = -1;
        y_end = -1;
        //}

        byte* src = (byte*)image->getPixels();

        for( int y = y_start; y != y_end; y += y_step )
        {
            QRgb * scanline = (QRgb *) img->scanLine( y );

            for( int x = 0; x < image->getWidth(); x++ )
            {
                Magic3D::ColorRGBApack color = image->getPixelColor(src);
                scanline[x] = qRgba( color.r, color.g, color.b, color.a );
                src += image->getBPP() / 8;
            }
        }

        delete image;
    }

    return img;
}

void MaterialInfo::updateObject()
{
    if (!updating && material)
    {
        if (ui->rbAlpha->isChecked())
            material->setBlend(Magic3D::eBLEND_ALPHA);
        else if (ui->rbMultiply->isChecked())
            material->setBlend(Magic3D::eBLEND_MULTIPLY);
        else if (ui->rbAdd->isChecked())
            material->setBlend(Magic3D::eBLEND_ADD);
        else if (ui->rbDivide->isChecked())
            material->setBlend(Magic3D::eBLEND_DIVIDE);
        else if (ui->rbInvert->isChecked())
            material->setBlend(Magic3D::eBLEND_INVERT);
        else if (ui->rbColor->isChecked())
            material->setBlend(Magic3D::eBLEND_COLOR);

        material->setShininess(ui->sldShininess->value());
        material->setAlphaTest(ui->txtAlphaTest->value());

        if (ui->cmbTexture1->currentIndex() > 0)
        {
            material->setTexture(0, Magic3D::ResourceManager::getTextures()->get(ui->cmbTexture1->currentText().toStdString()));
        }
        else
        {
            material->setTexture(0, NULL);
        }

        Magic3D::Texture* texture = material->getTexture(0);
        if (texture)
        {
            texture->setWrap(ui->rbClamp1->isChecked());
        }

        if (ui->cmbTexture2->currentIndex() > 0)
        {
            material->setTexture(1, Magic3D::ResourceManager::getTextures()->get(ui->cmbTexture2->currentText().toStdString()));
        }
        else
        {
            material->setTexture(1, NULL);
        }

        texture = material->getTexture(1);
        if (texture)
        {
            texture->setWrap(ui->rbClamp2->isChecked());
        }

        if (ui->cmbShader->currentIndex() > 0)
        {
            material->setShader(Magic3D::ResourceManager::getShaders()->get(ui->cmbShader->currentText().toStdString()));
        }
        else
        {
            material->setShader(NULL);
        }

        needSave();

        mainWindow->update();
        mainWindow->object2dinfo->update();

        loadMaterial();
    }
}

bool MaterialInfo::checkSaved(bool reload)
{
    bool result = true;
    bool needReload = reload;
    if (material && ui->actSave->isEnabled())
    {
        int ret = Utils::confirmSave("Save Material", tr("The material has been modified."), tr("Do you want to save your changes?"), false);
        if (ret == QMessageBox::Save)
        {
            material->save();
        }
        else
        {
            material->load();
            needReload = true;
        }

        ui->actSave->setEnabled(false);

        if (needReload)
        {
            loadMaterial();
        }
    }
    return result;
}

void MaterialInfo::on_imgTexture1_clicked()
{
    setMaterialTexture(0);
}

void MaterialInfo::on_imgTexture2_clicked()
{
    setMaterialTexture(1);
}

void MaterialInfo::setMaterialTexture(int index)
{
    if (material)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Texture"), UTILS_DEFAULT_PATH + tr(mainWindow->getProject()->getPathData().c_str()) + tr(M3D_PATH_TEXTURE), tr(MGE_TEXTURE_FILTER) );
        if (!fileName.isEmpty())
        {
            bool ok;
            QString text = QInputDialog::getText(this, tr("New Texture"), tr("Texture name:"), QLineEdit::Normal, tr(""), &ok);
            if (ok && !text.isEmpty())
            {
                bool created = false;
                material->setTexture(index, Magic3D::ResourceManager::getInstance()->addTexture(Magic3D::eTEXTURE_DIFFUSE, text.toStdString(), Utils::getFileName(fileName).toLocal8Bit().data(), true, true, created));
                updateTextureCombo();
                loadMaterial();
                updateObject();
            }
            else if (ok)
            {
                Utils::message(tr("Invalid Name"), tr("\"") + text + tr("\" is an invalid name!"));
            }
        }
    }
}

void MaterialInfo::clearVariables()
{
    while (!variables.isEmpty())
    {
        MaterialVar* variable = variables.takeLast();
        if (variable)
        {
            ui->layoutShaderVars->removeWidget(variable);
            delete variable;
        }
    }
}

void MaterialInfo::on_imgTexture1_customContextMenuRequested(QPoint pos)
{
    if (pos.isNull())
    {

    }
    if (material)
    {
        material->setTexture(0, NULL);
        updateObject();
    }
}

void MaterialInfo::on_imgTexture2_customContextMenuRequested(QPoint pos)
{
    if (pos.isNull())
    {

    }
    if (material)
    {
        material->setTexture(1, NULL);
        updateObject();
    }
}

void MaterialInfo::on_rbAlpha_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbAdd_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbInvert_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbMultiply_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbDivide_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbColor_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_btnColorAmbient_clicked()
{
    if (material)
    {
        material->setColorAmbient(chooseColor(material->getColorAmbient()));

        updateObject();
    }
}

void MaterialInfo::on_btnColorDiffuse_clicked()
{
    if (material)
    {
        material->setColorDiffuse(chooseColor(material->getColorDiffuse()));

        updateObject();
    }
}

void MaterialInfo::on_btnColorEmission_clicked()
{
    if (material)
    {
        material->setColorEmission(chooseColor(material->getColorEmission()));

        updateObject();
    }
}

void MaterialInfo::on_btnColorSpecular_clicked()
{
    if (material)
    {
        material->setColorSpecular(chooseColor(material->getColorSpecular()));

        updateObject();
    }
}

void MaterialInfo::cmbMaterials_currentIndexChanged(int index)
{
    checkSaved(false);
    if (index > 0 && cmbMaterials->count() > index && cmbMaterials->itemText(index) != UTILS_NONE)
    {
        bool created = false;
        material = Magic3D::ResourceManager::getMaterials()->load(cmbMaterials->itemText(index).toStdString(), created);
    }
    else
    {
        material = NULL;
    }

    loadMaterial();
}

void MaterialInfo::on_sldShininess_valueChanged(int value)
{
    if (!updating && material)
    {
        material->setShininess(value);
        needSave();
    }
}

void MaterialInfo::on_cmbTexture1_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void MaterialInfo::on_cmbTexture2_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void MaterialInfo::on_txtAlphaTest_valueChanged(double arg1)
{
    if (arg1 > 0.0)
    {

    }
    updateObject();
}

void MaterialInfo::on_cmbShader_currentIndexChanged(int index)
{
    if (index)
    {

    }
    updateObject();
}

void MaterialInfo::on_actNew_triggered()
{
    checkSaved(false);
    bool needReload = true;
    bool ok;
    QString result = QInputDialog::getText(NULL, "New Material", "Name:", QLineEdit::Normal, tr(""), &ok);
    if (ok && !result.isEmpty() && !result.contains(" "))
    {
        if (!Magic3D::ResourceManager::getMaterials()->get(result.toStdString()))
        {
            bool created = false;
            Magic3D::ResourceManager::getMaterials()->load(result.toStdString(), created);
            mainWindow->updateMaterialsList();
            cmbMaterials->clear();
            cmbMaterials->addItems(mainWindow->getMaterialList());
            cmbMaterials->setCurrentIndex(cmbMaterials->findText(result));
            needReload = false;
        }
        else
        {
            Utils::message(tr("Invalid Name"), tr("Material named \"") + result + tr("\" already exists!"));
        }
    }
    else if (ok)
    {
        result.clear();
        Utils::message(tr("Invalid Name"), tr("\"") + result + tr("\" is an invalid name!"));
    }

    if (needReload)
    {
        loadMaterial();
    }
}

void MaterialInfo::on_actDelete_triggered()
{
    if (material && Utils::confirm("Delete Material", tr("The material will be deleted!"), tr("Do you want to continue?")))
    {
        QString fileName = tr(material->getMaterialFile().c_str());
        Magic3D::ResourceManager::getMaterials()->remove(material->getName());
        QFile::remove(fileName);

        mainWindow->updateMaterialsList();
        cmbMaterials->setCurrentIndex(cmbMaterials->findText(UTILS_NONE));
        mainWindow->setComboList(mainWindow->getMaterialList(), cmbMaterials);
    }
}

void MaterialInfo::on_actSave_triggered()
{
    ui->actSave->setEnabled(false);
    if (material)
    {
        material->save();
    }
}

void MaterialInfo::on_rbClamp1_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbRepeat1_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbClamp2_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}

void MaterialInfo::on_rbRepeat2_toggled(bool checked)
{
    if (checked)
    {

    }
    updateObject();
}
