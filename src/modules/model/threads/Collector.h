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
};

#endif