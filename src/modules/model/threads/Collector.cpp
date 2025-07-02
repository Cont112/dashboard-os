#include "Collector.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>
#include <QStandardPaths>
#include <unistd.h>
#include <pwd.h>
#include <sys/statvfs.h>
#include <sys/stat.h>

Collector::Collector(QObject *parent) : QThread(parent) {
    mutex.lock();
    running = false;
    directoryChangeRequested = false;
    partitionUpdateRequested = false;
    currentFilesystemInfo.current_directory = "/"; // Inicializar com raiz
    mutex.unlock();
}

Collector::~Collector() {
    stop();
    wait(); //Espera a thread terminar
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

void Collector::run() {
    qDebug() << "DataCollector started";
    
    while (true) {
        {
            //Limpa a lista e verifica se a thread deve continuar rodando
            QMutexLocker locker(&mutex);
            proc_list.clear();
            if (!running) {
                break;
            }
        }

        QStringList proc_dirs;

        readProc(proc_dirs);
        for (const QString &process : proc_dirs) {
            {
                QMutexLocker locker(&mutex);
                if (!running) {
                    break;
                }
            }
            
            if(!isValidDirectory(process)) continue;

            process_t proc;
            proc.pid = process.toInt();
            
            collectProcessInfo(proc);
            collectThreadInfo(proc);
            collectMemoryInfo(proc);
            collectUsername(proc);
            
            // Nova coleta de informações de E/S
            collectProcessIOInfo(proc);
            
            mutex.lock();
            proc_list.push_back(proc);
            mutex.unlock();
        }
        
        //Verifica se a thread deve continuar rodando antes de emitir os dados
        {
            QMutexLocker locker(&mutex);
            if (!running) {
                break;
            }
        }
        
        collectSystemInfo();
        
        // Coleta informações do sistema de arquivos periodicamente
        collectFilesystemInfo();
        
        emit dataCollected(proc_list);
        
        QThread::msleep(UPDATE_INTERVAL); // Espera UPDATE_INTERVAL segundos antes de rodar novamente
    }
    
    qDebug() << "DataCollector stopped";
}

void Collector::readProc(QStringList& proc_dirs) {
    QDir dir(PROC_PATH);
    
    proc_dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
}

bool Collector::isValidDirectory(const QString& name) {
    bool is_process_dir = false;
    name.toInt(&is_process_dir);
    return is_process_dir;
}

void Collector::collectProcessInfo(process_t& proc) {
    QString statPath = QString(PROC_PATH "/%1/stat").arg(proc.pid);
    QFile statFile(statPath);
    if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&statFile);
        QString statContent = in.readAll();
        parseProcessStat(proc, statContent);
        statFile.close();
    }
}

void Collector::parseProcessStat(process_t& proc, QString content) {
    int firstParen = content.indexOf('(');
    int lastParen = content.lastIndexOf(')');
    
    if (firstParen == -1 || lastParen == -1 || firstParen >= lastParen) {
        return;
    }
    
    //Separa o nome do processo do resto do conteúdo e ignora os parenteses
    //É feito dessa forma para evitar problemas com nomes de processo que podem conter espaços
    QString name = content.mid(firstParen + 1, lastParen - firstParen - 1);
    QString remaining = content.mid(lastParen + 1).trimmed();
    
    QStringList remainingTokens = remaining.split(QChar::Space, Qt::SkipEmptyParts);
    
    if (remainingTokens.size() < 37) { 
        return;
    }
    
    proc.name = name;
    proc.state = remainingTokens[0];         
    proc.ppid = remainingTokens[1].toInt();  
    proc.utime = remainingTokens[11].toLongLong(); 
    proc.stime = remainingTokens[12].toLongLong(); 
    proc.start_time = (sysconf(_SC_CLK_TCK) != -1) ? remainingTokens[19].toLongLong() / sysconf(_SC_CLK_TCK) : 0;
    proc.cpu = remainingTokens[36].toInt();
}

