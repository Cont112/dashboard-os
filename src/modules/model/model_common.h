/**
 * @file model_common.h
 * @brief Header para as structs utilizadas no Model e nas threads
 * @author Pedro Neves
 */

#ifndef MODEL_COMMON_H
#define MODEL_COMMON_H

#include <QDateTime>
#include <QString>
#include <QList>
#include <QMap>
#include <QVector>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <iostream>
#include <QTimer>

#define PROC_PATH "/proc"
#define UPDATE_INTERVAL 3000 // 3 segundos

/**
 * @brief Struct que guarda informações de uma thread de um processo
 * @details Essa struct é utilizada no lugar de um ponteiro para outro process_t para simplificar o código
 * Os dados dessa struct são coletados de /proc/[pid]/task/[tid]/stat
 */
typedef struct {
    int tid;                    ///< ID da thread (Thread ID)
    QString state;             ///< Estado atual da thread (R, S, D, Z, T, etc.)
    int cpu;                   ///< Núcleo de CPU onde a thread está executando
    long long start_time;      ///< Tempo de início da thread em ticks
    double cpu_percentage;     ///< Percentual de uso de CPU da thread
} thread_info_t;

/**
 * @brief Struct que guarda informações de memória de um processo
 * @details Os dados dessa struct são coletados de /proc/[pid]/statm
 */
typedef struct {
    long long total_memory;    ///< Tamanho total da memória virtual em páginas
    long long ram_memory;      ///< Tamanho da memória RAM residente em páginas
    long long shared_memory;   ///< Número de páginas de memória compartilhada
    long long code_pages;      ///< Número de páginas de código (text)
    long long data_pages;      ///< Número de páginas de dados/stack
    double memory_percentage;  ///< Percentual de uso de memória do processo
} memory_info_t;

/**
 * @brief Struct que guarda informações de um arquivo aberto por um processo
 * @details Os dados dessa struct são coletados de /proc/[pid]/fd/
 */
typedef struct {
    int fd;                        ///< File descriptor number
    QString path;                  ///< Caminho do arquivo/dispositivo
    QString type;                  ///< Tipo (arquivo, socket, pipe, etc.)
    QString permissions;           ///< Permissões de acesso
    long long size;               ///< Tamanho do arquivo (se aplicável)
} open_file_t;

/**
 * @brief Struct que guarda informações de um semáforo/mutex usado por um processo
 * @details Os dados dessa struct são coletados de /proc/[pid]/maps e /proc/sysvipc/
 */
typedef struct {
    QString type;                  ///< Tipo (semáforo, mutex, shared memory, etc.)
    QString key;                   ///< Chave do recurso IPC
    QString permissions;           ///< Permissões do recurso
    int owner_pid;                ///< PID do processo proprietário
} ipc_resource_t;

/**
 * @brief Struct que guarda informações detalhadas de E/S de um processo
 * @details Coleta informações de arquivos abertos, sockets, pipes, etc.
 */
typedef struct {
    QList<open_file_t> open_files;     ///< Lista de arquivos abertos
    QList<ipc_resource_t> ipc_resources; ///< Lista de recursos IPC
    long long read_bytes;              ///< Bytes lidos pelo processo
    long long write_bytes;             ///< Bytes escritos pelo processo
    long long read_syscalls;           ///< Número de syscalls de leitura
    long long write_syscalls;          ///< Número de syscalls de escrita
} process_io_t;

/**
 * @brief Struct que guarda informações de uma partição/filesystem
 * @details Os dados dessa struct são coletados de /proc/mounts e /proc/partitions
 */
typedef struct {
    QString device;                ///< Dispositivo (ex: /dev/sda1)
    QString mountpoint;            ///< Ponto de montagem (ex: /)
    QString filesystem;            ///< Tipo de filesystem (ex: ext4)
    QString options;               ///< Opções de montagem
    long long total_space;         ///< Espaço total em bytes
    long long used_space;          ///< Espaço usado em bytes
    long long available_space;     ///< Espaço disponível em bytes
    double usage_percentage;       ///< Percentual de uso
} partition_info_t;

/**
 * @brief Struct que guarda informações de um arquivo/diretório
 * @details Usada para navegação no sistema de arquivos
 */
typedef struct {
    QString name;                  ///< Nome do arquivo/diretório
    QString path;                  ///< Caminho completo
    QString type;                  ///< Tipo (arquivo, diretório, link, etc.)
    QString permissions;           ///< Permissões no formato rwxrwxrwx
    QString owner;                 ///< Proprietário
    QString group;                 ///< Grupo
    long long size;               ///< Tamanho em bytes
    QDateTime modified_time;       ///< Data de modificação
    bool is_directory;            ///< Se é diretório
    bool is_hidden;               ///< Se é arquivo oculto
} file_info_t;

/**
 * @brief Struct que guarda informações do sistema de arquivos
 * @details Agrupa informações de partições e sistema de arquivos
 */
typedef struct {
    QList<partition_info_t> partitions;  ///< Lista de partições
    QString current_directory;           ///< Diretório atual sendo navegado
    QList<file_info_t> directory_contents; ///< Conteúdo do diretório atual
} filesystem_info_t;

/**
 * @brief Struct que guarda informações relevantes de um processo
 * @details A maioria dos dados dessa struct são coletados de /proc/[pid]/stat e /proc/[pid]/status
 */
typedef struct {
    QString name;                       ///< Nome do processo
    QString state;                      ///< Estado do processo (R, S, D, Z, T, etc.)
    int pid;                            ///< ID do processo (Process ID)
    int ppid;                           ///< ID do processo pai (Parent Process ID)
    int cpu;                            ///< Núcleo de CPU onde o processo está executando
    QString username;                   ///< Nome do usuário proprietário do processo
    int status;                         ///< Status do processo (código numérico)
    long long start_time;               ///< Tempo de início do processo em jiffies
    long long utime;                    ///< Tempo em modo usuário em clock ticks
    long long stime;                    ///< Tempo em modo kernel em clock ticks
    double cpu_percentage;              ///< Percentual de uso de CPU do processo
    bool processed;                     ///< Flag indicando se o processo foi processado
    QList<thread_info_t> threads;       ///< Lista de threads do processo
    memory_info_t memory_details;       ///< Detalhes de uso de memória do processo
    process_io_t io_details;            ///< Detalhes de E/S do processo (NOVO)
} process_t;

/**
 * @brief Struct que guarda informações gerais de CPU e memória do sistema
 * @details Os dados brutos dessa struct são coletados de /proc/stat e /proc/meminfo e depois são processados pelo Processor
 */
typedef struct {
    int total_processes;                 ///< Número total de processos no sistema
    int running_processes;               ///< Número de processos em execução
    int sleeping_processes;              ///< Número de processos dormindo/esperando
    int stopped_processes;               ///< Número de processos parados
    int zombie_processes;                ///< Número de processos zombie
    int idle_processes;                  ///< Número de processos ociosos
    int total_threads;                   ///< Número total de threads no sistema
    double system_cpu_usage;             ///< Percentual de uso total da CPU do sistema
    double cpu_idle_percentage;          ///< Percentual de tempo ocioso da CPU
    double system_memory_usage;          ///< Percentual de uso de memória do sistema
    long long total_physical_memory;     ///< Quantidade total de memória física em bytes
    long long available_physical_memory; ///< Quantidade de memória física disponível em bytes
    filesystem_info_t filesystem_info;   ///< Informações do sistema de arquivos (NOVO)
} system_stats_t;

#endif
