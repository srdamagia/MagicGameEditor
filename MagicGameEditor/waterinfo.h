#ifndef WATERINFO_H
#define WATERINFO_H

#include <QWidget>
#include "magic3dobjectpanel.h"

namespace Ui {
class WaterInfo;
}

class WaterInfo : public Magic3DObjectPanel
{
    Q_OBJECT

public:
    explicit WaterInfo(MainWindow* mainWindow);
    ~WaterInfo();

    virtual void update();
    virtual void updateObject();

private slots:
    void on_txtLength_valueChanged(int arg1);

    void on_txtWidth_valueChanged(int arg1);

    void on_txtTileLength_valueChanged(double arg1);

    void on_txtTileWidth_valueChanged(double arg1);

    void on_btnUpdate_clicked();

private:
    Ui::WaterInfo *ui;
};

#endif // WATERINFO_H
