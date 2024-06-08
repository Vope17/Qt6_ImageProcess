#include "imgprocess.h"
#include <QHBoxLayout>
imgProcess::imgProcess(QObject *parent)
    : QObject{parent}
{
    ResetAllValue();
    _kernel = QVector<QVector<int>>(3, QVector<int>(3, 1));
}

void imgProcess::ResetAllValue()
{
    _maskSize = 3;
    _brightnessValue = 0;
    _contrastValue = 100;
    _zoomValue = 1.0;
    _rotateValue = 0;
    _thresholdValue = 127;
    _isNegativeActivated = false;
    _isRGBtoGrayActivated = false;
    _isThresholdActivated = false;
    _isInterPolation = true;
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

void imgProcess::Saturation(int &r, int &g, int &b)
{
    // RGB to HSV
    int mx = std::max({r, g, b});
    int mn = std::min({r, g, b});
    int V = mx;
    double C = mx - mn;
    int H = 0;
    if (C == 0)
        H = 0;
    else if (mx == r && g >= b)
        H = 60 * (g - b) / C + 0;
    else if (mx == r && g < b)
        H = 60 * (g - b) / C + 360;
    else if (mx == g)
        H = 60 * (b - r) / C + 120;
    else if (mx == b)
        H = 60 * (r - g) / C + 240;

    // HSV to RGB
    int b_h = H / 60;
    double f = H / 60 - b_h;
    double p = V * (1 - _saturationValue);
    double q = V * (1 - f * _saturationValue);
    double t = V * (1 - (1 - f) * _saturationValue);
    int tmp_r, tmp_g, tmp_b;

    switch(b_h)
    {
    case 0:
        r = V, g = t, b = p;
        break;
    case 1:
        r = q, g = V, b = p;
        break;
    case 2:
        r = p, g = V, b = q;
        break;
    case 3:
        r = p, g = q, b = V;
        break;
    case 4:
        r = t, g = p, b = V;
        break;
    case 5:
        r = V, g = p, b = q;
        break;
    }

}

void imgProcess::Negative(int &r, int &g, int &b)
{
    r ^= 255;
    g ^= 255;
    b ^= 255;
}

void imgProcess::CustomLowPassFilter(QImage *img)
{
    if (!img || img->isNull())
        return;
    int HSize = _customLowPassFilterKernel.size();
    int radiusH = HSize / 2;
    QImage oldImg = img->copy();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int sum_r, sum_g, sum_b;
    int w = img->width();
    int h = img->height();
    int avg = 0;
    for (const auto &v : _customLowPassFilterKernel)
        for (const int &x : v)
            avg += x;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            sum_r = sum_g = sum_b = 0;
            for (int i = -radiusH; i <= radiusH; ++i)
            {
                if (i + radiusH < 0 || i + radiusH >= HSize)
                    continue;
                int WSize = _customLowPassFilterKernel[i + radiusH].size();
                if (WSize == 0)
                    continue;
                int radiusW = WSize / 2;
                for (int j = -radiusW; j <= radiusW; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    if (i + radiusH < HSize && i + radiusH >= 0 &&
                        j + radiusW < WSize && j + radiusW >= 0)
                    {
                        sum_r += old_r * _customLowPassFilterKernel[i + radiusH][j + radiusW];
                        sum_g += old_g * _customLowPassFilterKernel[i + radiusH][j + radiusW];
                        sum_b += old_b * _customLowPassFilterKernel[i + radiusH][j + radiusW];
                    }
                }
            }
            if (avg != 0)
            {
                r = qBound(0, qRound(sum_r / static_cast<double>(avg)), 255);
                g = qBound(0, qRound(sum_g / static_cast<double>(avg)), 255);
                b = qBound(0, qRound(sum_b / static_cast<double>(avg)), 255);
            }
            else
            {
                r = qBound(0, sum_r, 255);
                g = qBound(0, sum_g, 255);
                b = qBound(0, sum_b, 255);
            }
            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::RGBEqualization(QImage *img)
{
    /*
        Y=0.299R+0.587G+0.114B
        Cb=−0.168736R−0.331264G+0.5B+128
        Cr=0.5R−0.418688G−0.081312B+128

        R=Y+1.402(Cr−128)
        G=Y−0.344136(Cb−128)−0.714136(Cr−128)
        B=Y+1.772(Cb−128)
    */

    QImage oldImg = img->copy();
    int h = img->height();
    int w = img->width();
    QColor oldColor;
    int r, g, b;

    QVector<int> YCount(256, 0);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            oldColor = oldImg.pixelColor(x, y);
            r = oldColor.red(), g = oldColor.green(), b = oldColor.blue();
            YCount[0.299 * r + 0.587 * g + 0.114 * b]++;
        }
    }

    QVector<int> YAccumulation(256, 0);
    YAccumulation[0] = YCount[0];
    for (int i = 1; i < 256; ++i)
    {
        YAccumulation[i] = YAccumulation[i - 1] + YCount[i];
    }

    QVector<int> YMap(256, 0);
    for (int i = 0; i < 256; ++i)
    {
        YMap[i] = YAccumulation[i] * 255 / double(YAccumulation[255]) + 0.5;
    }

    int yVal, cb, cr;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            oldColor = oldImg.pixelColor(x, y);
            r = oldColor.red(), g =  oldColor.green(), b = oldColor.blue();
            yVal = 0.299 * r + 0.587 * g + 0.114 * b;
            cb = -0.169 * r - 0.331 * g + 0.5 * b + 128;
            cr = 0.5 * r - 0.419 * g - 0.081 * b + 128;
            yVal = YMap[yVal];

            r = qBound(0, static_cast<int>(yVal + 1.4 * (cr - 128)), 255);
            g = qBound(0, static_cast<int>(yVal - 0.343 * (cb - 128) - 0.711 * (cr - 128)), 255);
            b = qBound(0, static_cast<int>(yVal + 1.765 * (cb - 128)), 255);
            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::Equalization(QImage *img)
{
    if (!img)
        return;

    RGBEqualization(img);

    /*
    QImage oldImg = img->copy();
    int h = img->height();
    int w = img->width();
    QColor oldColor;
    int r, g, b;
    QVector<int> redCount(256, 0);
    QVector<int> greenCount(256, 0);
    QVector<int> blueCount(256, 0);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            oldColor = oldImg.pixelColor(x, y);
            redCount[oldColor.red()]++;
            greenCount[oldColor.green()]++;
            blueCount[oldColor.blue()]++;
        }
    }

    QVector<int> redAccumulation(256, 0);
    QVector<int> greenAccumulation(256, 0);
    QVector<int> blueAccumulation(256, 0);
    redAccumulation[0] = redCount[0];
    greenAccumulation[0] = greenCount[0];
    blueAccumulation[0] = blueCount[0];
    for (int i = 1; i < 256; ++i)
    {
        redAccumulation[i] = redAccumulation[i - 1] + redCount[i];
        greenAccumulation[i] = greenAccumulation[i - 1] + greenCount[i];
        blueAccumulation[i] = blueAccumulation[i - 1] + blueCount[i];
    }

    QVector<int> redMap(256, 0);
    QVector<int> greenMap(256, 0);
    QVector<int> blueMap(256, 0);
    for (int i = 0; i < 256; ++i)
    {
        redMap[i] = redAccumulation[i] * 255 / double(redAccumulation[255]) + 0.5;
        greenMap[i] = greenAccumulation[i] * 255 / double(greenAccumulation[255]) + 0.5;
        blueMap[i] = blueAccumulation[i] * 255 / double(blueAccumulation[255]) + 0.5;
    }

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            oldColor = oldImg.pixelColor(x, y);
            r = redMap[oldColor.red()];
            g = greenMap[oldColor.green()];
            b = blueMap[oldColor.blue()];
            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
    */
}

void imgProcess::RGBtoGray(int &r, int &g, int &b)
{
    int gray = (r * 0.299 + g * 0.587 + b * 0.114) + 0.5;
    gray = qBound(0, gray, 255);
    r = g = b = gray;
}

void imgProcess::Threshold(int &r, int &g, int &b)
{
    int tmp = qGray(r, g, b);
    r = g = b = (tmp >= _thresholdValue ? 255 : 0);
}

int imgProcess::OtusThreshold(QImage *img)
{
    if (!img)
        return 127;

    int h = img->height();
    int w = img->width();
    QVector<int> gray(256, 0);
    QColor color;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = img->pixelColor(x, y);
            int tmp = qGray(color.red(), color.green(), color.blue());
            gray[tmp]++;
        }
    }

    int sum = 0;
    for (int i = 0; i < 256; ++i)
        sum += gray[i];
    QVector<double> p(256, 0);
    for (int i = 0; i < 256; ++i)
        p[i] = gray[i] / double(sum);
    double sigma = 0xfffff;
    double w1, w2, u1, u2, s1, s2;
    int pos = 0;
    for (int t = 1; t < 256; ++t)
    {
        w1 = w2 = u1 = u2 = s1 = s2 = 0.0;
        for (int j = 0; j <= t; ++j)
            w1 += p[j];

        for (int j = t + 1; j < 256; ++j)
            w2 += p[j];

        for (int j = 0; j <= t; ++j)
            u1 += (j * p[j]) / w1;

        for (int j = t + 1; j < 256; ++j)
            u2 += (j * p[j]) / w2;

        for (int j = 0; j <= t; ++j)
            s1 += (j - u1) * (j - u1) * (p[j] / w1);

        for (int j = t + 1; j < 256; ++j)
            s2 += (j - u2) * (j - u2) * (p[j] / w2);

        double tmp = w1 * s1 + w2 * s2;
        if (sigma > tmp)
        {
            sigma = tmp;
            pos = t;
        }
    }
    int r, g, b;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = img->pixelColor(x, y);
            int tmp = qGray(color.red(), color.green(), color.blue());
            tmp = (tmp > pos ? 255 : 0);
            img->setPixelColor(x, y, QColor(tmp, tmp, tmp));
        }
    }
    return pos;
}

