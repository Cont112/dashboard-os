#include "Collector.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <unistd.h>

Collector::Collector(QObject *parent) : QThread(parent) {
    mutex.lock();
    running = false;
    mutex.unlock();
}

Collector::~Collector() {
    stop();
    wait();
}

void Collector::start() {
    mutex.lock();
    if (!running) {
        running = true;
    }
    mutex.unlock();
    QThread::start();
}

void Collector::stop() {
    mutex.lock();
    running = false;
    mutex.unlock();
}

void Collector::readProc() {
    QVector<process_t> process_list;
    QDir dir(PROC_PATH);
    
    QStringList processes = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
    qDebug() << "Found" << processes.size() << "directories in /proc";
    
    for (const QString &process : processes) {
        bool is_process_dir = false;
        process.toInt(&is_process_dir);
        if (!is_process_dir) {
            continue;
        }

        {
            QMutexLocker locker(&mutex);
            if (!running) {
                return;
            }
        }

        if (!dir.cd(process)) {
            continue;
        }

        process_t proc;
        QFile statFile(dir.filePath("stat"));
        
        if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&statFile);
            QString statContent = in.readAll();
            parseProcStat(proc, statContent);
            statFile.close();
            
            collectThreadInfo(proc);
            
            process_list.push_back(proc);
        }
        
        dir.cdUp();
    }

    qDebug() << "Collected" << process_list.size() << "processes";
    emit dataCollected(process_list);
}

void Collector::parseProcStat(process_t& proc, QString content) {
    QStringList tokens = content.split(QChar::Space);
    if (tokens.size() < 39) {
        qDebug() << "Invalid stat content, got" << tokens.size() << "tokens";
        return;
    }

    proc.pid = tokens[0].toInt();

    QString rawName = tokens[1];
    proc.name = rawName.mid(1, rawName.length() - 2);
    proc.cpu = tokens[38].toInt();
    proc.ppid = tokens[3].toInt();
    proc.state = tokens[2];
    proc.start_time = (sysconf(_SC_CLK_TCK) != - 1 ) ? tokens[21].toLongLong() / sysconf(_SC_CLK_TCK) : 0;
    proc.memory_details.total_memory = tokens[22].toLongLong();
    proc.memory_details.ram_memory = tokens[23].toLongLong();
}

void Collector::collectThreadInfo(process_t& proc) {
    QString taskDir = QString(PROC_PATH "/%1/task").arg(proc.pid);
    QDir dir(taskDir);
    QStringList threadDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (const QString& threadDir : threadDirs) {
        bool is_thread_dir = false;
        threadDir.toInt(&is_thread_dir);
        if (!is_thread_dir) continue;
        
        thread_info_t thread = {};
        thread.tid = threadDir.toInt();
        
        QString statPath = QString("%1/%2/stat").arg(taskDir).arg(threadDir);
        QFile statFile(statPath);
        
        if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&statFile);
            QString statContent = in.readAll();
            parseThreadStat(thread, statContent);
            statFile.close();
            
            proc.threads.append(thread);
        }
    }
}

void Collector::parseThreadStat(thread_info_t& thread, QString content) {
    QStringList tokens = content.split(QChar::Space);
    if (tokens.size() < 39) {
        qDebug() << "Invalid thread stat content, got" << tokens.size() << "tokens";
        return;
    }

    thread.state = tokens[2];
    thread.cpu = tokens[38].toInt();
    thread.start_time = (sysconf(_SC_CLK_TCK) != -1) ? tokens[21].toLongLong() / sysconf(_SC_CLK_TCK) : 0;
}

void Collector::run() {
    qDebug() << "DataCollector started";
    
    while (true) {
        {
            QMutexLocker locker(&mutex);
            if (!running) {
                break;
            }
        }

        readProc();
        
        {
            QMutexLocker locker(&mutex);
            if (!running) {
                break;
            }
        }
        
        QThread::msleep(UPDATE_INTERVAL);
    }
    
    qDebug() << "DataCollector stopped";
}
