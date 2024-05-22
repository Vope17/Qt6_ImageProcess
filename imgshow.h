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

public slots:
    void SlotShowSelectedImg(const QString &path);
};

#endif // IMGSHOW_H