void imgProcess::GrayToAscii(QImage img)
{
    int h = img.height();
    int w = img.width();
    QColor oldColor;
    int gray;
    //QString map = " .,:;ox%#@";
    QString map = " ~!@#$%^&*+";
    QString art = "";
    for (int y = 0; y < h; y += 2)
    {
        for (int x = 0; x < w; ++x)
        {
            oldColor = img.pixelColor(x, y);
            gray = qGray(oldColor.red(), oldColor.green(), oldColor.blue());
            art += map[gray * 10 / 255];
        }
        art += "\n";
    }
    QWidget *win = new QWidget();
    win->setAttribute(Qt::WA_DeleteOnClose);
    QLabel *asciiArt = new QLabel(art);
    asciiArt->setFont(QFont("Courier", 3));
    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(asciiArt);
    lay->setAlignment(Qt::AlignCenter);
    win->setLayout(lay);
    win->showMaximized();
}

void imgProcess::Dilation(QImage *img)
{
    if (!img)
        return;
    QImage oldImg = img->copy();
    int w = img->width();
    int h = img->height();
    int gray;
    int HSize = _kernel.size() % 2 ? _kernel.size() : _kernel.size() - 1;
    int radiusH = HSize / 2;
    QColor oldColor;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            bool isDilate = false;
            for (int i = -radiusH; i <= radiusH; ++i)
            {
                int WSize = _kernel[i + radiusH].size();
                if (WSize == 0)
                    continue;
                int radiusW = WSize / 2;
                for (int j = -radiusW; j <= radiusW; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    gray = qGray(oldColor.red(), oldColor.green(), oldColor.blue());
                    if (i + radiusH < HSize && i + radiusH >= 0 &&
                        j + radiusW < WSize && j + radiusW >= 0)
                    {
                        if (_kernel[i + radiusH][j + radiusW] == 1 && gray == 255)
                        {
                            isDilate= true;
                            goto goOut;
                        }
                    }
                }
            }
        goOut:
            if (isDilate)
            {
                img->setPixelColor(x, y, QColor(255, 255, 255));
            }
            else
            {
                img->setPixelColor(x, y, QColor(0, 0, 0));
            }
        }
    }
}

