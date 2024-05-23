#ifndef IMGSHOW_H
#define IMGSHOW_H

#include <QDialog>

namespace Ui {
class imgShow;
}

class imgShow : public QDialog
{
    Q_OBJECT

public:
    explicit imgShow(QWidget *parent = nullptr);
    ~imgShow();

private:
    Ui::imgShow *ui;
    QString _selectedPath;
    QPixmap _pixmap;
    QImage *_img;

public slots:
    void SlotShowSelectedImg(const QString &path);
    void SlotUpdatePixmap(QImage);
    void SlotUpdateImg(QImage);

signals:
    void sendImg(QImage*);
};

#endif // IMGSHOW_H
