#include "removefiledialog.h"
#include "ui_removefiledialog.h"

RemoveFileDialog::RemoveFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemoveFileDialog)
{
    ui->setupUi(this);
}

RemoveFileDialog::~RemoveFileDialog()
{
    delete ui;
}

bool RemoveFileDialog::IsRemoved()
{
    bool bChecked = ui->checkBox->isChecked();
    return bChecked;
}