void imgProcess::Erosion(QImage *img)
{
    if (!img)
        return;

    QImage oldImg = img->copy();
    int w = oldImg.width();
    int h = oldImg.height();
    int gray;
    int HSize = _kernel.size() % 2 ? _kernel.size() : _kernel.size() - 1;
    int radiusH = HSize / 2;
    int r, g, b;
    QColor oldColor;
    for (int y = 0; y < img->height(); ++y)
    {
        for (int x = 0; x < img->width(); ++x)
        {
            bool isOK = true;
            for (int i = -radiusH; i <= radiusH; ++i)
            {
                int WSize = _kernel[i + radiusH].size();
                if (WSize == 0)
                    continue;
                int radiusW = WSize / 2;
                for (int j = -radiusW; j <= radiusW; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                    {
                        isOK = false;
                        goto goOut;
                    }
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    gray = qGray(oldColor.red(), oldColor.green(), oldColor.blue());
                    if (i + radiusH < HSize && i + radiusH >= 0 &&
                        j + radiusW < WSize && j + radiusW >= 0)
                    {
                        if (_kernel[i + radiusH][j + radiusW] == 1 && gray == 0)
                        {
                            isOK = false;
                            goto goOut;
                        }
                    }
                }
            }
        goOut:
            if (!isOK)
            {
                img->setPixelColor(x, y, QColor(0, 0, 0));
            }
            else
            {
                img->setPixelColor(x, y, QColor(255, 255, 255));
            }
        }
    }
}

