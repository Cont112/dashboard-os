#ifndef MODEL_H
#define MODEL_H

#include "model_common.h"
#include "threads/Collector.h"
#include "threads/Processor.h"


class Model : public QObject {
    Q_OBJECT
public:
    Model(QObject *parent = nullptr);
    ~Model();

    void startMonitoring();
    void stopMonitoring();

signals:
    void dataUpdated(const QMap<int, process_t>& processes);
    void systemStatsUpdated(const system_stats_t& stats);
    void dataCollected(const QVector<process_t>& processes);

private slots:
    void handleCollectedData(const QVector<process_t>& processes);
    void handleProcessedData(const QMap<int, process_t>& processedData);

private:
    Collector* collector;
    Processor* processor;
    QMap<int, process_t> currentProcesses;
    system_stats_t currentStats;
    QMutex dataMutex;
    QTimer* updateTimer;
};

#endif