#include "Model.h"
#include <QDebug>

Model::Model(QObject *parent) : QAbstractTableModel(parent) {
    collector = new Collector(this);
    processor = new Processor(this);
    
    QObject::connect(collector, &Collector::dataCollected,
                    processor, &Processor::onDataCollected,
                    Qt::QueuedConnection);
    
    QObject::connect(collector, &Collector::systemInfoCollected,
                    processor, &Processor::onSystemInfoCollected,
                    Qt::QueuedConnection);
    
    QObject::connect(processor, &Processor::dataProcessed,
                    this, &Model::handleProcessedData,
                    Qt::QueuedConnection);
    
    QObject::connect(processor, &Processor::systemStatsUpdated,
                    this, &Model::handleSystemStatsUpdated,
                    Qt::QueuedConnection);
}

Model::~Model() {
    stopMonitoring();
    delete collector;
    delete processor;
}

void Model::startMonitoring() {
    collector->start();
    processor->start();
}

void Model::stopMonitoring() {
    collector->stop();
    processor->stop();
}

int Model::rowCount(const QModelIndex &) const {
    return currentProcesses.size();
}

int Model::columnCount(const QModelIndex &) const {
    return 7; // pid, name, state, cpu%, memory%, user, numero de threads
}

QHash<int, QByteArray> Model::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PidRole] = "pid";
    roles[NameRole] = "name";
    roles[CpuUsageRole] = "cpu_usage";
    roles[MemoryUsageRole] = "memory_usage";
    roles[StateRole] = "state";
    roles[UsernameRole] = "username";
    roles[ThreadsRole] = "threads";
    return roles;
}

QVariant Model::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= currentProcesses.size())
        return QVariant();

    auto it = std::next(currentProcesses.begin(), index.row());
    const process_t& process = it.value();

    switch (role) {
        case PidRole:
            return process.pid;
        case NameRole:
            return process.name;
        case CpuUsageRole:
            return process.cpu_percentage;
        case MemoryUsageRole:
            return process.memory_details.memory_percentage;
        case StateRole:
            return process.state;
        case UsernameRole:
            return process.username;
        case ThreadsRole:
            return process.threads.size();
        default:
            return QVariant();
    }
}

void Model::handleProcessedData(const QVector<process_t>& processedData) {
    beginResetModel();
    
    currentProcesses.clear();
    for(const auto& process : processedData) {
        currentProcesses[process.pid] = process;
    }
    
    endResetModel();
}

void Model::handleSystemStatsUpdated(const system_stats_t& stats) {
    currentStats = stats;

    emit systemStatsChanged(); 
}