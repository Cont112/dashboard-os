/**
 * @file Collector.h
 * @brief Header para a classe de threadCollector
 * @author Pedro Neves
 */

#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "../model_common.h"

/**
 * @brief Classe que implementa a thread de coleta de dados do diretório /proc
 * @details Essa classe é responsável por coletar os dados de processos e sistema
 */
class Collector : public QThread {
    Q_OBJECT

public:
    Collector(QObject *parent = nullptr);
    ~Collector();

    /**
     * @brief Rotina principal da thread
     * @details É chamada automaticamente pelo QThread quando a thread é iniciada
     */
    void run() override;

    /**
     * @brief Função que encerra a thread
     * @details Essa função é chamada pelo Model para parar a thread 
     */
    void stop();

    /**
     * @brief Função que inicia a thread
     * @details Essa função é chamada pelo Model para iniciar a thread
     */
    void start();

signals:
    /**
     * @brief Sinal emitido quando termina de coletar todos os dadosde processo disponiveis no /proc
     * @details Esse sinal é emitido pela thread diretamente ao Processor
     * @param processes Vetor de processos coletados
     */
    void dataCollected(const QVector<process_t>& processes);

    /**
     * @brief Sinal emitido quando termina de coletar todos os dados de sistema disponíveis no /proc
     * @details Esse sinal é emitido pela thread diretamente ao Processor
     * @param stats Struct com os dados de sistema coletados 
     */
    void systemInfoCollected(const system_stats_t& stats);

    /**
     * @brief Sinal emitido quando termina de coletar os dados do sistema de arquivos
     * @details Esse sinal é emitido pela thread diretamente ao Processor
     * @param filesystemInfo Struct com os dados do sistema de arquivos coletados
     */
    void filesystemInfoCollected(const filesystem_info_t& filesystemInfo);

public slots:
    /**
     * @brief Solicita navegação para um diretório específico
     * @param path Caminho do diretório de destino
     */
    void requestDirectoryNavigation(const QString& path);

    /**
     * @brief Solicita atualização de informações de partição
     * @param device Dispositivo da partição
     */
    void requestPartitionUpdate(const QString& device);

private:
    /**
     * @brief Função que lê os subdiretórios do /proc
     * @param proc_dirs Lista de subdiretórios do /proc
     */
    void readProc(QStringList& proc_dirs);

    /**
     * @brief Função que lê o conteúdo do arquivo /proc/[pid]/stat
     * @param proc Objeto do tipo process_t que será preenchido com os dados do processo
     */
    void collectProcessInfo(process_t& proc);
    /**
     * @brief Função que realiza o parsing do conteúdo do arquivo lido e popula o objeto process_t
     * @param proc Objeto do tipo process_t que será preenchido com os dados do processo
     * @param content Conteúdo do arquivo /proc/[pid]/stat 
     */
    void parseProcessStat(process_t& proc, QString content);

    /**
     * @brief Função que lê o conteúdo do arquivo /proc/[pid]/task/[tid]/stat
     * @param proc Objeto do tipo process_t que será preenchido com os dados da thread
     */
    void collectThreadInfo(process_t& proc);

    /**
     * @brief Função que realiza o parsing do conteúdo do arquivo lido e popula o objeto thread_info_t
     * @param thread Objeto do tipo thread_info_t que será preenchido com os dados da thread
     * @param content Conteúdo do arquivo /proc/[pid]/task/[tid]/stat
     */
    void parseThreadStat(thread_info_t& thread, QString content);

    /**
     * @brief Função que lê o conteúdo do arquivo /proc/[pid]/statm
     * @param proc Objeto do tipo process_t que será preenchido com os dados da memória do processo
     */
    void collectMemoryInfo(process_t& proc);

    /**
     * @brief Função que realiza o parsing do conteúdo do arquivo lido e popula o objeto memory_info_t
     * @param proc Objeto do tipo process_t que será preenchido com os dados da memória do processo
     * @param content Conteúdo do arquivo /proc/[pid]/statm
     */
    void parseMemoryStat(process_t& proc, QString content);

