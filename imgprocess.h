#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include <QObject>
#include <QImage>
#include <QDebug>
class imgProcess : public QObject
{
    Q_OBJECT
public:
    explicit imgProcess(QObject *parent = nullptr);
    QImage Brightness(QImage*, const int&);
    QImage Contrast(QImage*, const int&);
    void Negative(QImage);
    void LeftFlip(QImage);
    void RightFlip(QImage);
    void VerticalFlip(QImage);
    void HorizontalFlip(QImage);
    void Equalization(QImage);
    void BrightAdjust(QImage);
    void Threshold(QImage);
    void OtusThreshold(QImage);

    QImage RGBtoGray(QImage*);
    QImage GraytoRGB(QImage*);
    QImage GraytoBW(QImage*, const int&);

};

#endif // IMGPROCESS_H
