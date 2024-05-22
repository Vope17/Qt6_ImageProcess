#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QWizardPage>

namespace Ui {
class confirmPage;
}

class confirmPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit confirmPage(QWidget *parent = nullptr);
    ~confirmPage();

private:
    Ui::confirmPage *ui;
};

#endif // CONFIRMPAGE_H
