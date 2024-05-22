#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include "filetreethread.h"
#include "openfilethread.h"
class fileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    fileTreeWidget(QWidget *parent = nullptr);
    void AddFileToTree(const QString &name, const QString &path);

private:
    QSet<QString> _setPath;

    // show up the menu after right click item
    QTreeWidgetItem *_tmpItem;
    QTreeWidgetItem *_selectedItem;
    QAction *_actionImport;
    QAction *_actionSetStart;
    QAction *_actionCloseFile;
    QAction *_actionSliderShow;
    QProgressDialog *_dialogProgress;
    std::shared_ptr<fileTreeThread> _threadCreateFile;
    std::shared_ptr<openFileThread> _threadOpenFile;

private slots:
    void SlotItemPressed(QTreeWidgetItem*, int);
    void SlotImport();
    void SlotCloseFile();

    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();

public slots:
    void SlotOpenFile(const QString &path);

signals:
    void SigCancelProgress();
    void SigShowSelectedImg(const QString&);
};

#endif // FILETREEWIDGET_H