void imgProcess::Opening(QImage *img)
{
    Erosion(img);
    Dilation(img);
}

void imgProcess::Closing(QImage *img)
{
    Dilation(img);
    Erosion(img);
}

void imgProcess::BoundaryDetection(QImage *img)
{
    QImage tmpDilation = img->copy();
    QImage tmpErosion = img->copy();
    QColor dilationColor;
    QColor erosionColor;
    int tmpD, tmpE, tmp;
    Dilation(&tmpDilation);
    Erosion(&tmpErosion);
    for (int y = 0; y < tmpDilation.height(); ++y)
    {
        for (int x = 0; x < tmpDilation.width(); ++x)
        {
            dilationColor = tmpDilation.pixelColor(x, y);
            erosionColor = tmpErosion.pixelColor(x, y);
            tmpD = dilationColor.red();
            tmpE = erosionColor.red();
            tmp = tmpD ^ tmpE;
            img->setPixelColor(x, y, QColor(tmp, tmp, tmp));
        }
    }
}

void imgProcess::NoiseRemoval(QImage *img)
{
    Opening(img);
    Closing(img);
}

void imgProcess::Average(QImage *img)
{
    if (!img || img->isNull())
        return;
    int radius = _maskSize/ 2;
    QImage oldImg = img->copy();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int sum_r, sum_g, sum_b;
    int w = img->width();
    int h = img->height();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            sum_r = sum_g = sum_b = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    sum_r += old_r;
                    sum_g += old_g;
                    sum_b += old_b;
                }
            }
            int avg = _maskSize * _maskSize;
            r = (sum_r / avg);
            g = (sum_g / avg);
            b = (sum_b / avg);

            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::Gaussian(QImage *img)
{
    if (!img)
        return;

    int radius = _maskSize / 2;
    QVector<QVector<double>> mask(_maskSize, QVector<double>(_maskSize, 0));
    int mH = _maskSize, mW = _maskSize;
    double x, y;
    double sigma = 1.0;
    double s = 2.0 * sigma * sigma;
    double sum = 0.0;
    for (int i = -radius; i <= radius; ++i)
    {
        y = i * i;
        for (int j = -radius; j <= radius; ++j)
        {
            x = j * j;
            double g = qExp(- (x + y) / s);
            mask[i + radius][j + radius] = g;
            sum += g;
        }
    }

    for (int i = 0; i < mH; ++i)
        for (int j = 0; j < mW; ++j)
            mask[i][j] /= sum;
    int h = img->height();
    int w = img->width();
    QImage oldImg = img->copy();
    QColor oldColor;
    int old_r, old_g, old_b;
    int sum_r, sum_g, sum_b;
    int r, g, b;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            sum_r = sum_g = sum_b = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    sum_r += old_r * mask[radius + i][radius + j];
                    sum_g += old_g * mask[radius + i][radius + j];
                    sum_b += old_b * mask[radius + i][radius + j];
                }
            }
            r = sum_r;
            g = sum_g;
            b = sum_b;

            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::Maximum(QImage *img)
{
    if (!img)
        return;
    int radius = _maskSize/ 2;
    QImage oldImg = img->copy();
    int w = img->width();
    int h = img->height();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int mx_r, mx_g, mx_b;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            mx_r = mx_g = mx_b = 255;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    mx_r = qMin(mx_r, old_r);
                    mx_g = qMin(mx_g, old_g);
                    mx_b = qMin(mx_b, old_b);
                }
            }
            r = mx_r;
            g = mx_g;
            b = mx_b;

            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::Minimum(QImage *img)
{
    if (!img)
        return;
    int radius = _maskSize/ 2;
    QImage oldImg = img->copy();
    int w = img->width();
    int h = img->height();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int mn_r, mn_g, mn_b;
    for (int y = 0; y < img->height(); ++y)
    {
        for (int x = 0; x < img->width(); ++x)
        {
            mn_r = mn_g = mn_b = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    mn_r = qMax(mn_r, old_r);
                    mn_g = qMax(mn_g, old_g);
                    mn_b = qMax(mn_b, old_b);
                }
            }
            r = mn_r;
            g = mn_g;
            b = mn_b;

            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}

