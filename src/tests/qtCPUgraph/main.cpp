#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

class Dashboard : public QMainWindow {
    Q_OBJECT
public:
    Dashboard(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Set up the chart
        series = new QtCharts::QLineSeries();
        chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle("CPU Usage (%)");
        chart->axes(Qt::Horizontal).first()->setRange(0, 60); // 60 seconds
        chart->axes(Qt::Vertical).first()->setRange(0, 100); // 0-100%

        // Set up the chart view
        chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(chartView);
        resize(800, 600);

        // Initialize CPU usage tracking
        lastIdle = lastTotal = 0;
        x = 0;

        // Set up timer to update every second
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Dashboard::updateCpuUsage);
        timer->start(1000);
    }

private slots:
    void updateCpuUsage() {
        // Read CPU stats from /proc/stat
        QFile file("/proc/stat");
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QTextStream in(&file);
        QString line = in.readLine();
        file.close();

        // Parse CPU line (e.g., "cpu  user nice system idle ...")
        QStringList tokens = line.split(" ", Qt::SkipEmptyParts);
        if (tokens.size() < 5) {
            return;
        }

        // Calculate total and idle CPU time
        long user = tokens[1].toLong();
        long nice = tokens[2].toLong();
        long system = tokens[3].toLong();
        long idle = tokens[4].toLong();
        long total = user + nice + system + idle; // Simplified; add iowait, irq, etc., for precision

        // Calculate CPU usage percentage
        if (lastTotal > 0) {
            double usage = 100.0 * (1.0 - (idle - lastIdle) / (double)(total - lastTotal));
            series->append(x, usage);
            x += 1;

            // Scroll the chart after 60 seconds
            if (x > 60) {
                chart->axes(Qt::Horizontal).first()->setRange(x - 60, x);
            }
        }

        lastIdle = idle;
        lastTotal = total;
    }

private:
    QtCharts::QLineSeries *series;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
    long lastIdle, lastTotal;
    double x;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Dashboard dashboard;
    dashboard.show();
    return app.exec();
}

#include "main.moc" // Required for CMake/Moc
