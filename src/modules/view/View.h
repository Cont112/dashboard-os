#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QChart>
#include <QChartView>
#include <QTimer>
#include <QVector>
#include "Model.h"

QT_CHARTS_USE_NAMESPACE

class View : public QMainWindow {
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);
    ~View();

public slots:
    void updateProcessList(const QVector<process_t>& processes);
    void updateSystemStats(const QMap<QString, QVariant>& stats);
    void handleProcessSelection(int row, int column);

signals:
    void userAction(const QString& action, const QVariant& data);

private:
    void setupUI();
    void createProcessTable();
    void createSystemStatsPanel();
    void createProcessDetailsPanel();
    void updateCharts();

    QTableWidget* processTable;
    QChart* cpuChart;
    QChart* memoryChart;
    QChartView* cpuChartView;
    QChartView* memoryChartView;
    QWidget* systemStatsPanel;
    QWidget* processDetailsPanel;
};

#endif // VIEW_H