void imgProcess::Median(QImage *img)
{
    if (!img)
        return;
    int radius = _maskSize/ 2;
    int n = _maskSize * _maskSize;
    QImage oldImg = img->copy();
    int w = img->width();
    int h = img->height();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int m_r, m_g, m_b;
    QVector<int> nums_r(n, 0);
    QVector<int> nums_g(n, 0);
    QVector<int> nums_b(n, 0);
    for (int y = 0; y < img->height(); ++y)
    {
        for (int x = 0; x < img->width(); ++x)
        {
            int k = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    nums_r[k] = old_r;
                    nums_g[k] = old_g;
                    nums_b[k] = old_b;
                    k++;
                }
            }
            auto it_r = nums_r.begin() + n / 2;
            auto it_g = nums_g.begin() + n / 2;
            auto it_b = nums_b.begin() + n / 2;

            std::nth_element(nums_r.begin(), it_r, nums_r.end());
            std::nth_element(nums_g.begin(), it_g, nums_g.end());
            std::nth_element(nums_b.begin(), it_b, nums_b.end());

            r = *it_r;
            g = *it_g;
            b = *it_b;
            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }

}

void imgProcess::CustomHighPassFilter(QImage *img)
{
    if (!img || img->isNull())
        return;
    int XHSize = _customHighPassFilterKernelX.size();
    int YHSize = _customHighPassFilterKernelY.size();
    int XradiusH = XHSize / 2;
    int YradiusH = YHSize / 2;
    QImage oldImg = img->copy();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int sumX_r, sumX_g, sumX_b;
    int sumY_r, sumY_g, sumY_b;
    int w = img->width();
    int h = img->height();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            sumX_r = sumX_g = sumX_b = 0;
            sumY_r = sumY_g = sumY_b = 0;
            if (XHSize != 0)
            {
                for (int i = -XradiusH; i <= XradiusH; ++i)
                {
                    if (i + XradiusH < 0 || i + XradiusH >= XHSize)
                        continue;
                    int XWSize = _customHighPassFilterKernelX[i + XradiusH].size();
                    if (XWSize == 0)
                        continue;
                    int XradiusW = XWSize / 2;
                    for (int j = -XradiusW; j <= XradiusW; ++j)
                    {
                        if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                            continue;
                        oldColor = oldImg.pixelColor(x + j, y + i);
                        old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                        if (i + XradiusH < XHSize && i + XradiusH >= 0 &&
                            j + XradiusW < XWSize && j + XradiusW >= 0)
                        {
                            sumX_r += old_r * _customHighPassFilterKernelX[i + XradiusH][j + XradiusW];
                            sumX_g += old_g * _customHighPassFilterKernelX[i + XradiusH][j + XradiusW];
                            sumX_b += old_b * _customHighPassFilterKernelX[i + XradiusH][j + XradiusW];
                        }
                    }
                }
            }
            if (YHSize != 0)
            {
                for (int i = -YradiusH; i <= YradiusH; ++i)
                {
                    if (i + YradiusH < 0 || i + YradiusH >= YHSize)
                        continue;
                    int YWSize = _customHighPassFilterKernelY[i + YradiusH].size();
                    if (YWSize == 0)
                        continue;
                    int YradiusW = YWSize / 2;
                    for (int j = -YradiusW; j <= YradiusW; ++j)
                    {
                        if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                            continue;
                        oldColor = oldImg.pixelColor(x + j, y + i);
                        old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                        if (i + YradiusH < YHSize && i + YradiusH >= 0 &&
                            j + YradiusW < YWSize && j + YradiusW >= 0)
                        {
                            sumY_r += old_r * _customHighPassFilterKernelY[i + YradiusH][j + YradiusW];
                            sumY_g += old_g * _customHighPassFilterKernelY[i + YradiusH][j + YradiusW];
                            sumY_b += old_b * _customHighPassFilterKernelY[i + YradiusH][j + YradiusW];
                        }
                    }
                }
            }
            int magnitude_r = qBound(0, static_cast<int>(qSqrt(sumX_r * sumX_r + sumY_r * sumY_r)), 255);
            int magnitude_g = qBound(0, static_cast<int>(qSqrt(sumX_g * sumX_g + sumY_g * sumY_g)), 255);
            int magnitude_b = qBound(0, static_cast<int>(qSqrt(sumX_b * sumX_b + sumY_b * sumY_b)), 255);
            img->setPixelColor(x, y, QColor(magnitude_r, magnitude_g, magnitude_b));
        }
    }
}

