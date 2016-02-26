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

#ifndef MATERIALINFO_H
#define MATERIALINFO_H

#include <QGraphicsScene>
#include <QThread>
#include "materialvar.h"

namespace Ui {
    class MaterialInfo;
}

namespace Magic3D
{
    class Texture;
    class Image;
    class Material;
}

QImage* getImage(std::string fileName, bool png);

class LoadThread : public QThread
{
    Q_OBJECT
public:
    QPushButton* button;
    QString fileName;
    Magic3D::Texture* texture;
    int index;

    LoadThread(QPushButton* button, const QString& fileName, Magic3D::Texture* texture, int index);
    ~LoadThread();
signals:
    void finished(QImage* image, QPushButton* button, Magic3D::Texture* texture, int index);
protected:
    virtual void run();
};

class MaterialInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit MaterialInfo(MainWindow* mainWindow);
    ~MaterialInfo();

    virtual void update();
    virtual void updateObject();

    void updateTextureCombo();
    void updateShaderCombo();

    bool checkSaved(bool reload);
    void needSave();
    void cancel();
private:
    Ui::MaterialInfo *ui;

    Magic3D::Material* material;

    LoadThread* tga1;
    LoadThread* tga2;

    QToolBar* toolbar;
    QComboBox* cmbMaterials;

    QStringList texturesList;
    QList<MaterialVar*> variables;

    void setMaterialTexture(int index);
    void clearVariables();

    void loadMaterial();

private slots:
    void cmbMaterials_currentIndexChanged(int index);
    void on_cmbTexture2_currentIndexChanged(int index);
    void on_cmbTexture1_currentIndexChanged(int index);
    void loadFinished(QImage* image, QPushButton* button, Magic3D::Texture* texture, int index);
    void on_sldShininess_valueChanged(int value);
    void on_btnColorSpecular_clicked();
    void on_btnColorEmission_clicked();
    void on_btnColorDiffuse_clicked();
    void on_btnColorAmbient_clicked();
    void on_rbColor_toggled(bool checked);
    void on_rbDivide_toggled(bool checked);
    void on_rbMultiply_toggled(bool checked);
    void on_rbInvert_toggled(bool checked);
    void on_rbAdd_toggled(bool checked);
    void on_rbAlpha_toggled(bool checked);
    void on_imgTexture2_customContextMenuRequested(QPoint pos);
    void on_imgTexture1_customContextMenuRequested(QPoint pos);
    void on_imgTexture2_clicked();
    void on_imgTexture1_clicked();
    void on_txtAlphaTest_valueChanged(double arg1);
    void on_cmbShader_currentIndexChanged(int index);
    void on_actNew_triggered();
    void on_actDelete_triggered();
    void on_actSave_triggered();
    void on_rbClamp1_toggled(bool checked);
    void on_rbRepeat1_toggled(bool checked);
    void on_rbClamp2_toggled(bool checked);
    void on_rbRepeat2_toggled(bool checked);
};

#endif // MATERIALINFO_H
