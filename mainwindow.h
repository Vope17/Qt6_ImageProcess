#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *_fileTree;
    QWidget *_imgShow;
    QWidget *_toolWidget;

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void SlotCreateFile(bool);
    void SlotOpenFile(bool);
    void SlotSaveAsNewFile(bool);
    void SlotSplitWindow();
    void SlotShowQuantization();

signals:
    void SigOpenFile(const QString &path);
};
#endif // MAINWINDOW_H
