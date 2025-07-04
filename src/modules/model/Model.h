/**
 * @file Model.h
 * @brief Header para a classe Model
 * @author Pedro Neves
 */


#ifndef MODEL_H
#define MODEL_H

#include "model_common.h"
#include "threads/Collector.h"
#include "threads/Processor.h"
#include <QAbstractTableModel>

/**
 * @brief Classe que implementa o modelo de dados utilizado no QML
 * Utiliza duas threads para coletar e processar os dados
 */
class Model : public QAbstractTableModel {
    Q_OBJECT
    
    //Propriedades referentes aos dados de sistemas expostas para o QML
    Q_PROPERTY(double systemCpuUsage READ systemCpuUsage NOTIFY systemStatsChanged)
    Q_PROPERTY(double systemMemoryUsage READ systemMemoryUsage NOTIFY systemStatsChanged)
    Q_PROPERTY(int totalProcesses READ totalProcesses NOTIFY systemStatsChanged)
    Q_PROPERTY(int runningProcesses READ runningProcesses NOTIFY systemStatsChanged)
    Q_PROPERTY(int sleepingProcesses READ sleepingProcesses NOTIFY systemStatsChanged)
    Q_PROPERTY(int idleProcesses READ idleProcesses NOTIFY systemStatsChanged)
    Q_PROPERTY(int totalThreads READ totalThreads NOTIFY systemStatsChanged)
    Q_PROPERTY(QString totalMemoryMB READ totalMemoryMB NOTIFY systemStatsChanged)
    Q_PROPERTY(QString availableMemoryMB READ availableMemoryMB NOTIFY systemStatsChanged)
    
    // Novas propriedades para sistema de arquivos
    Q_PROPERTY(QString currentDirectory READ currentDirectory NOTIFY filesystemChanged)
    Q_PROPERTY(QVariantList partitions READ partitions NOTIFY filesystemChanged)
    Q_PROPERTY(QVariantList directoryContents READ directoryContents NOTIFY filesystemChanged)
    Q_PROPERTY(int selectedProcessPid READ selectedProcessPid WRITE setSelectedProcessPid NOTIFY selectedProcessChanged)

public:

    /**
     * @brief Enum de roles para os dados do modelo
     * Esses roles são utilizados pelo Qt para acessar os dados do modelo no arquivo QML
     */
    enum Roles {
        PidRole = Qt::UserRole + 1,
        NameRole,
        CpuUsageRole,
        MemoryUsageRole,
        StateRole,
        UsernameRole,
        ThreadsRole,
        OpenFilesCountRole,
        ReadBytesRole,
        WriteBytesRole
    };

    Model(QObject *parent = nullptr);
    ~Model();

    /**
     * @brief Inicia o monitoramento de dados do sistema operacional
     * inciando as threads de coleta e processamento
     */
    void startMonitoring();

    /**
     * @brief Interrompe o monitoramento encerrando corretament as threads
     */
    void stopMonitoring();

    /**
     * @brief Retorna o número de linhas do modelo de processos
     * Essa função é utilizada pelo View para exibir as linhas no QML
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Retorna o número de colunas do modelo de processos
     * Essa função é utilizada pelo View para exibir as colunas no QML
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Retorna os dados do modelo de processos
     * Essa função é utilizada pelo View para exibir os dados no QML
    */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Retorna os nomes dos roles do modelo de processos
     * Essa função é utilizada para facilitar a associação dos dados com os roles no QML
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Retorna o uso da CPU do sistema em porcentagem 
     */
    double systemCpuUsage() const { return currentStats.system_cpu_usage; }
    
    /**
     * @brief Retorna o uso da memória do sistema em porcentagem 
     */
    double systemMemoryUsage() const { return currentStats.system_memory_usage; }

    /**
     * @brief Retorna o número total de processos
     */
    int totalProcesses() const { return currentStats.total_processes; }

