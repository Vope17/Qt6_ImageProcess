#include "filetree.h"
#include "ui_filetree.h"

fileTree::fileTree(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::fileTree)
{
    ui->setupUi(this);
    this->setMinimumWidth(378);
    this->setMaximumWidth(378);
}

fileTree::~fileTree()
{
    delete ui;
}

QTreeWidget *fileTree::GetTreeWidget()
{
    return ui->treeWidget;
}

void fileTree::AddFileToTree(const QString name, const QString path)
{
    ui->treeWidget->AddFileToTree(name, path);
}
