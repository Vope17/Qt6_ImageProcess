#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include "wizard.h"
#include "filetree.h"
#include "filetreewidget.h"
#include "imgshow.h"
#include "toolwidget.h"
#include "quantization.h"
#include "showsplitimg.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629, 869);
    ui->setupUi(this);
    // Menu
    QMenu *fileMenu = menuBar()->addMenu(tr("檔案(&F)"));
    QAction *createAction = new QAction(QIcon(":/icon/createpro.png"),tr("新專案"), this);
    createAction->setShortcut(tr("Ctrl+n"));
    QAction *openAction = new QAction(QIcon(":/icon/openpro.png"), tr("開啟專案"), this);
    openAction->setShortcut(tr("Ctrl+o"));
    QAction *saveAsNewFileAction = new QAction(tr("另存新檔"), this);
    saveAsNewFileAction->setShortcut(tr("Ctrl+s"));
    QAction *exitAction = new QAction("離開程式", this);
    fileMenu->addAction(createAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsNewFileAction);
    fileMenu->addAction(exitAction);

    QMenu *operateActionMenu = menuBar()->addMenu(tr("操作(&O)"));
    QAction *redoAction = new QAction(tr("復原"));
    redoAction->setShortcut(tr("Ctrl+r"));
    QAction *undoAction = new QAction(tr("重做"));
    undoAction->setShortcut(tr("Ctrl+z"));
    QAction *splitWindowAction = new QAction(tr("分割視窗"));
    operateActionMenu->addAction(undoAction);
    operateActionMenu->addAction(redoAction);
    operateActionMenu->addAction(splitWindowAction);

    QMenu *statisticMenu = menuBar()->addMenu(tr("統計(&S)"));
    QAction *quantizationAction = new QAction(tr("量化"));
    statisticMenu->addAction(quantizationAction);

    // FileMenu connects
    connect(createAction, &QAction::triggered, this, &MainWindow::SlotCreateFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::SlotOpenFile);
    connect(saveAsNewFileAction, &QAction::triggered, this, &MainWindow::SlotSaveAsNewFile);
    _fileTree = new fileTree();
    ui->fileTreeLayout->addWidget(_fileTree);
    QTreeWidget *treeWidget = dynamic_cast<fileTree*>(_fileTree)->GetTreeWidget();
    auto *_fileTreeWidget = dynamic_cast<fileTreeWidget*>(treeWidget);
    connect(this, &MainWindow::SigOpenFile, _fileTreeWidget, &fileTreeWidget::SlotOpenFile);

    // imgArea
    _imgShow = new imgShow();
    ui->imgLayout->addWidget(_imgShow);
    auto *fileImgShow = dynamic_cast<imgShow*>(_imgShow);
    connect(_fileTreeWidget, &fileTreeWidget::SigShowSelectedImg, fileImgShow, &imgShow::SlotShowSelectedImg);

    // toolArea
    _toolWidget = new toolWidget();
    ui->toolLayout->addWidget(_toolWidget);
    toolWidget *_lowToolWidget = dynamic_cast<toolWidget*>(_toolWidget);
    connect(fileImgShow, &imgShow::sendImg, _lowToolWidget, &toolWidget::SlotGetImage);
    connect(_lowToolWidget, &toolWidget::SigUpdatePreviewImg, fileImgShow, &imgShow::SlotUpdatePreview);
    connect(_lowToolWidget, &toolWidget::SigUpdatePixmap, fileImgShow, &imgShow::SlotUpdatePixmap);

    // OperateMenu connects
    connect(undoAction, &QAction::triggered, fileImgShow, &imgShow::SlotUndo);
    connect(redoAction, &QAction::triggered, fileImgShow, &imgShow::SlotRedo);
    connect(fileImgShow, &imgShow::SigUnRedo, _lowToolWidget, &toolWidget::SlotUnRedo);
    connect(splitWindowAction, &QAction::triggered, this, &MainWindow::SlotSplitWindow);

    // StatisticMenu connects
    connect(quantizationAction, &QAction::triggered, this, &MainWindow::SlotShowQuantization);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto *fileImgShow = dynamic_cast<imgShow*>(_imgShow);
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreateFile(bool)
{
    qDebug() << "slot open file is triggered";
    Wizard wizard(this);
    wizard.setWindowTitle("Create Project");
    auto *page = wizard.page(0);
    page->setTitle(tr("Project Setting"));

    // return configuration
    connect(&wizard, &Wizard::SigFileSettings, dynamic_cast<fileTree*>(_fileTree), &fileTree::AddFileToTree);
    wizard.show();
    wizard.exec();

    disconnect(&wizard);
}

void MainWindow::SlotOpenFile(bool)
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("Choose the import Directory"));
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    QString originPath = QDir::currentPath();
    QStringList fileNames;
    if (fileDialog.exec())
        fileNames = fileDialog.selectedFiles();
    if (fileNames.length() <= 0 || fileNames.at(0) == originPath)
        return;
    qDebug() << fileNames;
    QString importPath = fileNames.at(0);
    qDebug() << importPath;
    emit SigOpenFile(importPath);
}

void MainWindow::SlotSaveAsNewFile(bool)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as new file"), QDir::homePath(), tr("Images (*.png *.jpg *.bmp);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    auto *fileImgShow = dynamic_cast<imgShow*>(_imgShow);
    QPixmap pm = fileImgShow->GetPixmap();
    QImage img = pm.toImage();
    if (!img.save(fileName))
        return;
}

void MainWindow::SlotSplitWindow()
{
    auto *fileImgShow = dynamic_cast<imgShow*>(_imgShow);
    QPixmap pm = fileImgShow->GetPixmap();
    QImage img = pm.toImage();
    ShowSplitImg *q = new ShowSplitImg(&img);
    q->show();
}

void MainWindow::SlotShowQuantization()
{
    auto *fileImgShow = dynamic_cast<imgShow*>(_imgShow);
    QPixmap pm = fileImgShow->GetPixmap();
    QImage img = pm.toImage();
    quantization *q = new quantization(&img);
    q->showMaximized();
}