void Collector::collectThreadInfo(process_t& proc) {
    QString taskDir = QString(PROC_PATH "/%1/task").arg(proc.pid);
    QDir dir(taskDir);
    QStringList threadDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    
    //Itera sobre todas as threads no diretório /proc/[pid]/task
    for (const QString& threadDir : threadDirs) {
        if (!isValidDirectory(threadDir)) continue;
        
        thread_info_t thread = {};
        thread.tid = threadDir.toInt();
        
        QString statPath = QString("%1/%2/stat").arg(taskDir).arg(threadDir);
        QFile statFile(statPath);
        
        if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&statFile);
            QString statContent = in.readAll();
            parseThreadStat(thread, statContent);
            statFile.close();
            
            //Adiciona a thread ao processo
            proc.threads.append(thread);
        }
    }
}

void Collector::parseThreadStat(thread_info_t& thread, QString content) {
    QStringList tokens = content.split(QChar::Space);
    if (tokens.size() < 39) {
        return;
    }

    thread.state = tokens[2];
    thread.cpu = tokens[38].toInt();
    thread.start_time = (sysconf(_SC_CLK_TCK) != -1) ? tokens[21].toLongLong() / sysconf(_SC_CLK_TCK) : 0;
}

void Collector::collectMemoryInfo(process_t& proc) {
    QString statmPath = QString(PROC_PATH "/%1/statm").arg(proc.pid);
    QFile statmFile(statmPath);
    
    if (statmFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&statmFile);
        QString content = in.readAll();
        parseMemoryStat(proc, content);
        
        statmFile.close();
    }
}

void Collector::parseMemoryStat(process_t& proc, QString content) {
    QStringList tokens = content.split(QChar::Space);
    if (tokens.size() < 7) {
        return;
    }

    //Todos os dados de memória são em páginas, então é necessário converter para bytes atraves da syscall getpagesize()
    proc.memory_details.total_memory = tokens[0].toLongLong() * getpagesize(); // size 
    proc.memory_details.ram_memory = tokens[1].toLongLong() * getpagesize(); //resident memory
    proc.memory_details.shared_memory = tokens[2].toLongLong() * getpagesize(); //number of resident shared pages
    proc.memory_details.code_pages = tokens[3].toLongLong() * getpagesize();  //text code
    proc.memory_details.data_pages = tokens[5].toLongLong() * getpagesize(); // data + stack
}

void Collector::collectSystemInfo() {
    system_stats_t stats = {};
    
    collectCpuInfo(stats);
    collectSystemMemoryInfo(stats);
    
    emit systemInfoCollected(stats);
}

void Collector::collectCpuInfo(system_stats_t& stats) {
    QString statPath = QString(PROC_PATH "/stat");
    QFile statFile(statPath);
    if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&statFile);
        QString line = in.readLine(); // Lê apenas a primeira linha correspondente a CPUInfo
        QStringList tokens = line.split(QChar::Space, Qt::SkipEmptyParts);
        
        if (tokens.size() >= 8) {
            //Tempo de CPU em ticks
            long long user = tokens[1].toLongLong(); // Tempo em modo de usuário
            long long nice = tokens[2].toLongLong(); // Tempo em modo de usuário com prioridade mais baixa
            long long system = tokens[3].toLongLong(); // Tempo em modo de sistema
            long long idle = tokens[4].toLongLong(); // Tempo em modo de espera
            long long iowait = tokens[5].toLongLong(); // Tempo em modo de espera para E/S
            long long irq = tokens[6].toLongLong(); // Tempo em modo de interrupções
            long long softirq = tokens[7].toLongLong(); // Tempo em modo de softirq
            
            long long total = user + nice + system + idle + iowait + irq + softirq;
            long long used = total - (idle + iowait); // Tempo total - tempo ocioso
            
            //Calcula a porcentagem de uso da CPU por iteração
            if (is_first_cpu_iteration) {
                stats.system_cpu_usage = (total > 0) ? (used * 100.0 / total) : 0.0;
                is_first_cpu_iteration = false;
            } else {
                long long delta_total = total - prev_total_cpu;
                long long delta_used = used - prev_used_cpu;
                
                stats.system_cpu_usage = (delta_total > 0) ? (delta_used * 100.0 / delta_total) : 0.0;
            }
            
            prev_total_cpu = total;
            prev_used_cpu = used;
            
            stats.cpu_idle_percentage = 100.0 - stats.system_cpu_usage;
        }
        statFile.close();
    }
}