    /**
     * @brief Retorna o número de processos em execução em um certo momento no processador
     */
    int runningProcesses() const { return currentStats.running_processes; }

    /**
     * @brief Retorna o número de processos em estado de espera
     */
    int sleepingProcesses() const { return currentStats.sleeping_processes; }

    /**
     * @brief Retorna o número de processos em estado de inatividade
     */
    int idleProcesses() const { return currentStats.idle_processes; }

    /**
     * @brief Retorna o número total de threads de todos os processos
     */
    int totalThreads() const { return currentStats.total_threads; }

    /**
     * @brief Retorna a quantidade total de memória física do sistema em MB
     */
    QString totalMemoryMB() const { return QString::number(currentStats.total_physical_memory / (1024*1024)); }

    /**
     * @brief Retorna a quantidade de memória física disponível (não necessariamente livre) para uso em MB
     */
    QString availableMemoryMB() const { return QString::number(currentStats.available_physical_memory / (1024*1024)); }

    // Novos métodos para sistema de arquivos
    /**
     * @brief Retorna o diretório atual sendo navegado
     */
    QString currentDirectory() const { return currentStats.filesystem_info.current_directory; }

    /**
     * @brief Retorna a lista de partições do sistema
     */
    QVariantList partitions() const;

    /**
     * @brief Retorna o conteúdo do diretório atual
     */
    QVariantList directoryContents() const;

    /**
     * @brief Retorna o PID do processo selecionado para detalhes
     */
    int selectedProcessPid() const { return m_selectedProcessPid; }

    /**
     * @brief Define o PID do processo selecionado para detalhes
     */
    void setSelectedProcessPid(int pid);

    /**
     * @brief Retorna informações detalhadas de E/S do processo selecionado
     */
    Q_INVOKABLE QVariantMap getProcessIODetails(int pid) const;

    /**
     * @brief Navega para um diretório específico
     */
    Q_INVOKABLE void navigateToDirectory(const QString& path);

    /**
     * @brief Navega para o diretório pai
     */
    Q_INVOKABLE void navigateToParent();

    /**
     * @brief Navega para o diretório home
     */
    Q_INVOKABLE void navigateToHome();

    /**
     * @brief Navega para o diretório raiz
     */
    Q_INVOKABLE void navigateToRoot();

signals:
    /**
     * @brief Sinal emitido quando os dados de sistema são atualizados
     * @details Esse sinal é conectado ao View para atualizar o overview do sistema no QML
     */
    void systemStatsChanged();

    /**
     * @brief Sinal emitido quando os dados do sistema de arquivos são atualizados
     */
    void filesystemChanged();

    /**
     * @brief Sinal emitido quando o processo selecionado é alterado
     */
    void selectedProcessChanged();

    // Sinais internos para comunicação com Collector
    /**
     * @brief Sinal interno para solicitar navegação de diretório
     */
    void requestDirectoryNavigation(const QString& path);

private slots:

    /**
     * @brief Callback para atualizar os dados de processos no Model
     * @details Essa callback é chamada apos a thread de processamento terminar de processar os dados de processos
     * @param processedData Dados de processos atualizados
    */
    void handleProcessedData(const QVector<process_t>& processedData);

    /**
     * @brief Callback para atualizar os dados de sistema no Model
     * @details Essa callback é chamada apos a thread de coleta terminar de coletar os dados de sistema
     * @param stats Dados de sistema atualizados
     */
    void handleSystemStatsUpdated(const system_stats_t& stats);

    /**
     * @brief Callback para atualizar os dados do sistema de arquivos no Model
     * @details Essa callback é chamada quando a thread de coleta termina de coletar dados do filesystem
     * @param filesystemInfo Dados do sistema de arquivos atualizados
     */
    void handleFilesystemUpdated(const filesystem_info_t& filesystemInfo);

private:
    Collector* collector;
    Processor* processor;
    QVector<process_t> currentProcesses;
    system_stats_t currentStats;
    int m_selectedProcessPid;
};

#endif