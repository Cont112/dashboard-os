#include "Processor.h"



Processor::Processor(QObject *parent) : QThread(parent) {
    mutex.lock();
    running = false;
    mutex.unlock();
}

Processor::~Processor() {
    stop();
    wait();
}

void Processor::start() {
    mutex.lock();
    if (!running) {
        running = true;
    }
    mutex.unlock();
    QThread::start();
}

void Processor::stop() {
    mutex.lock();
    running = false;
    dataAvailable.wakeOne();
    mutex.unlock();
}


void Processor::run() {
    qDebug() << "Processor started";
    
    while (true) {
        {
            QMutexLocker locker(&mutex);
            while (dataQueue.isEmpty() && running) {
                dataAvailable.wait(&mutex);
            }
            
            if (!running) {
                break;
            }
        }

        processData();

        {
            QMutexLocker locker(&mutex);
            if (!running) {
                break;
            }
        }
    }
    
    qDebug() << "Processor stopped";
}

void Processor::onDataCollected(const QVector<process_t>& processes) {
    QMutexLocker locker(&mutex);
    qDebug() << "Processor received data";
    for(const auto& process : processes) {
        dataQueue.push_back(process);
    }
    dataAvailable.wakeOne();
}

void Processor::processData() {
    QMutexLocker locker(&mutex);
    while(!dataQueue.empty()) {
        process_t process = dataQueue.front();
        dataQueue.pop_front();
        process.processed = true;
        qDebug() << "Processing process:" << process.name << "PID:" << process.pid << "PPID:" << process.ppid << "Threads:" << process.threads.size();
        for(const auto& thread : process.threads) {
            qDebug() << "Thread:" << thread.tid << "CPU:" << thread.cpu << "State:" << thread.state << "Start time:" << thread.start_time;
        }
    }

}

QString Processor::formatMemorySize(long long bytes) {
    if (bytes < 1024) {
        return QString::number(bytes) + " B";
    }
    if (bytes < 1024 * 1024) {
        return QString::number(bytes / 1024.0, 'f', 2) + " KB";
    }
    if (bytes < 1024 * 1024 * 1024) { 
        return QString::number(bytes / (1024.0 * 1024.0), 'f', 2) + " MB";
    }
    return QString::number(bytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

QString Processor::formatTime(long long seconds) {
    if (seconds < 60) {
        return QString::number(seconds) + "s";
    }
    if (seconds < 3600) {
        return QString::number(seconds / 60.0, 'f', 2) + "m";
    }
    return QString::number(seconds / 3600.0, 'f', 2) + "h";
}
