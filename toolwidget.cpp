#include "toolwidget.h"
#include "ui_toolwidget.h"

toolWidget::toolWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::toolWidget)
{
    ui->setupUi(this);
    initIcons();
    _img = new QImage();
    connect(ui->basicBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetI);
    connect(ui->filterBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetII);
    connect(ui->convertBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetIII);

    connect(ui->brightSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeBrightnessLabelValue);
    connect(ui->brightSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeBrightness);

    connect(ui->contrastSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeContrastLabelValue);
    connect(ui->contrastSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeContrast);

    connect(ui->RGBtoGray, &QPushButton::clicked, this, &toolWidget::SlotConvertRGBtoGray);
    connect(ui->GraytoRGB, &QPushButton::clicked, this, &toolWidget::SlotConvertGraytoRGB);
    connect(ui->GraytoBWSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeGraytoBWLabelValue);
    connect(ui->GraytoBWSlider, &QSlider::valueChanged, this, &toolWidget::SlotConvertGraytoBW);
}

toolWidget::~toolWidget()
{
    delete ui;
}

void toolWidget::initIcons()
{
    QPixmap basicPixmap;
    basicPixmap.load(":/icon/basic2.png");
    QPixmap scaledPixmap = basicPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->basicBtn->setIcon(scaledPixmap);
    ui->basicBtn->setIconSize(scaledPixmap.size());

    QPixmap filterPixmap;
    filterPixmap.load(":/icon/filter.png");
    scaledPixmap = filterPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->filterBtn->setIcon(scaledPixmap);
    ui->filterBtn->setIconSize(scaledPixmap.size());

    QPixmap convertPixmap;
    convertPixmap.load(":/icon/convert.png");
    scaledPixmap = convertPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->convertBtn->setIcon(scaledPixmap);
    ui->convertBtn->setIconSize(scaledPixmap.size());

}

void toolWidget::SlotGetImage(QImage *img)
{
    *_img = *img;
    ui->brightSlider->setValue(0);
    ui->brightnessValue->setText("0");
    ui->contrastSlider->setValue(50);
    ui->contrastValue->setText("50");
}

void toolWidget::SlotChangeWidgetI()
{
    ui->stackedWidget->setCurrentWidget(ui->basicOperation);
}

void toolWidget::SlotChangeWidgetII()
{
    ui->stackedWidget->setCurrentWidget(ui->filters);
}

void toolWidget::SlotChangeWidgetIII()
{
    ui->stackedWidget->setCurrentWidget(ui->convert);
}


void toolWidget::SlotChangeBrightnessLabelValue(int value)
{
    ui->brightnessValue->setText(QString::number(value));
}

void toolWidget::SlotChangeContrastLabelValue(int value)
{
    ui->contrastValue->setText(QString::number(value));
}

void toolWidget::SlotChangeBrightness(int value)
{
    _newImg = _ip->Brightness(_img, value);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotChangeContrast(int value)
{
    _newImg = _ip->Contrast(_img, value);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotConvertRGBtoGray()
{
    _newImg = _ip->RGBtoGray(_img);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotConvertGraytoRGB()
{
    _newImg = _ip->GraytoRGB(_img);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotChangeGraytoBWLabelValue(int value)
{
    ui->GraytoBWValue->setText(QString::number(value));
}

void toolWidget::SlotConvertGraytoBW(int value)
{
    _newImg = _ip->GraytoBW(_img, value);
    emit SigUpdatePixmap(_newImg);
}
