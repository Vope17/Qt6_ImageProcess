#include "openfilepage.h"
#include "ui_openfilepage.h"
#include <QDir>
#include <QFileDialog>
openFilePage::openFilePage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::openFilePage)
{
    ui->setupUi(this);
    registerField("proName*", ui->lineEdit);
    registerField("proPath*", ui->lineEdit_2);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &openFilePage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &openFilePage::completeChanged);
    QString curPath = QDir::currentPath();
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());

}

openFilePage::~openFilePage()
{
    delete ui;
}

void openFilePage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

bool openFilePage::isComplete() const
{
    if (ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "")
        return false;

    QDir dir(ui->lineEdit_2->text());
    if (!dir.exists())
    {
        ui->tips->setText("project path is not exists");
        return false;
    }

    //check path
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir destDir(absFilePath);
    if (destDir.exists())
    {
        ui->tips->setText("project has exists. Please change path or name");
        return false;
    }

    ui->tips->setText("");
    return QWizardPage::isComplete();
}

void openFilePage::on_pushButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("Choose Directory");
    auto path = QDir::currentPath();
    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (fileDialog.exec())
        fileNames = fileDialog.selectedFiles();

    if (fileNames.length() <= 0)
        return;

    QString importPath = fileNames.at(0);
    ui->lineEdit_2->setText(importPath);
}

