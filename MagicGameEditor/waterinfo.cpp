#include "waterinfo.h"
#include "ui_waterinfo.h"

WaterInfo::WaterInfo(MainWindow* mainWindow) : Magic3DObjectPanel(mainWindow),
    ui(new Ui::WaterInfo)
{
    ui->setupUi(this);
}

WaterInfo::~WaterInfo()
{
    delete ui;
}

void WaterInfo::update()
{
    updating = true;
    Magic3D::Object* object = getObject();
    if (object && object->getType() == Magic3D::eOBJECT_WATER)
    {
        Magic3D::Water* water = (Magic3D::Water*)object;

        ui->txtLength->setValue(water->getLength());
        ui->txtWidth->setValue(water->getWidth());
        ui->txtTileLength->setValue(water->getTileLength());
        ui->txtTileWidth->setValue(water->getTileWidth());
    }
    updating = false;
}

void WaterInfo::updateObject()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getType() == Magic3D::eOBJECT_WATER)
    {
        Magic3D::Water* water = (Magic3D::Water*)object;

        water->setSize(ui->txtLength->value(), ui->txtWidth->value(), ui->txtTileLength->value(), ui->txtTileWidth->value());

        update();
        mainWindow->setProjectChanged(true);
        mainWindow->update();
    }
}

void WaterInfo::on_txtLength_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void WaterInfo::on_txtWidth_valueChanged(int arg1)
{
    if (arg1 > 0)
    {

    }
    updateObject();
}

void WaterInfo::on_txtTileLength_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void WaterInfo::on_txtTileWidth_valueChanged(double arg1)
{
    if (arg1 > 0.0f)
    {

    }
    updateObject();
}

void WaterInfo::on_btnUpdate_clicked()
{
    Magic3D::Object* object = getObject();
    if (!updating && object && object->getType() == Magic3D::eOBJECT_WATER)
    {
        Magic3D::Water* water = (Magic3D::Water*)object;
        water->generateWater();
    }
}
