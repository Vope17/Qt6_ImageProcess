#include "showsplitimg.h"

ShowSplitImg::ShowSplitImg(QWidget *parent)
    : QMainWindow{parent}
{
}

ShowSplitImg::ShowSplitImg(QImage *img, QWidget *parent)
{
    showImg = new QLabel(this);
    this->setCentralWidget(showImg);
    showImg->setPixmap(QPixmap::fromImage(*img));
    showImg->setScaledContents(true);
}

void ShowSplitImg::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        showImg->size();
        QSize oldSize = showImg->size(); //->pixmap().size();
        if (numDegrees.y() > 0)
        {
            showImg->resize(oldSize * 1.1);
        }
        else if (numDegrees.y() < 0)
        {
            showImg->resize(oldSize * 0.95);
        }
    }
    event->accept();
}
