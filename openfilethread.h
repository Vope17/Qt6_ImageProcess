#ifndef OPENFILETHREAD_H
#define OPENFILETHREAD_H

#include <QDir>
#include <QThread>
#include <QTreeWidget>
class openFileThread : public QThread
{
    Q_OBJECT
public:
    explicit openFileThread(const QString &srcPath, int fileCount, QTreeWidget *self, QObject *parent = nullptr);

    void OpenFileTree(const QString &srcPath, int &fileCount, QTreeWidget *self);

protected:
    virtual void run();

private:
    void RecursiveFileTree(const QString &srcPath, int &fileCount, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preItem);
    QString _srcPath;
    int _fileCount;
    QTreeWidget *_self;
    bool _threadStop;
    QTreeWidgetItem *_root;
signals:
    void SigUpdateProgress(int);
    void SigFinishProgress(int);
public slots:
    void SlotCancelProgress();
};

#endif // OPENFILETHREAD_H