void Collector::collectSystemMemoryInfo(system_stats_t& stats) {
    QString meminfoPath = QString(PROC_PATH "/meminfo");
    QFile meminfoFile(meminfoPath);
    
    if (meminfoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&meminfoFile);
        QMap<QString, long long> memValues;
        
        QString allContent = in.readAll();

        QStringList lines = allContent.split('\n', Qt::SkipEmptyParts);
        
        for (const QString& line : lines) {
            QStringList parts = line.split(QChar::Space, Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                QString key = parts[0].chopped(1);
                long long value = parts[1].toLongLong();
                memValues[key] = value;
            }
        }
        
        stats.total_physical_memory = memValues["MemTotal"] * 1024; // Converte de KB para bytes
        stats.available_physical_memory = memValues["MemAvailable"] * 1024; // Converte de KB para bytes
            
        meminfoFile.close();
    } else {
        qDebug() << "Failed to open /proc/meminfo";
    }
}

void Collector::collectUsername(process_t& proc) {
    QString statusPath = QString(PROC_PATH "/%1/status").arg(proc.pid);
    QFile statusFile(statusPath);
    
    if (statusFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&statusFile);

        QString allContent = in.readAll();

        QStringList lines = allContent.split('\n', Qt::SkipEmptyParts);
        
        for (const QString& line : lines) {
            if (line.startsWith("Uid:")) {
                QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts); //Ignora espaços em branco de qualquer tipo
                if (parts.size() >= 2) {
                    uid_t uid = parts[1].toInt();
                    
                    struct passwd pwd;
                    struct passwd *result;
                    char buffer[1024];
                    
                    int ret = getpwuid_r(uid, &pwd, buffer, sizeof(buffer), &result); //Pega o nome do usuário do UID do processo
                    if (ret == 0 && result != nullptr) {
                        proc.username = QString(pwd.pw_name);
                    } else {
                        proc.username = QString::number(uid);
                    }
                }
                break;
            }
        }
        
        statusFile.close();
    }
    
    if (proc.username.isEmpty()) {
        proc.username = "unknown";
    }
}

// Implementação dos novos métodos para sistema de arquivos
void Collector::requestDirectoryNavigation(const QString& path) {
    QMutexLocker locker(&mutex);
    requestedDirectory = path;
    directoryChangeRequested = true;
}

void Collector::requestPartitionUpdate(const QString& device) {
    QMutexLocker locker(&mutex);
    requestedPartition = device;
    partitionUpdateRequested = true;
}

void Collector::collectProcessIOInfo(process_t& proc) {
    // Inicializar valores de E/S com 0
    proc.io_details.read_bytes = 0;
    proc.io_details.write_bytes = 0;
    proc.io_details.read_syscalls = 0;
    proc.io_details.write_syscalls = 0;
    proc.io_details.open_files.clear();
    proc.io_details.ipc_resources.clear();
    
    // Coletar informações de E/S
    collectOpenFiles(proc);
    collectIOStats(proc);
    collectIPCResources(proc);
}

