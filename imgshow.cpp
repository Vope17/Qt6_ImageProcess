#include "imgshow.h"
#include "ui_imgshow.h"

imgShow::imgShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::imgShow)
{
    ui->setupUi(this);
    _img = new QImage;
    _oldImg = new QImage;
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
    *_oldImg = *_img;
    ui->label->setPixmap(_pixmap);
    _undoHistory.clear();
    _redoHistory.clear();
    _undoHistory.push(*_oldImg);
    emit sendImg(_img);
}

void imgShow::SlotUpdatePreview(QImage img)
{
    _pixmap = QPixmap::fromImage(img);
    ui->label->setPixmap(_pixmap);
}


void imgShow::SlotUpdatePixmap()
{
    _undoHistory.push(*_oldImg);
    _redoHistory.clear();
    *_oldImg = *_img;
    _pixmap = QPixmap::fromImage(*_img);
    ui->label->setPixmap(_pixmap);
    emit sendImg(_img);
}

void imgShow::SlotUndo()
{
    if (_undoHistory.size() <= 1)
        return;
    _redoHistory.push(*_img);
    QImage tmp = _undoHistory.top(); _undoHistory.pop();
    *_img = tmp;
    *_oldImg = *_img;
    _pixmap = QPixmap::fromImage(*_img);
    ui->label->setPixmap(_pixmap);
    emit SigUnRedo(_img);
}

void imgShow::SlotRedo()
{
    if (_redoHistory.empty())
        return;
    QImage tmp = _redoHistory.top(); _redoHistory.pop();
    _undoHistory.push(*_img);
    *_img = tmp;
    *_oldImg = *_img;
    _pixmap = QPixmap::fromImage(*_img);
    ui->label->setPixmap(_pixmap);
    emit SigUnRedo(_img);
}