void imgProcess::Sobel(QImage *img)
{
    if (!img)
        return;
    QVector<QVector<int>> sobel_X= {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    QVector<QVector<int>> sobel_Y = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int radius = 3 / 2;
    int h = img->height();
    int w = img->width();
    QImage oldImg = img->copy();
    QColor oldColor;
    int old_r, old_g, old_b;
    int sum_r, sum_g, sum_b;
    int r, g, b;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int sumX_r = 0, sumX_g = 0, sumX_b = 0;
            int sumY_r = 0, sumY_g = 0, sumY_b = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    sumX_r += old_r * sobel_X[radius + i][radius + j];
                    sumX_g += old_g * sobel_X[radius + i][radius + j];
                    sumX_b += old_b * sobel_X[radius + i][radius + j];
                    sumY_r += old_r * sobel_Y[radius + i][radius + j];
                    sumY_g += old_g * sobel_Y[radius + i][radius + j];
                    sumY_b += old_b * sobel_Y[radius + i][radius + j];
                }
            }
            int magnitude_r = qBound(0, static_cast<int>(qSqrt(sumX_r * sumX_r + sumY_r * sumY_r)), 255);
            int magnitude_g = qBound(0, static_cast<int>(qSqrt(sumX_g * sumX_g + sumY_g * sumY_g)), 255);
            int magnitude_b = qBound(0, static_cast<int>(qSqrt(sumX_b * sumX_b + sumY_b * sumY_b)), 255);
            img->setPixelColor(x, y, QColor(magnitude_r, magnitude_g, magnitude_b));
        }
    }
}

void imgProcess::Prewitt(QImage *img)
{
    if (!img)
        return;

    QVector<QVector<int>> prewitt_X= {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    QVector<QVector<int>> prewitt_Y= {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};

    int radius = 3 / 2;
    int h = img->height();
    int w = img->width();
    QImage oldImg = img->copy();
    QColor oldColor;
    int old_r, old_g, old_b;
    int sum_r, sum_g, sum_b;
    int r, g, b;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int sumX_r = 0, sumX_g = 0, sumX_b = 0;
            int sumY_r = 0, sumY_g = 0, sumY_b = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    sumX_r += old_r * prewitt_X[radius + i][radius + j];
                    sumX_g += old_g * prewitt_X[radius + i][radius + j];
                    sumX_b += old_b * prewitt_X[radius + i][radius + j];
                    sumY_r += old_r * prewitt_Y[radius + i][radius + j];
                    sumY_g += old_g * prewitt_Y[radius + i][radius + j];
                    sumY_b += old_b * prewitt_Y[radius + i][radius + j];
                }
            }
            int magnitude_r = qBound(0, static_cast<int>(qSqrt(sumX_r * sumX_r + sumY_r * sumY_r)), 255);
            int magnitude_g = qBound(0, static_cast<int>(qSqrt(sumX_g * sumX_g + sumY_g * sumY_g)), 255);
            int magnitude_b = qBound(0, static_cast<int>(qSqrt(sumX_b * sumX_b + sumY_b * sumY_b)), 255);
            img->setPixelColor(x, y, QColor(magnitude_r, magnitude_g, magnitude_b));
        }
    }
}

