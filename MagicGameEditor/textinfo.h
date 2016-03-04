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
