#include "toolwidget.h"
#include "ui_toolwidget.h"

toolWidget::toolWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::toolWidget)
{
    ui->setupUi(this);
    initIcons();
    _previewImg = new QImage();
    connect(ui->basicBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetI);
    connect(ui->filterBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetII);
    connect(ui->convertBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetIII);
    connect(ui->morphologyBtn, &QPushButton::clicked, this, &toolWidget::SlotChangeWidgetIV);

    // Basic area
    connect(ui->brightSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeBrightness);
    connect(ui->contrastSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeContrast);
    //connect(ui->saturationSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeSaturation);
    connect(ui->negativeCheckBox, &QCheckBox::checkStateChanged, this, &toolWidget::SlotCheckNegative);

    // filter area
    connect(ui->maskSizeSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeMaskSize);
    connect(ui->customFilterKernelTextEditor, &QPlainTextEdit::textChanged, this, &toolWidget::SlotUpdateFilterKernel);
    connect(ui->customFilterButton, &QPushButton::clicked, this, &toolWidget::SlotCustomLowPassFilter);
    connect(ui->averageButton, &QPushButton::clicked, this, &toolWidget::SlotAverage);
    connect(ui->gaussianButton, &QPushButton::clicked, this, &toolWidget::SlotGaussian);
    connect(ui->maximumButton, &QPushButton::clicked, this, &toolWidget::SlotMaximum);
    connect(ui->minimumButton, &QPushButton::clicked, this, &toolWidget::SlotMinimum);
    connect(ui->medianButton, &QPushButton::clicked, this, &toolWidget::SlotMedian);

    connect(ui->customHighPassFilterKernelXTextEditor, &QPlainTextEdit::textChanged, this, &toolWidget::SlotUpdateHighPassFilterKernelX);
    connect(ui->customHighPassFilterKernelYTextEditor, &QPlainTextEdit::textChanged, this, &toolWidget::SlotUpdateHighPassFilterKernelY);
    connect(ui->customHighPassFilterButton, &QPushButton::clicked, this, &toolWidget::SlotCustomHighPassFilter);
    connect(ui->sobelButton, &QPushButton::clicked, this, &toolWidget::SlotSobel);
    connect(ui->prewittButton, &QPushButton::clicked, this, &toolWidget::SlotPrewitt);
    connect(ui->sharpenButton, &QPushButton::clicked, this, &toolWidget::SlotSharpen);

    // convert area
    connect(ui->horizontalFlipButton, &QPushButton::clicked, this, &toolWidget::SlotHorizontalFlip);
    ui->horizontalFlipButton->setShortcut(tr("s"));
    connect(ui->verticalFlipButton, &QPushButton::clicked, this, &toolWidget::SlotVerticalFlip);
    ui->verticalFlipButton->setShortcut(tr("w"));
    connect(ui->leftFlipButton, &QPushButton::clicked, this, &toolWidget::SlotLeftFlip);
    ui->leftFlipButton->setShortcut(tr("a"));
    connect(ui->rightFlipButton, &QPushButton::clicked, this, &toolWidget::SlotRightFlip);
    ui->rightFlipButton->setShortcut(tr("d"));
    connect(ui->NeighbouringRadio, &QRadioButton::toggled, this, &toolWidget::SlotChangeZoomImplementation);
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeZoom);
    connect(ui->rotateSlider, &QSlider::valueChanged, this, &toolWidget::SlotRotate);

    connect(ui->RGBtoGrayCheckBox, &QCheckBox::checkStateChanged, this, &toolWidget::SlotCheckRGBtoGray);
    connect(ui->thresholdCheckBox, &QCheckBox::checkStateChanged, this, &toolWidget::SlotCheckThreshold);
    connect(ui->thresholdSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeThreshold);
    connect(ui->otusThresholdButton, &QPushButton::clicked, this, &toolWidget::SlotOtus);
    connect(ui->equalizationButton, &QPushButton::clicked, this, &toolWidget::SlotEqualization);
    connect(ui->GrayToAsciiButton, &QPushButton::clicked, this, &toolWidget::SlotGrayToAscii);


    // morphology area
    connect(ui->kernelSizeSlider, &QSlider::valueChanged, this, &toolWidget::SlotChangeKernelSize);
    connect(ui->kernelSizeTextEditor, &QPlainTextEdit::textChanged, this, &toolWidget::SlotUpdateKernel);
    connect(ui->dilationButton, &QPushButton::clicked, this, &toolWidget::SlotDilation);
    connect(ui->erosionButton, &QPushButton::clicked, this, &toolWidget::SlotErosion);
    connect(ui->openingButton, &QPushButton::clicked, this, &toolWidget::SlotOpening);
    connect(ui->closingButton, &QPushButton::clicked, this, &toolWidget::SlotClosing);
    connect(ui->boundaryDetectionButton, &QPushButton::clicked, this, &toolWidget::SlotBoundaryDetection);
    connect(ui->noiseRemovalButton, &QPushButton::clicked, this, &toolWidget::SlotNoiseRemoval);
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

    tmpPixmap.load(":/icon/convert.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->convertBtn->setIcon(scaledPixmap);

    tmpPixmap.load(":/icon/activate_on.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _activate_on = scaledPixmap;

    tmpPixmap.load(":/icon/activate_off.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _activate_off = scaledPixmap;

    tmpPixmap.load(":/icon/leftFlip.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->leftFlipButton->setIconSize(scaledPixmap.size());
    ui->leftFlipButton->setIcon(tmpPixmap);

    tmpPixmap.load(":/icon/rightFlip.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->rightFlipButton->setIconSize(scaledPixmap.size());
    ui->rightFlipButton->setIcon(tmpPixmap);

    tmpPixmap.load(":/icon/verticalFlip.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->verticalFlipButton->setIconSize(scaledPixmap.size());
    ui->verticalFlipButton->setIcon(tmpPixmap);

    tmpPixmap.load(":/icon/horizontalFlip.png");
    scaledPixmap = tmpPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->horizontalFlipButton->setIconSize(scaledPixmap.size());
    ui->horizontalFlipButton->setIcon(tmpPixmap);

    ui->filterBtn->setIconSize(scaledPixmap.size());
    ui->negativeCheckBox->setIconSize(scaledPixmap.size());
    ui->convertBtn->setIconSize(scaledPixmap.size());
    ui->RGBtoGrayCheckBox->setIconSize(scaledPixmap.size());
    ui->thresholdCheckBox->setIconSize(scaledPixmap.size());

    ui->negativeCheckBox->setIcon(_activate_off);
    ui->RGBtoGrayCheckBox->setIcon(_activate_off);
    ui->thresholdCheckBox->setIcon(_activate_off);
}

void toolWidget::initToolOptions()
{
    _ip->ResetAllValue();
    ui->brightSlider->setValue(0);
    ui->brightnessValue->setText("0");
    ui->contrastSlider->setValue(100);
    ui->contrastValue->setText("1.0");
    //ui->saturationSlider->setValue(0);
    //ui->saturationValue->setText("0");
    ui->negativeCheckBox->setCheckState(Qt::Unchecked);
    ui->maskSizeSlider->setValue(1);
    ui->maskSizeValue->setText("3");

    ui->InterPolationRadio->setChecked(true);
    ui->zoomSlider->setValue(50);
    ui->zoomValue->setText("1");
    ui->rotateSlider->setValue(0);
    ui->rotateValue->setText("0");

    ui->RGBtoGrayCheckBox->setCheckState(Qt::Unchecked);
    ui->thresholdCheckBox->setCheckState(Qt::Unchecked);
    ui->thresholdSlider->setValue(127);
    ui->thresholdValue->setText("127");
}


void toolWidget::SlotGetImage(QImage *img)
{
    if (!img)
        return;

    _img = img;
    *_previewImg = img->copy().scaled(150, 150, Qt::KeepAspectRatio);
    _newImg = *_img;
    _previewPixmap = _previewPixmap.fromImage(*_previewImg);
    _ip->ResetAllValue();
    _ip->_isGray = img->allGray() ? true : false;
    initToolOptions();
}

void toolWidget::SlotUnRedo(QImage *img)
{
    if (!_img)
        return;

    _img = img;
    _newImg = *img;
    *_previewImg = img->copy().scaled(150, 150, Qt::KeepAspectRatio);
    _previewPixmap = _previewPixmap.fromImage(*_previewImg);
    _ip->ResetAllValue();
    _ip->_isGray = img->allGray() ? true : false;
    initToolOptions();
}

void toolWidget::SlotChangeWidgetI()
{
    ui->stackedWidget->setCurrentWidget(ui->basicOperation);

    if (!_img)
        return;

    if (*_img != _newImg)
    {
        *_img = _newImg;
        emit SigUpdatePixmap();
    }
    _ip->ResetAllValue();
    initToolOptions();
}

void toolWidget::SlotChangeWidgetII()
{

    ui->stackedWidget->setCurrentWidget(ui->filters);

    if (!_img)
        return;

    if (*_img != _newImg)
    {
        *_img = _newImg;
        emit SigUpdatePixmap();
    }
    initToolOptions();
}

void toolWidget::SlotChangeWidgetIII()
{
    ui->stackedWidget->setCurrentWidget(ui->convert);

    if (!_img)
        return;

    if (*_img != _newImg)
    {
        *_img = _newImg;
        emit SigUpdatePixmap();
    }
    initToolOptions();
}

void toolWidget::SlotChangeWidgetIV()
{
    ui->stackedWidget->setCurrentWidget(ui->morphology);

    if (!_img)
        return;

    if (*_img != _newImg)
    {
        *_img = _newImg;
        emit SigUpdatePixmap();
    }
    initToolOptions();
}

void toolWidget::SlotChangeBrightness(int value)
{
    if (!_img)
        return;

    ui->brightnessValue->setText(QString::number(value));
    _ip->_brightnessValue = value;
    _newImg = _ip->BasicOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotChangeContrast(int value)
{
    if (!_img)
        return;

    ui->contrastValue->setText(QString::number(value / 100.0));
    _ip->_contrastValue = value;
    _newImg = _ip->BasicOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotChangeSaturation(int value)
{
    if (!_img)
        return;
    double normalizeValue = value / double(100);
    //ui->saturationValue->setText(QString::number(normalizeValue));
    _ip->_saturationValue = normalizeValue;
    _newImg = _ip->BasicOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotCheckNegative(Qt::CheckState state)
{
    if (!_img)
        return;

    if (state == Qt::Unchecked)
    {
        _ip->_isNegativeActivated = false;
        ui->negativeCheckBox->setIcon(_activate_off);
    }
    else
    {
        _ip->_isNegativeActivated = true;
        ui->negativeCheckBox->setIcon(_activate_on);
    }
    _newImg = _ip->BasicOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotChangeThreshold(int value)
{
    if (!_img)
        return;

    ui->thresholdValue->setText(QString::number(value));
    _ip->_thresholdValue = value;
    _newImg = _ip->ConvertOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotCheckRGBtoGray(Qt::CheckState state)
{
    if (!_img)
        return;

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
    _newImg = _ip->ConvertOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotCheckThreshold(Qt::CheckState state)
{
    if (!_img)
        return;

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
    _newImg = _ip->ConvertOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotOtus()
{
    if (!_img)
        return;

    *_img = _ip->ConvertOperation(_img);
    int OtusValue = _ip->OtusThreshold(_img);
    emit SigUpdatePixmap();
    _ip->_thresholdValue = OtusValue;
    ui->thresholdValue->setText(QString::number(OtusValue));
    ui->thresholdSlider->setValue(OtusValue);
}

void toolWidget::SlotEqualization()
{
    if (!_img)
        return;

    *_img = _ip->ConvertOperation(_img);
    _ip->Equalization(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotGrayToAscii()
{
    if (!_img)
        return;

    _newImg = _ip->ConvertOperation(_img);
    _ip->GrayToAscii(_newImg);
    //emit SigUpdatePixmap();
}

void toolWidget::SlotChangeKernelSize(int value)
{
    ui->kernelSizeValue->setText(QString::number(value));
    QVector<QVector<int>> kernel(value, QVector<int>(value, 1));
    _ip->_kernel = kernel;
    QString content;
    for (int i = 0; i < value; ++i)
    {
        for (int j = 0; j < value; ++j)
        {
            content += '1';
            content += (j == value - 1 ? "" : " ");
        }
        content += (i == value - 1 ? "" : "\n");
    }
    ui->kernelSizeTextEditor->setPlainText(content);
}

void toolWidget::SlotUpdateKernel()
{
    QString content = ui->kernelSizeTextEditor->toPlainText();
    QStringList lines = content.split("\n");
    QVector<QVector<int>> kernel;
    for (const QString &line : lines)
    {
        if (line.isEmpty())
            continue;
        QStringList words = line.split(" ");
        QVector<int> lineVector;
        for (const QString &word : words)
        {
            if(!word.isEmpty())
                lineVector.push_back(word.toInt());
        }
        if (!lineVector.isEmpty())
            kernel.push_back(lineVector);
    }
    _ip->_kernel = kernel;
}

void toolWidget::SlotDilation()
{
    if (!_img)
        return;

    if (_ip->_kernel.isEmpty())
        return;

    _ip->Dilation(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotErosion()
{
    if (!_img)
        return;

    if (_ip->_kernel.isEmpty())
        return;

    _ip->Erosion(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotOpening()
{
    if (!_img)
        return;

    _ip->Opening(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotClosing()
{
    if (!_img)
        return;

    _ip->Closing(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotBoundaryDetection()
{
    if (!_img)
        return;
    _ip->BoundaryDetection(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotNoiseRemoval()
{
    if (!_img)
        return;
    _ip->NoiseRemoval(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotChangeMaskSize(int value)
{
    value = 2 * value - 1;
    ui->maskSizeValue->setText(QString::number(value));
    _ip->_maskSize = value;
}

void toolWidget::SlotUpdateFilterKernel()
{
    QString content = ui->customFilterKernelTextEditor->toPlainText();
    QStringList lines = content.split("\n");
    QVector<QVector<int>> kernel;
    for (const QString &line : lines)
    {
        if (line.isEmpty())
            continue;
        QStringList words = line.split(" ");
        qDebug() << "words :" << words;
        QVector<int> lineVector;
        for (const QString &word : words)
        {
            if (!word.isEmpty())
                lineVector.push_back(word.toInt());
        }
        if (!lineVector.isEmpty())
            kernel.push_back(lineVector);
    }
    qDebug() << kernel;
    _ip->_customLowPassFilterKernel = kernel;
}

void toolWidget::SlotCustomLowPassFilter()
{
    if (!_img)
        return;
    if (_ip->_customLowPassFilterKernel.isEmpty())
        return;

    _ip->CustomLowPassFilter(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotAverage()
{
    if (!_img)
        return;

    _ip->Average(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotGaussian()
{
    if (!_img)
        return;

    _ip->Gaussian(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotMaximum()
{
    if (!_img)
        return;

    _ip->Maximum(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotMinimum()
{
    if (!_img)
        return;

    _ip->Minimum(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotMedian()
{
    if (!_img)
        return;

    _ip->Median(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotUpdateHighPassFilterKernelX()
{
    QString content = ui->customHighPassFilterKernelXTextEditor->toPlainText();
    QStringList lines = content.split("\n");
    QVector<QVector<int>> kernel;
    for (const QString &line : lines)
    {
        if (line.isEmpty())
            continue;
        QStringList words = line.split(" ");
        qDebug() << "words :" << words;
        QVector<int> lineVector;
        for (const QString &word : words)
        {
            if (!word.isEmpty())
                lineVector.push_back(word.toInt());
        }
        if (!lineVector.isEmpty())
            kernel.push_back(lineVector);
    }
    qDebug() << kernel;
    _ip->_customHighPassFilterKernelX = kernel;
}

void toolWidget::SlotUpdateHighPassFilterKernelY()
{
    QString content = ui->customHighPassFilterKernelYTextEditor->toPlainText();
    QStringList lines = content.split("\n");
    QVector<QVector<int>> kernel;
    for (const QString &line : lines)
    {
        if (line.isEmpty())
            continue;
        QStringList words = line.split(" ");
        QVector<int> lineVector;
        for (const QString &word : words)
        {
            if (!word.isEmpty())
                lineVector.push_back(word.toInt());
        }
        if (!lineVector.isEmpty())
            kernel.push_back(lineVector);
    }
    _ip->_customHighPassFilterKernelY = kernel;
}

void toolWidget::SlotCustomHighPassFilter()
{
    if (!_img)
        return;
    //if (_ip->_customHighPassFilterKernelX.isEmpty() || _ip->_customHighPassFilterKernelY.isEmpty())
     //   return;

    _ip->CustomHighPassFilter(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotSobel()
{
    if (!_img)
        return;

    _ip->Sobel(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotPrewitt()
{
    if (!_img)
        return;

    _ip->Prewitt(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotSharpen()
{
    if (!_img)
        return;

    _ip->Sharpen(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotHorizontalFlip()
{
    if (!_img)
        return;

    *_img = _ip->ConvertOperation(_img);
    _ip->HorizontalFlip(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotVerticalFlip()
{
    if (!_img)
        return;

    *_img = _ip->ConvertOperation(_img);
    _ip->VerticalFlip(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotLeftFlip()
{
    if (!_img)
        return;

    *_img = _ip->ConvertOperation(_img);
    _ip->LeftFlip(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotRightFlip()
{
    if (!_img)
        return;

    *_img = _ip->ConvertOperation(_img);
    _ip->RightFlip(_img);
    emit SigUpdatePixmap();
}

void toolWidget::SlotChangeZoomImplementation(bool)
{
    if (!_img)
        return;
    if (ui->InterPolationRadio->isChecked())
        _ip->_isInterPolation = true;
    else
        _ip->_isInterPolation = false;

    _newImg = _ip->ConvertOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotChangeZoom(int value)
{
    if (!_img)
        return;

    double scaleValue = value / double(50);
    ui->zoomValue->setText(QString::number(scaleValue));
    _ip->_zoomValue = scaleValue;

    _newImg = _ip->ConvertOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}

void toolWidget::SlotRotate(int value)
{
    if (!_img)
        return;

    ui->rotateValue->setText(QString::number(value));
    _ip->_rotateValue = value;
    _newImg = _ip->ConvertOperation(_img);
    emit SigUpdatePreviewImg(_newImg);
}
