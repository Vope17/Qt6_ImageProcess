#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QLabel>
class imgProcess : public QObject
{
    Q_OBJECT
public:
    explicit imgProcess(QObject *parent = nullptr);
    void ResetAllValue();
    void Brightness(int&, int&, int&);
    void Contrast(int&, int&, int&);
    void Saturation(int&, int&, int&);
    void Negative(int &r, int &g, int &b);
    void BrightAdjust();

    void CustomLowPassFilter(QImage*);
    void Average(QImage*);
    void Gaussian(QImage*);
    void Maximum(QImage*);
    void Minimum(QImage*);
    void Median(QImage*);

    void CustomHighPassFilter(QImage*);
    void Sobel(QImage*);
    void Prewitt(QImage*);
    void Sharpen(QImage*);

    void LeftFlip(QImage*);
    void RightFlip(QImage*);
    void VerticalFlip(QImage*);
    void HorizontalFlip(QImage*);
    QImage ZoomInterPolation(QImage);
    QImage ZoomNeighbouring(QImage);
    QImage Rotate(QImage);
    void RGBEqualization(QImage*);
    void Equalization(QImage*);
    void RGBtoGray(int&, int&, int&);
    void Threshold(int&, int&, int&);
    int OtusThreshold(QImage*);
    void GrayToAscii(QImage);

    void Dilation(QImage*);
    void Erosion(QImage*);
    void Opening(QImage*);
    void Closing(QImage*);
    void BoundaryDetection(QImage*);
    void NoiseRemoval(QImage*);

    QImage BasicOperation(QImage*);
    QImage ConvertOperation(QImage*);
    int _h;
    int _w;

    int _brightnessValue;
    int _contrastValue;
    double _saturationValue;
    int _thresholdValue;
    double _zoomValue;
    int _rotateValue;

    int _maskSize;
    bool _isGray;
    bool _isNegativeActivated;
    bool _isRGBtoGrayActivated;
    bool _isThresholdActivated;
    bool _isInterPolation;

    QVector<QVector<int>> _customLowPassFilterKernel;
    QVector<QVector<int>> _customHighPassFilterKernelX;
    QVector<QVector<int>> _customHighPassFilterKernelY;
    QVector<QVector<int>> _kernel;
private:
    QImage _newImg;
    QImage _oldImg;
};

#endif // IMGPROCESS_H
