#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "../model_common.h"
#include <QThread>
#include <QMutex>
#include <QVector>
#include <QString>
#include <QDir>

class Collector : public QThread {
    Q_OBJECT
public:
    Collector(QObject *parent = nullptr);
    ~Collector();
    void run() override;
    void stop();
    void start();

signals:
    void dataCollected(const QVector<process_t>& processes);

private:
    bool running;
    QMutex mutex;
    void readProc();
    void parseProcStat(process_t& proc, QString content);
    
    void collectThreadInfo(process_t& proc);
    void parseThreadStat(thread_info_t& thread, QString content);
    

};

#endif