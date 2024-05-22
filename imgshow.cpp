#include "imgshow.h"
#include "ui_imgshow.h"

imgShow::imgShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::imgShow)
{
    ui->setupUi(this);
}

imgShow::~imgShow()
{
    delete ui;
}

void imgShow::SlotShowSelectedImg(const QString &path)
{
    _selectedPath = path;
    _pixmap.load(_selectedPath);
    //auto width = this->width() - 20;
    //auto height = this->height() - 20;
    //_pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
    ui->label->setPixmap(_pixmap);
}
