#include "Model.h"

Model::Model(QObject *parent) : QObject(parent) {
    collector = new Collector(this);
    processor = new Processor(this);
    
    QObject::connect(collector, &Collector::dataCollected, 
                    this, &Model::handleCollectedData,
                    Qt::QueuedConnection);
    
    QObject::connect(collector, &Collector::dataCollected,
                    processor, &Processor::onDataCollected,
                    Qt::QueuedConnection);
    
    QObject::connect(processor, &Processor::dataProcessed,
                    this, &Model::handleProcessedData,
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

void Model::handleCollectedData(const QVector<process_t>& processes) {
    emit dataCollected(processes);
}

void Model::handleProcessedData(const QMap<int, process_t>& processedData) {
    currentProcesses = processedData;
}