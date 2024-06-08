#include "mainwindow.h"

#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/style.qss");
    if (qss.open(QFile::ReadOnly))
    {
        qDebug("open qss successfully");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug("open qss failed");
        return 1;
    }

    MainWindow w;
    w.showMaximized();
    return a.exec();
}
