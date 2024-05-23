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

    connect(ui->brightSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeBrightness);

    connect(ui->contrastSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeContrast);

    connect(ui->RGBtoGrayCheckBox, &QCheckBox::checkStateChanged, this, &toolWidget::SlotCheckRGBtoGray);
    connect(ui->thresholdCheckBox, &QCheckBox::checkStateChanged, this, &toolWidget::SlotCheckThreshold);
    connect(ui->thresholdSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeThreshold);
}

toolWidget::~toolWidget()
{
    delete ui;
}

void toolWidget::initIcons()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(":/icon/basic2.png");
    QPixmap scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->basicBtn->setIcon(scaledPixmap);
    ui->basicBtn->setIconSize(scaledPixmap.size());

    tmpPixmap.load(":/icon/filter.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->filterBtn->setIcon(scaledPixmap);
    ui->filterBtn->setIconSize(scaledPixmap.size());

    tmpPixmap.load(":/icon/convert.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->convertBtn->setIcon(scaledPixmap);
    ui->convertBtn->setIconSize(scaledPixmap.size());

    ui->RGBtoGrayCheckBox->setIconSize(scaledPixmap.size());
    ui->thresholdCheckBox->setIconSize(scaledPixmap.size());
    tmpPixmap.load(":/icon/activate_on.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _activate_on = scaledPixmap;

    tmpPixmap.load(":/icon/activate_off.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _activate_off = scaledPixmap;

    ui->RGBtoGrayCheckBox->setIcon(_activate_off);
    ui->thresholdCheckBox->setIcon(_activate_off);
}

void toolWidget::SlotGetImage(QImage *img)
{
    *_img = *img;
    _ip->ResetAllValue();
    _ip->_isGray = img->allGray() ? true : false;
    ui->brightSlider->setValue(0);
    ui->brightnessValue->setText("0");
    ui->contrastSlider->setValue(100);
    ui->contrastValue->setText("1.0");
    ui->RGBtoGrayCheckBox->setCheckState(Qt::Unchecked);
    ui->thresholdCheckBox->setCheckState(Qt::Unchecked);
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

void toolWidget::SlotChangeBrightness(int value)
{
    ui->brightnessValue->setText(QString::number(value));
    _ip->_brightnessValue = value;
    _newImg = _ip->AllOperation(_img);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotChangeContrast(int value)
{
    ui->contrastValue->setText(QString::number(value / 100.0));
    _ip->_contrastValue = value;
    _newImg = _ip->AllOperation(_img);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotConvertRGBtoGray()
{
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotChangeThreshold(int value)
{
    ui->thresholdValue->setText(QString::number(value));
    _ip->_thresholdValue = value;
    _newImg = _ip->AllOperation(_img);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotCheckRGBtoGray(Qt::CheckState state)
{
    if (state == Qt::Unchecked)
    {
        _ip->_isRGBtoGrayActivated = false;
        ui->RGBtoGrayCheckBox->setIcon(_activate_off);
    }
    else
    {
        _ip->_isRGBtoGrayActivated = true;
        ui->RGBtoGrayCheckBox->setIcon(_activate_on);
    }
    _newImg = _ip->AllOperation(_img);
    emit SigUpdatePixmap(_newImg);
}

void toolWidget::SlotCheckThreshold(Qt::CheckState state)
{
    if (state == Qt::Unchecked)
    {
        _ip->_isThresholdActivated = false;
        ui->thresholdCheckBox->setIcon(_activate_off);
    }
    else
    {
        _ip->_isThresholdActivated = true;
        ui->thresholdCheckBox->setIcon(_activate_on);
    }
    _newImg = _ip->AllOperation(_img);
    emit SigUpdatePixmap(_newImg);
}
