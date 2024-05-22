#include "filetreethread.h"
#include <QDir>
#include "filetreeitem.h"
#include "const.h"
fileTreeThread::fileTreeThread(const QString &srcPath, const QString &destPath,
                               QTreeWidgetItem *root, QTreeWidgetItem *parentItem,
                               int fileCount, QTreeWidget *self, QObject *parent)
    :QThread(parent), _srcPath(srcPath), _destPath(destPath), _fileCount(fileCount), _parentItem(parentItem), _self(self), _root(root), _threadStop(false)
{

}

fileTreeThread::~fileTreeThread()
{

}

void fileTreeThread::run()
{
    CreateFileTree(_srcPath, _destPath, _root, _parentItem, _fileCount, _self);
    if (_threadStop)
    {
        auto path = dynamic_cast<fileTreeItem*>(_root)->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        return;
    }

    emit SigFinishProgress(_fileCount);
}

void fileTreeThread::CreateFileTree(const QString &srcPath, const QString &destPath, QTreeWidgetItem *root, QTreeWidgetItem *parentItem, int &fileCount, QTreeWidget *self, QTreeWidgetItem *preItem)
{
    if (_threadStop)
        return;

    bool needCopy = true;
    if (srcPath == destPath)
        needCopy = false;

    QDir importPath(srcPath);
    QStringList nameFilters;
    importPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    importPath.setSorting(QDir::Name);
    QFileInfoList list = importPath.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        if (_threadStop)
            return;
        QFileInfo fileInfo = list.at(i);
        bool isDir = fileInfo.isDir();
        if (isDir)
        {
            if (_threadStop)
                return;

            fileCount++;
            emit SigUpdateProgress(fileCount);
            QDir destDir(destPath);
            QString subDestPath = destDir.absoluteFilePath(fileInfo.fileName());
            QDir subDestDir(subDestPath);
            if (!subDestDir.exists())
            {
                bool ok = subDestDir.mkpath(subDestPath);
                if (!ok)
                    continue;
            }

            auto *item = new fileTreeItem(parentItem, fileInfo.fileName(), subDestPath, root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, subDestPath);
            CreateFileTree(fileInfo.absoluteFilePath(), subDestPath, root, item, fileCount, self, preItem);
        }
        else
        {
            if (_threadStop)
                return;
            const QString &suffix = fileInfo.completeSuffix();
            if (!(suffix == "png" || suffix == "jpeg" || suffix == "jpg" || suffix == "bmp"))
                continue;
            fileCount++;
            emit SigUpdateProgress(fileCount);
            if (!needCopy)
                continue;

            QDir destDir(destPath);
            QString destFilePath = destDir.absoluteFilePath(fileInfo.fileName());
            if (!QFile::copy(fileInfo.absoluteFilePath(), destFilePath))
                continue;

            auto *item = new fileTreeItem(parentItem, fileInfo.fileName(), destFilePath, root, TreeItemImg);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, destFilePath);

            if (preItem)
            {
                auto *preFileItem = dynamic_cast<fileTreeItem*>(preItem);
                preFileItem->SetNextItem(item);
            }
            item->SetPreItem(preItem);
            preItem = item;
        }
    }
}

void fileTreeThread::SlotCancelProgress()
{
    this->_threadStop = true;
}

