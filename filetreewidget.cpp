#include "filetreewidget.h"
#include <QDir>
#include <filetreeitem.h>
#include "const.h"
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QFileDialog>
#include "removefiledialog.h"
fileTreeWidget::fileTreeWidget(QWidget *parent):QTreeWidget(parent)
    , _tmpItem(nullptr), _dialogProgress(nullptr), _selectedItem(nullptr), _threadCreateFile(nullptr), _threadOpenFile(nullptr)
{
    this->header()->hide();
    connect(this, &fileTreeWidget::itemPressed, this, &fileTreeWidget::SlotItemPressed);
    _actionImport = new QAction(QIcon(":/icon/import.png"), tr("Import Files"), this);
    _actionSetStart = new QAction(QIcon(":/icon/core.png"), tr("Setting File"), this);
    _actionCloseFile= new QAction(QIcon(":/icon/close.png"), tr("Close File"), this);
    _actionSliderShow = new QAction(QIcon(":/icon/slidershow.png"), tr("Play"), this);

    connect(_actionImport, &QAction::triggered, this, &fileTreeWidget::SlotImport);

    connect(_actionCloseFile, &QAction::triggered, this, &fileTreeWidget::SlotCloseFile);
}

void fileTreeWidget::AddFileToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString filePath = dir.absoluteFilePath(name);
    if (_setPath.find(filePath) != _setPath.end())
        return;

    QDir fileDir(filePath);
    if (!fileDir.exists())
    {
        bool enable = fileDir.mkpath(filePath);
        if (!enable)
            return;
    }
    _setPath.insert(filePath);
    auto *item = new fileTreeItem(this, name, filePath, TreeItemFile);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, filePath);
    this->addTopLevelItem(item);
}

void fileTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if (QGuiApplication::mouseButtons() == Qt::LeftButton)
    {
        auto *treePressedItem = dynamic_cast<fileTreeItem*>(pressedItem);
        int itemType = treePressedItem->type();
        if (itemType == TreeItemImg)
        {
            emit SigShowSelectedImg(treePressedItem->GetPath());
            _selectedItem = pressedItem;
        }
    }
    if (QGuiApplication::mouseButtons() == Qt::RightButton)
    {
        QMenu menu(this);
        int itemType = pressedItem->type();
        if (itemType == TreeItemFile)
        {
            _tmpItem = pressedItem;
            menu.addAction(_actionImport);
            menu.addAction(_actionSetStart);
            menu.addAction(_actionCloseFile);
            menu.addAction(_actionSliderShow);
            menu.exec(QCursor::pos());
        }
    }
}

void fileTreeWidget::SlotImport()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("Choose Import Directory"));
    QString path = "";
    if (!_tmpItem)
    {
        qDebug() << "tmpItem is empty\n";
        return;
    }
    path = dynamic_cast<fileTreeItem*>(_tmpItem)->GetPath();
    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (fileDialog.exec())
    {
        fileNames = fileDialog.selectedFiles();
    }
    if (fileNames.isEmpty())
        return;

    QString importPath = fileNames.at(0);
    int fileCount =	0;
    _dialogProgress = new QProgressDialog(this);

    _threadCreateFile = std::make_shared<fileTreeThread>(std::ref(importPath), std::ref(path), _tmpItem, _tmpItem, fileCount, this, nullptr);

    connect(_threadCreateFile.get(), &fileTreeThread::SigUpdateProgress, this, &fileTreeWidget::SlotUpdateProgress);
    connect(_threadCreateFile.get(), &fileTreeThread::SigFinishProgress, this, &fileTreeWidget::SlotFinishProgress);
    connect(_dialogProgress, &QProgressDialog::canceled, this, &fileTreeWidget::SlotCancelProgress);
    connect(this, &fileTreeWidget::SigCancelProgress, _threadCreateFile.get(), &fileTreeThread::SlotCancelProgress);
    _threadCreateFile->start();

    _dialogProgress->setWindowTitle("Loading...");
    _dialogProgress->setFixedWidth(PROGRESS_WIDTH);
    _dialogProgress->setRange(0, PROGRESS_WIDTH);
    _dialogProgress->exec();
}

void fileTreeWidget::SlotCloseFile()
{
    RemoveFileDialog removeFileDialog;
    auto res = removeFileDialog.exec();
    if (res != QDialog::Accepted)
        return;

    bool bRemove = removeFileDialog.IsRemoved();
    auto indexTmpItem = this->indexOfTopLevelItem(_tmpItem);
    auto *c_fileTreeItem = dynamic_cast<fileTreeItem*>(_tmpItem);
    //auto *c_selectedItem = dynamic_cast<fileTreeItem*>(_tmpItem);
    auto deletePath = c_fileTreeItem->GetPath();
    _setPath.remove(deletePath);
    if (bRemove)
    {
        QDir deleteDir(deletePath);
        deleteDir.removeRecursively();
    }

    delete this->takeTopLevelItem(indexTmpItem);
    _tmpItem = nullptr;
}

void fileTreeWidget::SlotUpdateProgress(int count)
{
    if (!_dialogProgress)
        return;

    if (count >= PROGRESS_MAX)
    {
        _dialogProgress->setValue(count % PROGRESS_MAX);
    }
    else
    {
        _dialogProgress->setValue(count);
    }
}

void fileTreeWidget::SlotFinishProgress()
{
    qDebug() << "aa";
    if (!_dialogProgress)
        return;
    _dialogProgress->setValue(PROGRESS_MAX);
    _dialogProgress->deleteLater();
}

void fileTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete _dialogProgress;
    _dialogProgress = nullptr;
}

void fileTreeWidget::SlotOpenFile(const QString &path)
{
    if (_setPath.find(path) != _setPath.end())
        return;

    _setPath.insert(path);
    int fileCount = 0;
    QDir fileDir(path);
    QString fileName = fileDir.dirName();

    _threadOpenFile = std::make_shared<openFileThread>(path, fileCount, this, nullptr);

    _dialogProgress = new QProgressDialog(this);
    connect(_threadOpenFile.get(), &openFileThread::SigUpdateProgress, this, &fileTreeWidget::SlotUpdateProgress);
    connect(_threadOpenFile.get(), &openFileThread::SigFinishProgress, this, &fileTreeWidget::SlotFinishProgress);
    connect(_dialogProgress, &QProgressDialog::canceled, this, &fileTreeWidget::SlotCancelProgress);
    connect(this, &fileTreeWidget::SigCancelProgress, _threadOpenFile.get(), &openFileThread::SlotCancelProgress);

    _threadOpenFile->start();

    _dialogProgress->setWindowTitle("Loading...");
    _dialogProgress->setFixedWidth(PROGRESS_WIDTH);
    _dialogProgress->setRange(0, PROGRESS_WIDTH);
    _dialogProgress->exec();
}