void imgProcess::Sharpen(QImage *img)
{
    if (!img)
        return;

    int radius = _maskSize/ 2;
    QImage oldImg = img->copy();
    int w = img->width();
    int h = img->height();
    int r, g, b;
    QColor oldColor;
    int old_r, old_g, old_b;
    int sum_r, sum_g, sum_b;
    for (int y = 0; y < img->height(); ++y)
    {
        for (int x = 0; x < img->width(); ++x)
        {
            sum_r = sum_g = sum_b = 0;
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    if (x + j < 0 || x + j >= w || y + i < 0 || y + i >= h)
                        continue;
                    oldColor = oldImg.pixelColor(x + j, y + i);
                    old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
                    sum_r += old_r;
                    sum_g += old_g;
                    sum_b += old_b;
                }
            }
            oldColor = oldImg.pixelColor(x, y);
            old_r = oldColor.red(), old_g = oldColor.green(), old_b = oldColor.blue();
            int avg = _maskSize * _maskSize;
            r = sum_r / static_cast<double>(avg) + 0.5;
            g = sum_g / static_cast<double>(avg) + 0.5;
            b = sum_b / static_cast<double>(avg) + 0.5;
            r = qBound(0, 2 * old_r - r, 255);
            g = qBound(0, 2 * old_g - g, 255);
            b = qBound(0, 2 * old_b - b, 255);
            img->setPixelColor(x, y, QColor(r, g, b));
        }
    }
}



void imgProcess::LeftFlip(QImage* img)
{
    if (!img)
        return;

    QImage oldImg = img->copy();
    QColor color;
    int w = img->width();
    int h = img->height();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = oldImg.pixelColor(x, y);
            img->setPixelColor(y, w - x - 1, color);
        }
    }
}
void imgProcess::RightFlip(QImage* img)
{
    if (!img)
        return;

    QImage oldImg = img->copy();
    QColor color;
    int w = img->width();
    int h = img->height();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = oldImg.pixelColor(x, y);
            img->setPixelColor(h - y - 1, x, color);
        }
    }
}
void imgProcess::VerticalFlip(QImage* img)
{
    if (!img)
        return;

    QImage oldImg = img->copy();
    QColor color;
    int w = img->width();
    int h = img->height();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = oldImg.pixelColor(x, y);
            img->setPixelColor(x, h - y - 1, color);
        }
    }
}
void imgProcess::HorizontalFlip(QImage* img)
{
    if (!img)
        return;

    QImage oldImg = img->copy();
    QColor color;
    int w = img->width();
    int h = img->height();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = oldImg.pixelColor(x, y);
            img->setPixelColor(w - x - 1, y, color);
        }
    }
}

