#ifndef QUANTIZATION_H
#define QUANTIZATION_H

#include <QMainWindow>

namespace Ui {
class quantization;
}

class quantization : public QMainWindow
{
    Q_OBJECT

public:
    explicit quantization(QWidget *parent = nullptr);
    quantization(QImage*, QWidget *parent = nullptr);
    ~quantization();

private:
    Ui::quantization *ui;
};

#endif // QUANTIZATION_H
