#include "qglobal.h"
#include "qnamespace.h"
#include "qwidget.h"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

class Dashboard : public QMainWindow {
  Q_OBJECT
public:
  Dashboard(QWidget *parent = nullptr) : QMainWindow(parent) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    qDebug() << "contrutora";
    parseMemory();
  }

private slots:
  void parseMemory() {
    qDebug() << "pre parsing";
    QFile meminfo("/proc/meminfo");

    if (!meminfo.open(QIODevice::ReadOnly)) {
      qDebug() << "não foi meminfo"; 
      return;
    }

    QTextStream in_meminfo(&meminfo);
    QStringList tokens = in_meminfo.readLine().split(" ", Qt::SkipEmptyParts);

    long long memtotal = tokens[1].toLongLong();
    
    qDebug() << tokens[0] << memtotal;
  }

private:
  QWidget *centralWidget;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Dashboard dashboard;
  dashboard.show();

  return app.exec();
}

#include "main.moc"