void Collector::collectOpenFiles(process_t& proc) {
    QString fdDir = QString(PROC_PATH "/%1/fd").arg(proc.pid);
    QDir dir(fdDir);
    
    if (dir.exists()) {
        QStringList fdList = dir.entryList(QDir::Files);
        
        for (const QString& fdStr : fdList) {
            bool ok;
            int fd = fdStr.toInt(&ok);
            if (!ok) continue;
            
            open_file_t openFile;
            openFile.fd = fd;
            
            QString linkPath = QString("%1/%2").arg(fdDir).arg(fdStr);
            QString target = QFile::symLinkTarget(linkPath);
            
            if (!target.isEmpty()) {
                openFile.path = target;
                
                // Determinar tipo de arquivo com mais detalhes
                if (target.startsWith("/dev/")) {
                    openFile.type = "dev";
                } else if (target.startsWith("socket:[")) {
                    openFile.type = "socket";
                } else if (target.startsWith("pipe:[")) {
                    openFile.type = "pipe";
                } else if (target.startsWith("anon_inode:")) {
                    openFile.type = "anon";
                } else if (target == "/dev/null" || target == "/dev/zero") {
                    openFile.type = "special";
                } else {
                    // Arquivo regular
                    openFile.type = "file";
                    // Tentar obter tamanho do arquivo
                    QFileInfo fileInfo(target);
                    if (fileInfo.exists()) {
                        openFile.size = fileInfo.size();
                    }
                }
                
                openFile.permissions = "rw"; // Básico por enquanto
                
                proc.io_details.open_files.append(openFile);
            }
        }
    }
}

void Collector::collectIPCResources(process_t& proc) {
    // Coletar informações de recursos IPC através de /proc/[pid]/maps
    QString mapsPath = QString(PROC_PATH "/%1/maps").arg(proc.pid);
    QFile mapsFile(mapsPath);
    
    if (mapsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&mapsFile);
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            
            // Procurar por mapeamentos de memória compartilhada e outros recursos IPC
            if (line.contains("ipc") || line.contains("shm") || line.contains("sem")) {
                ipc_resource_t resource;
                
                QStringList parts = line.split(' ', Qt::SkipEmptyParts);
                if (parts.size() >= 6) {
                    resource.type = "shared_memory";
                    resource.key = parts[0]; // Endereço do mapeamento
                    resource.permissions = parts[1]; // Permissões
                    resource.owner_pid = proc.pid;
                    
                    proc.io_details.ipc_resources.append(resource);
                }
            }
        }
        mapsFile.close();
    }
    
    // Verificar recursos IPC do sistema
    collectSystemIPCResources(proc);
}

void Collector::collectSystemIPCResources(process_t& proc) {
    // Verificar semáforos do sistema
    QFile semFile("/proc/sysvipc/sem");
    if (semFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&semFile);
        in.readLine(); // Pular cabeçalho
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            
            if (parts.size() >= 6) {
                int ownerPid = parts[4].toInt();
                if (ownerPid == proc.pid) {
                    ipc_resource_t resource;
                    resource.type = "semaphore";
                    resource.key = parts[0];
                    resource.permissions = parts[2];
                    resource.owner_pid = ownerPid;
                    
                    proc.io_details.ipc_resources.append(resource);
                }
            }
        }
        semFile.close();
    }
    
    // Verificar filas de mensagens
    QFile msgFile("/proc/sysvipc/msg");
    if (msgFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&msgFile);
        in.readLine(); // Pular cabeçalho
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            
            if (parts.size() >= 6) {
                int ownerPid = parts[4].toInt();
                if (ownerPid == proc.pid) {
                    ipc_resource_t resource;
                    resource.type = "message_queue";
                    resource.key = parts[0];
                    resource.permissions = parts[2];
                    resource.owner_pid = ownerPid;
                    
                    proc.io_details.ipc_resources.append(resource);
                }
            }
        }
        msgFile.close();
    }
}

