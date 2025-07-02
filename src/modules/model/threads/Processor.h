/**
 * @file Processor.h
 * @brief Header para a thread Processor
 * @author Pedro Neves
 */
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../model_common.h"
#include <QWaitCondition>

/**
 * @brief Classe que implementa a thread de processamento de dados
 * @details Essa classe é responsável por processar os dados coletados pela thread Collector
 */
class Processor : public QThread {
    Q_OBJECT
public:
    Processor(QObject *parent = nullptr);
    ~Processor();

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
     * @brief Sinal emitido quando termina de processar todos os dados de processos
     * @details Esse sinal é emitido pela thread diretamente ao Model
     * @param processedData Vetor de processos processados
     */
    void dataProcessed(const QVector<process_t>& processedData);

    /**
     * @brief Sinal emitido quando termina de processar todos os dados de sistema
     * @details Esse sinal é emitido pela thread diretamente ao Model
     * @param stats Struct com os dados de sistema processados
     */
    void systemStatsUpdated(const system_stats_t& stats);

    /**
     * @brief Sinal emitido quando termina de processar os dados do sistema de arquivos
     * @details Esse sinal é emitido pela thread diretamente ao Model
     * @param filesystemInfo Struct com os dados do sistema de arquivos processados
     */
    void filesystemUpdated(const filesystem_info_t& filesystemInfo);

public slots:
    /**
     * @brief Callback que recebe os dados de processos coletados pela thread Collector
     * @details Adiciona os dados recebidos na fila de dados local
     * @param processes Vetor de processos coletados
     */
    void onDataCollected(const QVector<process_t>& processes);
    
    /**
     * @brief Callback que recebe os dados de sistema coletados pela thread Collector
     * @details Adiciona os os dados de sistema recebidos na struct currentStats
     * @param stats Struct com os dados de sistema coletados
     */
    void onSystemInfoCollected(const system_stats_t& stats);

    /**
     * @brief Callback que recebe os dados do sistema de arquivos coletados pela thread Collector
     * @details Adiciona os dados do sistema de arquivos recebidos na struct currentFilesystemInfo
     * @param filesystemInfo Struct com os dados do sistema de arquivos coletados
     */
    void onFilesystemInfoCollected(const filesystem_info_t& filesystemInfo);

private:

    /**
     * @brief Função que processa os dados de processos
     * @details Essa função é responsável por processar os dados de processos recebidos
     */
    void processData();

    /**
     * @brief Função que atualiza os dados do sistema com os estados dos processos recebidos
     * @details Ela conta a quantidade de processos em cada estado e adiciona ao struct currentStats
     */
    void updateProcessStates();

    /**
     * @brief Função que calcula as porcentagens de uso de CPU de cada processo
     * @details Calcula a porcentagem de CPU baseada no tempo de CPU usado entre iterações
     */
    void calculateProcessPercentages();

    // Novos métodos para processamento de dados do sistema de arquivos
    /**
     * @brief Função que processa os dados do sistema de arquivos
     * @details Calcula estatísticas de uso de espaço em disco e processa informações de arquivos
     */
    void processFilesystemData();

    /**
     * @brief Função que calcula estatísticas de uso de partições
     * @details Calcula percentuais de uso baseado no espaço total e disponível
     */
    void calculatePartitionUsage();

    /**
     * @brief Função que ordena conteúdo de diretórios
     * @details Organiza arquivos e diretórios em ordem apropriada
     */
    void sortDirectoryContents();

    /**
     * @brief Função que processa estatísticas de E/S dos processos
     * @details Calcula métricas de desempenho de E/S por processo
     */
    void processIOStatistics();

    bool running;
    QMutex mutex;
    QWaitCondition wakeCondition;
    QVector<process_t> dataQueue;
    QVector<process_t> processedData;

    system_stats_t currentStats;
    filesystem_info_t currentFilesystemInfo;
    
    QMap<int, long long> previousCpuTime;
    
    // Novos membros para processamento de E/S
    QMap<int, long long> previousReadBytes;
    QMap<int, long long> previousWriteBytes;
    bool filesystemUpdatePending;

};

#endif 