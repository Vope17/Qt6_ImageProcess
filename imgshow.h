#ifndef IMGSHOW_H
#define IMGSHOW_H

#include <QDialog>
#include <QStack>
namespace Ui {
class imgShow;
}

class imgShow : public QDialog
{
    Q_OBJECT

public:
    explicit imgShow(QWidget *parent = nullptr);
    ~imgShow();
    QPixmap GetPixmap();
private:
    Ui::imgShow *ui;
    QString _selectedPath;
    QPixmap _pixmap;
    QImage *_img;
    QImage *_oldImg;
    QStack<QImage> _undoHistory;
    QStack<QImage> _redoHistory;

public slots:
    void SlotShowSelectedImg(const QString &path);
    void SlotUpdatePreview(QImage);
    void SlotUpdatePixmap();
    void SlotUndo();
    void SlotRedo();

signals:
    void sendImg(QImage*);
    void SigUnRedo(QImage*);
};

#endif // IMGSHOW_H
