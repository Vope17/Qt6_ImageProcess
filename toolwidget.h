#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include "imgprocess.h"
namespace Ui {
class toolWidget;
}

class toolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit toolWidget(QWidget *parent = nullptr);
    ~toolWidget();

private:
    Ui::toolWidget *ui;
    QImage *_img;
    imgProcess *_ip = new imgProcess(this);

    void initIcons();
public slots:
    void SlotGetImage(QImage*);

private slots:
    void SlotChangeWidgetI();
    void SlotChangeWidgetII();
    void SlotChangeWidgetIII();

    void SlotChangeBrightness(int);
    void SlotChangeContrast(int);
    void SlotConvertRGBtoGray();
    void SlotChangeThreshold(int);

    void SlotCheckRGBtoGray(Qt::CheckState);
    void SlotCheckThreshold(Qt::CheckState);

private:
    QImage _newImg;
    QPixmap _activate_on;
    QPixmap _activate_off;

signals:
    void SigUpdatePixmap(QImage);
};

#endif // TOOLWIDGET_H
