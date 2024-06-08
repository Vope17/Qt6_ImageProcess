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
    void initToolOptions();

public slots:
    void SlotGetImage(QImage*);
    void SlotUnRedo(QImage*);

private slots:
    void SlotChangeWidgetI();
    void SlotChangeWidgetII();
    void SlotChangeWidgetIII();
    void SlotChangeWidgetIV();

    // basic
    void SlotChangeBrightness(int);
    void SlotChangeContrast(int);
    void SlotChangeSaturation(int);
    void SlotCheckNegative(Qt::CheckState);

    // filter
    void SlotChangeMaskSize(int);
    void SlotUpdateFilterKernel();
    void SlotCustomLowPassFilter();
    void SlotAverage();
    void SlotGaussian();
    void SlotMaximum();
    void SlotMinimum();
    void SlotMedian();

    void SlotUpdateHighPassFilterKernelX();
    void SlotUpdateHighPassFilterKernelY();
    void SlotCustomHighPassFilter();
    void SlotSobel();
    void SlotPrewitt();
    void SlotSharpen();

    // convert
    void SlotHorizontalFlip();
    void SlotVerticalFlip();
    void SlotLeftFlip();
    void SlotRightFlip();
    void SlotChangeZoomImplementation(bool);
    void SlotChangeZoom(int);
    void SlotRotate(int);

    void SlotCheckRGBtoGray(Qt::CheckState);
    void SlotCheckThreshold(Qt::CheckState);
    void SlotChangeThreshold(int);
    void SlotOtus();
    void SlotEqualization();
    void SlotGrayToAscii();

    // morphology
    void SlotChangeKernelSize(int value);
    void SlotUpdateKernel();
    void SlotDilation();
    void SlotErosion();
    void SlotOpening();
    void SlotClosing();
    void SlotBoundaryDetection();
    void SlotNoiseRemoval();
private:
    QImage _newImg;
    QImage *_previewImg;
    QPixmap _previewPixmap;
    QPixmap _activate_on;
    QPixmap _activate_off;

signals:
    void SigUpdatePreviewImg(QImage);
    void SigUpdatePixmap();
};

#endif // TOOLWIDGET_H
