#include <QDir>
#include <filetreeitem.h>
#include "openfilethread.h"
#include "const.h"
openFileThread::openFileThread(const QString &srcPath, int fileCount, QTreeWidget *self, QObject *parent)
    :QThread(parent), _threadStop(false), _srcPath(srcPath), _fileCount(fileCount), _self(self), _root(nullptr)
{

}

void openFileThread::OpenFileTree(const QString &srcPath, int &fileCount, QTreeWidget *self)
{
    qDebug() << "openFileTree thread is triggered";
    QDir srcDir(srcPath);
    auto name = srcDir.dirName();
    auto *item = new fileTreeItem(self, name, srcPath, TreeItemFile);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, srcPath);
    _root = item;
    RecursiveFileTree(srcPath, fileCount, self, _root, item, nullptr);
}

void openFileThread::run()
{
    OpenFileTree(_srcPath, _fileCount, _self);
    if (_threadStop && _root && _self)
    {
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        return;
    }
    qDebug() << "11";
    emit SigFinishProgress(_fileCount);
    qDebug() << "44";
}

void openFileThread::RecursiveFileTree(const QString &srcPath, int &fileCount, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preItem)
{
    QDir srcDir(srcPath);
    srcDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    srcDir.setSorting(QDir::Name);
    QFileInfoList list = srcDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        if (_threadStop)
            return;

        QFileInfo fileInfo = list.at(i);
        bool IsDir = fileInfo.isDir();
        if (IsDir)
        {
            qDebug() << "recursive in dir";
            if (_threadStop)
                return;

            fileCount++;
            emit SigUpdateProgress(fileCount);
            auto *item = new fileTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(), _root, TreeItemFile);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            RecursiveFileTree(fileInfo.absoluteFilePath(), fileCount, self, root, item, preItem);
        }
        else
        {
            if (_threadStop)
                return;

            const QString &suffix = fileInfo.completeSuffix();
            if (!(suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "bmp"))
                continue;

            fileCount++;
            emit SigUpdateProgress(fileCount);
            auto *item = new fileTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(), _root, TreeItemImg);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            if (preItem)
            {
                auto *preFileItem = dynamic_cast<fileTreeItem*>(preItem);
                preFileItem->SetNextItem(item);
            }
            item->SetNextItem(preItem);
            preItem = item;
        }
    }
    emit SigFinishProgress(fileCount);
}

void openFileThread::SlotCancelProgress()
{
    this->_threadStop = true;
}
