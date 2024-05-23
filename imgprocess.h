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
    void Negative();
    void LeftFlip();
    void RightFlip();
    void VerticalFlip();
    void HorizontalFlip();
    void Equalization();
    void BrightAdjust();
    void Threshold(int&, int&, int&);
    void OtusThreshold();

    void RGBtoGray(int&, int&, int&);
    void GraytoRGB();
    void GraytoBW(const int&);

    QImage AllOperation(QImage*);

    int _brightnessValue;
    int _contrastValue;
    int _thresholdValue;
    bool _isGray;
    bool _isRGBtoGrayActivated;
    bool _isThresholdActivated;
private:
    QImage _newImg;
};

#endif // IMGPROCESS_H
