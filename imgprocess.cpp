#include "imgprocess.h"
imgProcess::imgProcess(QObject *parent)
    : QObject{parent}
{
    ResetAllValue();
}

void imgProcess::ResetAllValue()
{
    _brightnessValue = 0;
    _contrastValue = 100;
    _thresholdValue = 127;
    _isGray = false;
    _isRGBtoGrayActivated = false;
    _isThresholdActivated = false;
}

void imgProcess::Brightness(int &r, int &g, int &b)
{
    r = qBound(0, r + _brightnessValue, 255);
    g = qBound(0, g + _brightnessValue, 255);
    b = qBound(0, b + _brightnessValue, 255);
}

void imgProcess::Contrast(int &r, int &g, int &b)
{
    r = qBound(0, int((r - 127) * _contrastValue / 100.0 + 0.5) + 127, 255);
    g = qBound(0, int((g - 127) * _contrastValue / 100.0 + 0.5) + 127, 255);
    b = qBound(0, int((b - 127) * _contrastValue / 100.0 + 0.5) + 127, 255);
}

void imgProcess::RGBtoGray(int &r, int &g, int &b)
{
    int gray = (r * 0.299 + g * 0.587 + b * 0.114) + 0.5;
    gray = qBound(0, gray, 255);
    r = g = b = gray;
}

void imgProcess::GraytoRGB()
{
    for (int y = 0; y < _newImg.height(); ++y) {
        for (int x = 0; x < _newImg.width(); ++x) {
            QColor color = _newImg.pixelColor(x, y);
            int r = color.red();
            int g = color.green();
            int b = color.blue();
            _newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::Threshold(int &r, int &g, int &b)
{
    if (!(_isGray || _isRGBtoGrayActivated))
        return;

    r = g = b = (r >= _thresholdValue ? 255 : 0);
}

QImage imgProcess::AllOperation(QImage *img)
{
    _newImg = *img;
    int r, g, b;
    QColor color;
    for (int y = 0; y < _newImg.height(); ++y) {
        for (int x = 0; x < _newImg.width(); ++x) {
            color = _newImg.pixelColor(x, y);
            r = color.red(), g = color.green(), b = color.blue();

            Brightness(r, g, b);
            Contrast(r, g, b);
            if (_isRGBtoGrayActivated)
                RGBtoGray(r, g, b);
            if (_isThresholdActivated)
                Threshold(r, g, b);
            _newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    qDebug() << "123hihiih";
    return _newImg;
}



