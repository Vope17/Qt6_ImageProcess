#ifndef OPENFILEPAGE_H
#define OPENFILEPAGE_H

#include <QWizardPage>

namespace Ui {
class openFilePage;
}

class openFilePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit openFilePage(QWidget *parent = nullptr);
    ~openFilePage();
    void GetProSettings(QString &name, QString &path);

protected:
    bool isComplete() const override;
private slots:
    void on_pushButton_clicked();

private:
    Ui::openFilePage *ui;
};

#endif // OPENFILEPAGE_H
