#include <iostream>
#include <vector>
#include <string>
#include <QString>

#define PROC_PATH "/proc"

typedef struct {
    std::string name;
    std::string state;
    int pid;
    int ppid; //ID do processo pai
    int cpu;
    long long total_memory;
    long long ram_memory;
    int status;
} process_t;

class Model{
private:
    std::vector<process_t> processes;
public:
    Model();
    ~Model();
    
    void mainThread();
    void readProc();
    void parseProcStat(process_t& proc, QString content);

    inline std::vector<process_t>* getProcesses() {return &(this->processes);}
};