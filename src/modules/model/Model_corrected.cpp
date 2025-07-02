#include "Model.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

Model::Model(QObject *parent) : QAbstractTableModel(parent), m_selectedProcessPid(-1) {
    collector = new Collector(this);
    processor = new Processor(this);
    
    // Conexões existentes
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

    // Novas conexões para sistema de arquivos
    QObject::connect(collector, &Collector::filesystemInfoCollected,
                    processor, &Processor::onFilesystemInfoCollected,
                    Qt::QueuedConnection);
    
    QObject::connect(processor, &Processor::filesystemUpdated,
                    this, &Model::handleFilesystemUpdated,
                    Qt::QueuedConnection);

    // Conectar navegação de diretórios
    QObject::connect(this, &Model::requestDirectoryNavigation,
                    collector, &Collector::requestDirectoryNavigation,
                    Qt::QueuedConnection);
    
    QObject::connect(this, &Model::requestPartitionUpdate,
                    collector, &Collector::requestPartitionUpdate,
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
    return 12; // Expandido para incluir informações de E/S
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
    // Novos roles para E/S
    roles[OpenFilesCountRole] = "open_files_count";
    roles[IpcResourcesCountRole] = "ipc_resources_count";
    roles[ReadBytesRole] = "read_bytes";
    roles[WriteBytesRole] = "write_bytes";
    roles[ReadSyscallsRole] = "read_syscalls";
    roles[WriteSyscallsRole] = "write_syscalls";
    return roles;
}

QVariant Model::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= currentProcesses.size())
        return QVariant();

    const process_t& process = currentProcesses.at(index.row());

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
        // Novos roles para E/S
        case OpenFilesCountRole:
            return process.io_details.open_files.size();
        case IpcResourcesCountRole:
            return process.io_details.ipc_resources.size();
        case ReadBytesRole:
            return QVariant::fromValue(process.io_details.read_bytes);
        case WriteBytesRole:
            return QVariant::fromValue(process.io_details.write_bytes);
        case ReadSyscallsRole:
            return QVariant::fromValue(process.io_details.read_syscalls);
        case WriteSyscallsRole:
            return QVariant::fromValue(process.io_details.write_syscalls);
        default:
            return QVariant();
    }
}

// Implementação dos novos métodos para sistema de arquivos
QVariantList Model::partitions() const {
    QVariantList list;
    for (const auto& partition : currentStats.filesystem_info.partitions) {
        QVariantMap partitionMap;
        partitionMap["device"] = partition.device;
        partitionMap["mountpoint"] = partition.mountpoint;
        partitionMap["filesystem"] = partition.filesystem;
        partitionMap["options"] = partition.options;
        partitionMap["total_space"] = QVariant::fromValue(partition.total_space);
        partitionMap["used_space"] = QVariant::fromValue(partition.used_space);
        partitionMap["available_space"] = QVariant::fromValue(partition.available_space);
        partitionMap["usage_percentage"] = partition.usage_percentage;
        list.append(partitionMap);
    }
    return list;
}

QVariantList Model::directoryContents() const {
    QVariantList list;
    for (const auto& file : currentStats.filesystem_info.directory_contents) {
        QVariantMap fileMap;
        fileMap["name"] = file.name;
        fileMap["path"] = file.path;
        fileMap["type"] = file.type;
        fileMap["permissions"] = file.permissions;
        fileMap["owner"] = file.owner;
        fileMap["group"] = file.group;
        fileMap["size"] = QVariant::fromValue(file.size);
        fileMap["modified_time"] = file.modified_time;
        fileMap["is_directory"] = file.is_directory;
        fileMap["is_hidden"] = file.is_hidden;
        list.append(fileMap);
    }
    return list;
}

void Model::setSelectedProcessPid(int pid) {
    if (m_selectedProcessPid != pid) {
        m_selectedProcessPid = pid;
        emit selectedProcessChanged();
    }
}

QVariantMap Model::getProcessIODetails(int pid) const {
    QVariantMap details;
    
    // Encontra o processo pelo PID
    auto it = std::find_if(currentProcesses.begin(), currentProcesses.end(),
                          [pid](const process_t& proc) { return proc.pid == pid; });
    
    if (it != currentProcesses.end()) {
        const process_t& process = *it;
        
        details["name"] = process.name;
        details["state"] = process.state;
        details["username"] = process.username;
        details["thread_count"] = process.threads.size();
        details["read_bytes"] = QVariant::fromValue(process.io_details.read_bytes);
        details["write_bytes"] = QVariant::fromValue(process.io_details.write_bytes);
        details["read_syscalls"] = QVariant::fromValue(process.io_details.read_syscalls);
        details["write_syscalls"] = QVariant::fromValue(process.io_details.write_syscalls);
        
        // Arquivos abertos
        QVariantList openFilesList;
        for (const auto& file : process.io_details.open_files) {
            QVariantMap fileMap;
            fileMap["fd"] = file.fd;
            fileMap["path"] = file.path;
            fileMap["type"] = file.type;
            fileMap["permissions"] = file.permissions;
            fileMap["size"] = QVariant::fromValue(file.size);
            openFilesList.append(fileMap);
        }
        details["open_files"] = openFilesList;
        
        // Recursos IPC
        QVariantList ipcResourcesList;
        for (const auto& resource : process.io_details.ipc_resources) {
            QVariantMap resourceMap;
            resourceMap["type"] = resource.type;
            resourceMap["key"] = resource.key;
            resourceMap["permissions"] = resource.permissions;
            resourceMap["owner_pid"] = resource.owner_pid;
            ipcResourcesList.append(resourceMap);
        }
        details["ipc_resources"] = ipcResourcesList;
    }
    
    return details;
}

void Model::navigateToDirectory(const QString& path) {
    emit requestDirectoryNavigation(path);
}

void Model::navigateToParent() {
    QString currentDir = currentStats.filesystem_info.current_directory;
    if (!currentDir.isEmpty() && currentDir != "/") {
        QDir dir(currentDir);
        if (dir.cdUp()) {
            emit requestDirectoryNavigation(dir.absolutePath());
        }
    }
}

void Model::navigateToHome() {
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    emit requestDirectoryNavigation(homePath);
}

void Model::navigateToRoot() {
    emit requestDirectoryNavigation("/");
}

void Model::refreshPartitionInfo(const QString& device) {
    emit requestPartitionUpdate(device);
}

void Model::handleProcessedData(const QVector<process_t>& processedData) {
    beginResetModel();
    currentProcesses = processedData;
    endResetModel();
}

void Model::handleSystemStatsUpdated(const system_stats_t& stats) {
    currentStats = stats;
    emit systemStatsChanged(); 
}

void Model::handleFilesystemUpdated(const filesystem_info_t& filesystemInfo) {
    currentStats.filesystem_info = filesystemInfo;
    emit filesystemChanged();  
} 