void Collector::collectIOStats(process_t& proc) {
    QString ioPath = QString(PROC_PATH "/%1/io").arg(proc.pid);
    QFile ioFile(ioPath);
    
    if (ioFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&ioFile);
        QString content = in.readAll();
        QStringList lines = content.split('\n', Qt::SkipEmptyParts);
        
        for (const QString& line : lines) {
            QStringList parts = line.split(QChar::Space, Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                QString key = parts[0].chopped(1); // Remove ':'
                long long value = parts[1].toLongLong();
                
                if (key == "read_bytes") {
                    proc.io_details.read_bytes = value;
                } else if (key == "write_bytes") {
                    proc.io_details.write_bytes = value;
                } else if (key == "syscr") {
                    proc.io_details.read_syscalls = value;
                } else if (key == "syscw") {
                    proc.io_details.write_syscalls = value;
                }
            }
        }
        ioFile.close();
    } else {
        // Se não conseguir ler o arquivo /proc/[pid]/io (comum para processos do kernel),
        // tentar obter informações básicas de outra forma
        
        // Para processos do kernel, podemos tentar informações do /proc/[pid]/stat
        QString statPath = QString(PROC_PATH "/%1/stat").arg(proc.pid);
        QFile statFile(statPath);
        if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&statFile);
            QString content = in.readAll();
            QStringList tokens = content.split(QChar::Space);
            
            // Estimativa grosseira baseada em dados do stat (não são exatos para E/S)
            if (tokens.size() > 40) {
                // Usar alguns campos como aproximação para atividade
                proc.io_details.read_syscalls = tokens[13].toLongLong(); // Minor faults
                proc.io_details.write_syscalls = tokens[14].toLongLong(); // Major faults
                proc.io_details.read_bytes = proc.io_details.read_syscalls * 4096; // Aproximação
                proc.io_details.write_bytes = proc.io_details.write_syscalls * 4096; // Aproximação
            }
            statFile.close();
        }
        
        // Para processos do kernel, definir valores padrões mais informativos
        if (proc.name.startsWith("[") && proc.name.endsWith("]")) {
            // É um processo do kernel, pode não ter E/S de arquivo tradicional
            proc.io_details.read_bytes = 0;
            proc.io_details.write_bytes = 0;
            proc.io_details.read_syscalls = 0;
            proc.io_details.write_syscalls = 0;
        }
    }
}

void Collector::collectFilesystemInfo() {
    filesystem_info_t info = currentFilesystemInfo; // Usar estado atual
    
    // Verifica se há uma mudança de diretório solicitada
    {
        QMutexLocker locker(&mutex);
        if (directoryChangeRequested) {
            info.current_directory = requestedDirectory;
            currentFilesystemInfo.current_directory = requestedDirectory;
            directoryChangeRequested = false;
        }
    }
    
    // Coleta informações de partições
    collectPartitionInfo(info);
    
    // Coleta conteúdo do diretório atual
    collectDirectoryContents(info.current_directory, info);
    
    // Atualiza o estado atual
    currentFilesystemInfo = info;
    
    emit filesystemInfoCollected(info);
}

void Collector::collectPartitionInfo(filesystem_info_t& filesystemInfo) {
    filesystemInfo.partitions.clear();
    
    QFile mountsFile("/proc/mounts");
    if (mountsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&mountsFile);
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            
            if (parts.size() >= 4) {
                QString device = parts[0];
                QString mountpoint = parts[1];
                QString filesystem = parts[2];
                QString options = parts[3];
                
                // Filtrar apenas sistemas de arquivos relevantes
                if (filesystem == "ext2" || filesystem == "ext3" || filesystem == "ext4" ||
                    filesystem == "xfs" || filesystem == "btrfs" || filesystem == "ntfs" ||
                    filesystem == "vfat" || filesystem == "tmpfs" || filesystem == "rootfs") {
                    
                    partition_info_t partition;
                    partition.device = device;
                    partition.mountpoint = mountpoint;
                    partition.filesystem = filesystem;
                    partition.options = options;
                    
                    // Usar statvfs para obter informações de espaço
                    struct statvfs stats;
                    if (statvfs(mountpoint.toLocal8Bit().constData(), &stats) == 0) {
                        partition.total_space = stats.f_blocks * stats.f_frsize;
                        partition.available_space = stats.f_bavail * stats.f_frsize;
                        partition.used_space = partition.total_space - (stats.f_bfree * stats.f_frsize);
                        
                        if (partition.total_space > 0) {
                            partition.usage_percentage = 
                                (static_cast<double>(partition.used_space) / partition.total_space) * 100.0;
                        } else {
                            partition.usage_percentage = 0.0;
                        }
                    } else {
                        partition.total_space = 0;
                        partition.used_space = 0;
                        partition.available_space = 0;
                        partition.usage_percentage = 0.0;
                    }
                    
                    filesystemInfo.partitions.append(partition);
                }
            }
        }
        mountsFile.close();
    }
}

