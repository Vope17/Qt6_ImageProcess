#include "quantization.h"
#include "ui_quantization.h"
#include <QtCharts>

quantization::quantization(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quantization)
{
    ui->setupUi(this);

}

quantization::quantization(QImage *img, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quantization)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QColor color;
    int h = img->height();
    int w = img->width();
    QVector<int> redScale(256, 0);
    QVector<int> greenScale(256, 0);
    QVector<int> blueScale(256, 0);
    QVector<int> grayScale(256, 0);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            color = img->pixelColor(x, y);
            redScale[color.red()]++;
            greenScale[color.green()]++;
            blueScale[color.blue()]++;
            grayScale[qGray(color.red(), color.green(), color.blue())]++;
        }
    }

    QBarSet *Rset = new QBarSet("Red");
    QBarSet *Gset = new QBarSet("Green");
    QBarSet *Bset = new QBarSet("Blue");
    QBarSet *Grayset = new QBarSet("Gray");

    for (int i = 0; i < 256; ++i)
    {
        *Rset << redScale[i];
        *Gset << greenScale[i];
        *Bset << blueScale[i];
        *Grayset << grayScale[i];
    }

    // Red Chart
    QBarSeries *Rseries = new QBarSeries();
    Rseries->append(Rset);
    Rset->setBrush(Qt::red);
    QChart *Rchart = new QChart();
    Rchart->addSeries(Rseries);
    Rchart->setTitle("Red Scale");
    Rchart->setAnimationOptions(QChart::SeriesAnimations);

    //QBarCategoryAxis *RaxisX = new QBarCategoryAxis();
    //QStringList categories;
    //for (int i = 0; i < 256; ++i)
    //    categories << QString::number(i);
    //RaxisX->append(categories);
    //Rchart->addAxis(RaxisX, Qt::AlignBottom);
    //Rseries->attachAxis(RaxisX);

    QValueAxis *RaxisY = new QValueAxis();
    RaxisY->setRange(0, 5000);
    Rchart->addAxis(RaxisY, Qt::AlignLeft);
    Rseries->attachAxis(RaxisY);

    Rchart->legend()->setVisible(true);
    Rchart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *RchartView = new QChartView(Rchart);
    RchartView->setRenderHint(QPainter::Antialiasing);

    // Green Chart
    QBarSeries *Gseries = new QBarSeries();
    Gseries->append(Gset);
    Gset->setBrush(Qt::green);
    QChart *Gchart = new QChart();
    Gchart->addSeries(Gseries);
    Gchart->setTitle("Green Scale");
    Gchart->setAnimationOptions(QChart::SeriesAnimations);

    //QBarCategoryAxis *GaxisX = new QBarCategoryAxis();
    //GaxisX->append(categories);
    //Gchart->addAxis(GaxisX, Qt::AlignBottom);
    //Gseries->attachAxis(GaxisX);

    QValueAxis *GaxisY = new QValueAxis();
    GaxisY->setRange(0, 5000);
    Gchart->addAxis(GaxisY, Qt::AlignLeft);
    Gseries->attachAxis(GaxisY);

    Gchart->legend()->setVisible(true);
    Gchart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *GchartView = new QChartView(Gchart);
    GchartView->setRenderHint(QPainter::Antialiasing);

    // Blue Chart
    QBarSeries *Bseries = new QBarSeries();
    Bseries->append(Bset);
    Bset->setBrush(Qt::blue);
    QChart *Bchart = new QChart();
    Bchart->addSeries(Bseries);
    Bchart->setTitle("Blue Scale");
    Bchart->setAnimationOptions(QChart::SeriesAnimations);

    //QBarCategoryAxis *BaxisX = new QBarCategoryAxis();
    //BaxisX->append(categories);
    //Bchart->addAxis(BaxisX, Qt::AlignBottom);
    //Bseries->attachAxis(BaxisX);

    QValueAxis *BaxisY = new QValueAxis();
    BaxisY->setRange(0, 5000);
    Bchart->addAxis(BaxisY, Qt::AlignLeft);
    Bseries->attachAxis(BaxisY);

    Bchart->legend()->setVisible(true);
    Bchart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *BchartView = new QChartView(Bchart);
    BchartView->setRenderHint(QPainter::Antialiasing);

    // Gray Chart
    QBarSeries *Grayseries = new QBarSeries();
    Grayseries->append(Grayset);
    Grayset->setBrush(Qt::gray);
    QChart *Graychart = new QChart();
    Graychart->addSeries(Grayseries);
    Graychart->setTitle("Gray Scale");
    Graychart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *GrayaxisY = new QValueAxis();
    GrayaxisY->setRange(0, 5000);
    Graychart->addAxis(GrayaxisY, Qt::AlignLeft);
    Grayseries->attachAxis(GrayaxisY);

    Graychart->legend()->setVisible(true);
    Graychart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *GraychartView = new QChartView(Graychart);
    GraychartView->setRenderHint(QPainter::Antialiasing);

    ui->gridLayout->addWidget(RchartView, 0, 0);
    ui->gridLayout->addWidget(GchartView, 0, 1);
    ui->gridLayout->addWidget(BchartView, 1, 0);
    ui->gridLayout->addWidget(GraychartView, 1, 1);

}


quantization::~quantization()
{
    delete ui;
}
