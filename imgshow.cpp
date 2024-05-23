#include "imgshow.h"
#include "ui_imgshow.h"

imgShow::imgShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::imgShow)
{
    ui->setupUi(this);
    _img = new QImage;
}

imgShow::~imgShow()
{
    delete ui;
}

QPixmap imgShow::GetPixmap()
{
    return _pixmap;
}

void imgShow::SlotShowSelectedImg(const QString &path)
{
    _selectedPath = path;
    _pixmap.load(_selectedPath);
    *_img = _pixmap.toImage();
    ui->label->setPixmap(_pixmap);
    emit sendImg(_img);
}


void imgShow::SlotUpdatePixmap(QImage img)
{
    _pixmap = QPixmap::fromImage(img);
    ui->label->setPixmap(QPixmap::fromImage(img));
}

