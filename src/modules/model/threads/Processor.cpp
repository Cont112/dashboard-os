#include "Processor.h"
#include <unistd.h>
#include <algorithm>

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
    wakeCondition.wakeOne(); // Acorda a thread para que ela termine a execução
    mutex.unlock();
}

void Processor::run() {
    qDebug() << "Processor started";
    
    while (true) {
        {
            QMutexLocker locker(&mutex);
            if (!running) {
                break;
            }
            
            if (dataQueue.isEmpty()) {
                wakeCondition.wait(&mutex);
                if (!running) {
                    break;
                }
            }
        }

        processData();
    }
    
    qDebug() << "Processor stopped";
}

void Processor::onDataCollected(const QVector<process_t>& processes) {
    QMutexLocker locker(&mutex);
    dataQueue.clear(); // Limpa a fila de dados
    for(const auto& process : processes) {
        dataQueue.push_back(process); // Adiciona os dados recebidos na fila de dados
    }
    wakeCondition.wakeOne(); // Acorda a thread
}

void Processor::processData() {
    QMutexLocker locker(&mutex);
    processedData.clear();
    
    while(!dataQueue.empty()) {
        process_t process = dataQueue.front();
        dataQueue.pop_front();
        
        if (currentStats.total_physical_memory > 0) {
            process.memory_details.memory_percentage = 
                (process.memory_details.ram_memory * 100.0) / currentStats.total_physical_memory;
        } else {
            process.memory_details.memory_percentage = 0.0;
        }
        
        process.processed = true;
        processedData.push_back(process);
    }

    calculateProcessPercentages();
    updateProcessStates();

    emit dataProcessed(processedData);
}

void Processor::onSystemInfoCollected(const system_stats_t& stats) {
    QMutexLocker locker(&mutex);
    currentStats = stats;
    
    if (stats.total_physical_memory > 0) {

        
        // Calcula a porcentagem de uso de memória do sistema
        currentStats.system_memory_usage = 
            ((stats.total_physical_memory - stats.available_physical_memory) * 100.0) / stats.total_physical_memory;
        
        currentStats.total_processes = processedData.size();
        currentStats.total_threads = 0;
        for(const auto& proc : processedData) {
            currentStats.total_threads += proc.threads.size();
        }
        
        updateProcessStates();
        
        emit systemStatsUpdated(currentStats);
    }
    
    
    if (stats.total_physical_memory > 0 && !processedData.isEmpty()) {
        for (auto& process : processedData) {
            if (process.memory_details.memory_percentage == 0.0 && process.memory_details.ram_memory > 0) {
                process.memory_details.memory_percentage = 
                    (process.memory_details.ram_memory * 100.0) / stats.total_physical_memory;
            }
        }
        
        emit dataProcessed(processedData);
    }
}

void Processor::updateProcessStates() {
    currentStats.running_processes = 0;
    currentStats.sleeping_processes = 0;
    currentStats.stopped_processes = 0;
    currentStats.zombie_processes = 0;
    currentStats.idle_processes = 0;
    
    for(const auto& proc : processedData) {
        if (proc.state == "R") {
            currentStats.running_processes++;
        } else if (proc.state == "S" || proc.state == "D") {
            currentStats.sleeping_processes++;
        } else if (proc.state == "T" || proc.state == "t") {
            currentStats.stopped_processes++;
        } else if (proc.state == "Z") {
            currentStats.zombie_processes++;
        } else if (proc.state == "I") {
            currentStats.idle_processes++;
        }
    }
}

void Processor::calculateProcessPercentages() {

    double timeIntervalSeconds = UPDATE_INTERVAL / 1000.0;
    long long clock = sysconf(_SC_CLK_TCK);
    
    if (clock <= 0) {
        for (auto& process : processedData) {
            process.cpu_percentage = 0.0;
            previousCpuTime[process.pid] = process.utime + process.stime;
        }
        return;
    }
    
    for (auto& process : processedData) {
        long long currentProcessCpuTime = process.utime + process.stime;
        
        if (previousCpuTime.contains(process.pid)) {
            long long previousProcessCpuTime = previousCpuTime[process.pid];
            long long cpuTimeDelta = currentProcessCpuTime - previousProcessCpuTime;
            
            double processCpuSeconds = (double)cpuTimeDelta / clock;
            
            process.cpu_percentage = (processCpuSeconds / timeIntervalSeconds) * 100.0;
            
            if (process.cpu_percentage < 0.0) {
                process.cpu_percentage = 0.0;
            } else if (process.cpu_percentage > 200.0) {
                process.cpu_percentage = 200.0;
            }
        } else {
            process.cpu_percentage = 0.0;
        }
        
        previousCpuTime[process.pid] = currentProcessCpuTime;
    }
}

void Processor::onFilesystemInfoCollected(const filesystem_info_t& filesystemInfo) {
    QMutexLocker locker(&mutex);
    currentFilesystemInfo = filesystemInfo;
    
    processFilesystemData();
    
    emit filesystemUpdated(currentFilesystemInfo);
}

void Processor::processFilesystemData() {
    calculatePartitionUsage();
    sortDirectoryContents();
}

void Processor::calculatePartitionUsage() {
    for (auto& partition : currentFilesystemInfo.partitions) {
        if (partition.total_space > 0) {
            partition.usage_percentage = 
                (static_cast<double>(partition.used_space) / partition.total_space) * 100.0;
        } else {
            partition.usage_percentage = 0.0;
        }
    }
}

void Processor::sortDirectoryContents() {
    std::sort(currentFilesystemInfo.directory_contents.begin(),
              currentFilesystemInfo.directory_contents.end(),
              [](const file_info_t& a, const file_info_t& b) {
                  if (a.is_directory != b.is_directory) {
                      return a.is_directory > b.is_directory; // Diretórios primeiro
                  }
                  return a.name.toLower() < b.name.toLower(); // Ordem alfabética
              });
}