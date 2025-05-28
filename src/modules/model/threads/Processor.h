#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../model_common.h"
#include <QWaitCondition>

class Processor : public QThread {
    Q_OBJECT
public:
    Processor(QObject *parent = nullptr);
    ~Processor();
    void run() override;
    void stop();
    void start();

signals:
    void dataProcessed(const QMap<int, process_t>& processedData);
    void systemStatsUpdated(const system_stats_t& stats);

public slots:
    void onDataCollected(const QVector<process_t>& processes);

private:
    bool running;
    QMutex mutex;
    QWaitCondition dataAvailable;
    QVector<process_t> dataQueue;
    
    void processData();

    QString formatMemorySize(long long bytes);
    QString formatTime(long long seconds);
};

#endif 