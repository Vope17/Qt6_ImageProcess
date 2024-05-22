#ifndef FILETREETHREAD_H
#define FILETREETHREAD_H

#include <QThread>
#include <QTreeWidgetItem>
class fileTreeThread : public QThread
{
    Q_OBJECT
public:
    fileTreeThread(const QString &srcPath, const QString &destPath,
                   QTreeWidgetItem *root, QTreeWidgetItem *parentItem, int fileCount,
                   QTreeWidget *self, QObject *parent = nullptr);
    ~fileTreeThread();

protected:
    virtual void run();

private:
    void CreateFileTree(const QString &srcPath, const QString &destPath, QTreeWidgetItem *root, QTreeWidgetItem *parentItem,
                        int &fileCount, QTreeWidget *self, QTreeWidgetItem *preItem = nullptr);

    QString _srcPath;
    QString _destPath;
    int _fileCount;
    QTreeWidgetItem *_parentItem;
    QTreeWidget *_self;
    QTreeWidgetItem *_root;
    bool _threadStop;

public slots:
    void SlotCancelProgress();

signals:
    void SigUpdateProgress(int);
    void SigFinishProgress(int);
};

#endif // FILETREETHREAD_H