    /**
     * @brief Função que coleta o nome do usuário do processo atraves do UID no arquivo /proc/[pid]/status
     * @param proc Objeto do tipo process_t que será preenchido com o nome do usuário do processo
     */
    void collectUsername(process_t& proc);

    // Novos métodos para coleta de informações de E/S
    /**
     * @brief Função que coleta informações de E/S de um processo (arquivos abertos, recursos IPC, etc.)
     * @param proc Objeto do tipo process_t que será preenchido com os dados de E/S
     */
    void collectProcessIOInfo(process_t& proc);

    /**
     * @brief Função que coleta os arquivos abertos por um processo
     * @param proc Objeto do tipo process_t que será preenchido com os arquivos abertos
     */
    void collectOpenFiles(process_t& proc);

    /**
     * @brief Função que coleta os recursos IPC usados por um processo
     * @param proc Objeto do tipo process_t que será preenchido com os recursos IPC
     */
    void collectIPCResources(process_t& proc);

    /**
     * @brief Função que coleta recursos IPC do sistema relacionados a um processo
     * @param proc Objeto do tipo process_t que será preenchido com os recursos IPC do sistema
     */
    void collectSystemIPCResources(process_t& proc);

    /**
     * @brief Função que coleta estatísticas de E/S de um processo
     * @param proc Objeto do tipo process_t que será preenchido com as estatísticas de E/S
     */
    void collectIOStats(process_t& proc);

    /**
     * @brief Função que coleta os dados de sistema
     */
    void collectSystemInfo();

    /**
     * @brief Função que coleta os dados de CPU do sistema, do arquivo /proc/stat
     * @param stats Objeto do tipo system_stats_t que será preenchido com os dados de CPU do sistema
     */
    void collectCpuInfo(system_stats_t& stats);

    /**
     * @brief Função que coleta os dados de memória do sistema, do arquivo /proc/meminfo
     * @param stats Objeto do tipo system_stats_t que será preenchido com os dados de memória do sistema
     */
    void collectSystemMemoryInfo(system_stats_t& stats);

    // Novos métodos para sistema de arquivos
    /**
     * @brief Função que coleta informações do sistema de arquivos
     */
    void collectFilesystemInfo();

    /**
     * @brief Função que coleta informações de todas as partições do sistema
     * @param filesystemInfo Objeto que será preenchido com informações das partições
     */
    void collectPartitionInfo(filesystem_info_t& filesystemInfo);

    /**
     * @brief Função que coleta informações específicas de uma partição
     * @param device Dispositivo da partição
     * @param partition Objeto partition_info_t que será preenchido
     */
    void collectSinglePartitionInfo(const QString& device, partition_info_t& partition);

    /**
     * @brief Função que coleta o conteúdo de um diretório
     * @param path Caminho do diretório
     * @param filesystemInfo Objeto que será preenchido com o conteúdo do diretório
     */
    void collectDirectoryContents(const QString& path, filesystem_info_t& filesystemInfo);

    /**
     * @brief Função que coleta informações de um arquivo/diretório específico
     * @param path Caminho do arquivo/diretório
     * @param fileInfo Objeto file_info_t que será preenchido
     */
    void collectFileInfo(const QString& path, file_info_t& fileInfo);

    /**
     * @brief Função que converte permissões numéricas para string
     * @param mode Modo de permissão numérico
     * @return String com as permissões no formato rwxrwxrwx
     */
    QString convertPermissions(mode_t mode);

    /**
     * @brief Helper que verifica se o diretório é um diretório válido de processo
     * @param name Nome do diretório
     * @return true se o diretório é um diretório válido de processo, false caso contrário
     */
    bool isValidDirectory(const QString& name);

    bool running;
    QMutex mutex;
    QVector<process_t> proc_list;
    
    long long prev_total_cpu = 0;
    long long prev_used_cpu = 0;
    bool is_first_cpu_iteration = true;
    
    // Novos membros para sistema de arquivos
    filesystem_info_t currentFilesystemInfo;
    QString requestedDirectory;
    QString requestedPartition;
    bool directoryChangeRequested;
    bool partitionUpdateRequested;
};

#endif