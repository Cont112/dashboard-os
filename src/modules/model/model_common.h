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
#define UPDATE_INTERVAL 5000

typedef struct {
    int tid;
    QString state;
    int cpu;
    long long start_time;
    double cpu_percentage;
} thread_info_t;

typedef struct {
    long long total_memory;
    long long ram_memory;
    long long virtual_memory;
    long long shared_memory;
    long long code_pages;
    long long heap_pages;
    long long stack_pages;
    long long data_pages;
    double memory_percentage;
} memory_info_t;

typedef struct {
    QString name;
    QString state;
    int pid;
    int ppid; 
    int cpu;
    QString username;
    int status;
    long long start_time;
    double cpu_percentage;
    int child_count;
    bool processed;
    QList<thread_info_t> threads;
    memory_info_t memory_details;
} process_t;

typedef struct {
    int total_processes;
    int running_processes;
    int sleeping_processes;
    int stopped_processes;
    int zombie_processes;
    int total_threads;
    double system_cpu_usage;
    double cpu_idle_percentage;
    double system_memory_usage;
    long long total_physical_memory;
    long long free_physical_memory;
    long long total_virtual_memory;
    long long free_virtual_memory;
    QMap<int, QList<int>> process_tree;
} system_stats_t;

#endif
