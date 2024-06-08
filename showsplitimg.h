#ifndef SHOWSPLITIMG_H
#define SHOWSPLITIMG_H

#include <QMainWindow>
#include <QLabel>
#include <QWheelEvent>
class ShowSplitImg : public QMainWindow
{
    Q_OBJECT
public:
    explicit ShowSplitImg(QWidget *parent = nullptr);
    ShowSplitImg(QImage *img, QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    QLabel *showImg;
};

#endif // SHOWSPLITIMG_H
