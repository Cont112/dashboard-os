#include "Collector.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <unistd.h>
#include <pwd.h>

Collector::Collector(QObject *parent) : QThread(parent) {
    mutex.lock();
    running = false;
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