QImage imgProcess::ZoomInterPolation(QImage img)
{
    int w = img.width();
    int h = img.height();
    double ry, rx, cx, cy;

    double scale = _zoomValue;

    int newW = w * scale;
    int newH = h * scale;
    if (newW == w && newH == h)
        return img;

    QImage newImg(newW, newH, QImage::Format_RGB32);

    for (int y = 0; y < newH; ++y)
    {
        for (int x = 0; x < newW; ++x)
        {
            rx = (w - 1.0) / double(newW - 1) * x;
            ry = (h - 1.0) / double(newH - 1) * y;
            int x1 = int(rx);
            int y1 = int(ry);
            int x2 = qMin(x1 + 1, w - 1);
            int y2 = qMin(y1 + 1, h - 1);

            double alpha = rx - x1;
            double beta = ry - y1;

            QColor color1 = img.pixelColor(x1, y1);
            QColor color2 = img.pixelColor(x2, y1);
            QColor color3 = img.pixelColor(x1, y2);
            QColor color4 = img.pixelColor(x2, y2);

            int r = (1 - alpha) * (1 - beta) * color1.red() + alpha * (1 - beta) * color2.red() +
                      (1 - alpha) * beta * color3.red() + alpha * beta * color4.red();

            int g = (1 - alpha) * (1 - beta) * color1.green() + alpha * (1 - beta) * color2.green() +
                        (1 - alpha) * beta * color3.green() + alpha * beta * color4.green();

            int b = (1 - alpha) * (1 - beta) * color1.blue() + alpha * (1 - beta) * color2.blue() +
                       (1 - alpha) * beta * color3.blue() + alpha * beta * color4.blue();

            newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImg;
}

QImage imgProcess::ZoomNeighbouring(QImage img)
{
    int w = img.width();
    int h = img.height();
    double ry, rx, cx, cy;

    double scale = _zoomValue;

    int newW = w * scale;
    int newH = h * scale;
    if (newW == w && newH == h)
        return img;

    QImage newImg(newW, newH, QImage::Format_RGB32);
    QColor color;
    for (int y = 0; y < newH; ++y)
    {
        for (int x = 0; x < newW; ++x)
        {
            rx = qRound((w - 1.0) / double(newW - 1) * x);
            ry = qRound((h - 1.0) / double(newH - 1) * y);
            color = img.pixelColor(rx, ry);
            newImg.setPixelColor(x, y, color);
        }
    }
    return newImg;
}

QImage imgProcess::Rotate(QImage img)
{
    int rotateAngle = _rotateValue;

    double radians = rotateAngle * M_PI / 180.0;
    int w = img.width();
    int h = img.height();

    int newW = qAbs(w * qCos(radians)) + qAbs(h * qSin(radians));
    int newH = qAbs(w * qSin(radians)) + qAbs(h * qCos(radians));

    QImage newImg(newW, newH, QImage::Format_RGB32);
    newImg.fill(QColor(64, 66, 68));

    double centerX = w / 2.0;
    double centerY = h / 2.0;
    double newCenterX = newW / 2.0;
    double newCenterY = newH / 2.0;

    for (int y = 0; y < newH; ++y)
    {
        for (int x = 0; x < newW; ++x)
        {
            double newX = x - newCenterX;
            double newY = y - newCenterY;

            int oriX = newX * qCos(-radians) - newY * qSin(-radians) + centerX + 0.5;
            int oriY= newX * qSin(-radians) + newY * qCos(-radians) + centerY + 0.5;
            if (oriX >= 0 && oriX < w && oriY >= 0 && oriY < h)
            {
                QColor color = img.pixelColor(oriX, oriY);
                newImg.setPixelColor(x, y, color);
            }
        }
    }

    return newImg;
}

QImage imgProcess::BasicOperation(QImage *img)
{
    if (!img)
        return _newImg;
    _newImg = *img;
    _oldImg = *img;
    _h = img->height();
    _w = img->width();
    int r, g, b;
    QColor color;
    for (int y = 0; y < _h; ++y)
    {
        for (int x = 0; x < _w; ++x)
        {
            if (!(_brightnessValue || _contrastValue != 100 || _saturationValue || _isNegativeActivated))
                return *img;
            color = _newImg.pixelColor(x, y);
            r = color.red(), g = color.green(), b = color.blue();
            if (_brightnessValue)
                Brightness(r, g, b);
            if (_contrastValue != 100)
                Contrast(r, g, b);
            if (_isNegativeActivated)
                Negative(r, g, b);
            _newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return _newImg;
}

QImage imgProcess::ConvertOperation(QImage *img)
{
    if (!img)
        return _newImg;
    _newImg = *img;
    _oldImg = *img;
    _h = img->height();
    _w = img->width();
    int r, g, b;
    QColor color;
    for (int y = 0; y < _h; ++y)
    {
        for (int x = 0; x < _w; ++x)
        {
            if (!(_isRGBtoGrayActivated || _isThresholdActivated))
                goto goOut;
            color = _newImg.pixelColor(x, y);
            r = color.red(), g = color.green(), b = color.blue();
            if (_isRGBtoGrayActivated)
                RGBtoGray(r, g, b);
            if (_isThresholdActivated)
                Threshold(r, g, b);
            _newImg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    goOut:
    if (_zoomValue != 1.0)
    {
        if (_isInterPolation)
            _newImg = ZoomInterPolation(_newImg);
        else
            _newImg = ZoomNeighbouring(_newImg);
    }
    if (_rotateValue != 0)
        _newImg = Rotate(_newImg);
    return _newImg;
}



