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
    QImage _newImg;
    imgProcess *_ip = new imgProcess(this);

    void initIcons();
public slots:
    void SlotGetImage(QImage*);

private slots:
    void SlotChangeWidgetI();
    void SlotChangeWidgetII();
    void SlotChangeWidgetIII();

    void SlotChangeBrightnessLabelValue(int);
    void SlotChangeContrastLabelValue(int);

    void SlotChangeBrightness(int);
    void SlotChangeContrast(int);
    void SlotConvertRGBtoGray();
    void SlotConvertGraytoRGB();
    void SlotChangeGraytoBWLabelValue(int);
    void SlotConvertGraytoBW(int);

signals:
    void SigUpdateImg(QImage);
    void SigUpdatePixmap(QImage);
};

#endif // TOOLWIDGET_H
