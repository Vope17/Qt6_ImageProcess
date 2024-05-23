#include "imgprocess.h"
imgProcess::imgProcess(QObject *parent)
    : QObject{parent}
{}

QImage imgProcess::Brightness(QImage *img, const int &brightness)
{
    QImage newImg = *img;
    for (int y = 0; y < newImg.height(); ++y) {
        for (int x = 0; x < newImg.width(); ++x) {
            QColor color = newImg.pixelColor(x, y);

            int r = color.red() + brightness;
            int g = color.green() + brightness;
            int b = color.blue() + brightness;

            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImg;
}

QImage imgProcess::Contrast(QImage *img, const int &contrast)
{
    QImage newImg = *img;
    for (int y = 0; y < newImg.height(); ++y) {
        for (int x = 0; x < newImg.width(); ++x) {
            QColor color = newImg.pixelColor(x, y);

            int r = (color.red() - 127) * contrast / 50 + 127;
            int g = (color.green() - 127) * contrast / 50 + 127;
            int b = (color.blue() - 127) * contrast / 50 + 127;

            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImg;
}

QImage imgProcess::RGBtoGray(QImage *img)
{
    QImage newImg = *img;
    for (int y = 0; y < newImg.height(); ++y) {
        for (int x = 0; x < newImg.width(); ++x) {
            QColor color = newImg.pixelColor(x, y);

            int gray = color.red() * 0.299 + color.green() * 0.587 + color.blue() * 0.114;

            gray = qBound(0, gray, 255);

            newImg.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }
    return newImg;
}

QImage imgProcess::GraytoRGB(QImage *img)
{
    QImage newImg = *img;
    for (int y = 0; y < newImg.height(); ++y) {
        for (int x = 0; x < newImg.width(); ++x) {
            QColor color = newImg.pixelColor(x, y);
            int r = color.red();
            int g = color.green();
            int b = color.blue();
            qDebug() << r << ' ' << g << ' ' << b;
            newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImg;
}

QImage imgProcess::GraytoBW(QImage *img, const int &threshold)
{
    QImage newImg = *img;
    for (int y = 0; y < newImg.height(); ++y) {
        for (int x = 0; x < newImg.width(); ++x) {
            QColor color = newImg.pixelColor(x, y);

            int gray = (color.red() * 0.299 + color.green() * 0.587 + color.blue() * 0.114) + 0.5;

            gray = qBound(0, gray, 255);
            gray = (gray > threshold ? 255 : 0);

            newImg.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }
    return newImg;
}


