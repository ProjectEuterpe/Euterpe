#ifndef FLOATPAN_H
#define FLOATPAN_H

#include <QWidget>
#include <ui_FloatPan.h>

namespace Ui {
class FloatTable;
}

class FloatTable : public QWidget{
    Q_OBJECT

public:
    explicit FloatTable(QWidget *parent = nullptr);
    ~FloatTable();
    void setCustomText(QString text);
    void setCustomPos(float x, float y);

private:
    Ui::FloatTable *ui;
};

#endif // FLOATPAN_H