void Collector::collectSinglePartitionInfo(const QString& device, partition_info_t& partition) {
    Q_UNUSED(device);
    Q_UNUSED(partition);
    // Implementação básica - pode ser expandida depois
}

void Collector::collectDirectoryContents(const QString& path, filesystem_info_t& filesystemInfo) {
    filesystemInfo.directory_contents.clear();
    filesystemInfo.current_directory = path;
    
    QDir dir(path);
    if (!dir.exists()) {
        return;
    }
    
    // Obter lista de entradas no diretório
    QStringList entries = dir.entryList(QDir::AllEntries | QDir::Hidden, QDir::Name);
    
    for (const QString& entry : entries) {
        // Pular . (diretório atual)
        if (entry == ".") continue;
        
        QString fullPath = dir.absoluteFilePath(entry);
        file_info_t fileInfo;
        
        collectFileInfo(fullPath, fileInfo);
        
        if (!fileInfo.name.isEmpty()) {
            filesystemInfo.directory_contents.append(fileInfo);
        }
    }
}

void Collector::collectFileInfo(const QString& path, file_info_t& fileInfo) {
    QFileInfo info(path);
    
    if (!info.exists()) {
        return;
    }
    
    fileInfo.name = info.fileName();
    fileInfo.path = path;
    fileInfo.size = info.size();
    fileInfo.is_directory = info.isDir();
    fileInfo.is_hidden = info.isHidden();
    fileInfo.modified_time = info.lastModified();
    fileInfo.owner = info.owner();
    fileInfo.group = info.group();
    
    // Determinar tipo de arquivo
    if (info.isDir()) {
        fileInfo.type = "directory";
    } else if (info.isSymLink()) {
        fileInfo.type = "symlink";
    } else if (info.isExecutable()) {
        fileInfo.type = "executable";
    } else {
        fileInfo.type = "file";
    }
    
    // Obter permissões usando stat()
    struct stat statBuf;
    if (stat(path.toLocal8Bit().constData(), &statBuf) == 0) {
        fileInfo.permissions = convertPermissions(statBuf.st_mode);
    } else {
        fileInfo.permissions = "?????????";
    }
}

QString Collector::convertPermissions(mode_t mode) {
    QString permissions;
    
    // Tipo de arquivo
    if (S_ISDIR(mode)) permissions += "d";
    else if (S_ISLNK(mode)) permissions += "l";
    else if (S_ISCHR(mode)) permissions += "c";
    else if (S_ISBLK(mode)) permissions += "b";
    else if (S_ISFIFO(mode)) permissions += "p";
    else if (S_ISSOCK(mode)) permissions += "s";
    else permissions += "-";
    
    // Permissões do proprietário
    permissions += (mode & S_IRUSR) ? "r" : "-";
    permissions += (mode & S_IWUSR) ? "w" : "-";
    permissions += (mode & S_IXUSR) ? "x" : "-";
    
    // Permissões do grupo
    permissions += (mode & S_IRGRP) ? "r" : "-";
    permissions += (mode & S_IWGRP) ? "w" : "-";
    permissions += (mode & S_IXGRP) ? "x" : "-";
    
    // Permissões de outros
    permissions += (mode & S_IROTH) ? "r" : "-";
    permissions += (mode & S_IWOTH) ? "w" : "-";
    permissions += (mode & S_IXOTH) ? "x" : "-";
    
    return permissions;
}
