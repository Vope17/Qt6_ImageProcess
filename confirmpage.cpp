#include "confirmpage.h"
#include "ui_confirmpage.h"

confirmPage::confirmPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::confirmPage)
{
    ui->setupUi(this);
}

confirmPage::~confirmPage()
{
    delete ui;
}
