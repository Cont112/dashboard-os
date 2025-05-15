#include <stdio.h>
#include <string>

const char* proc_dir = "/proc/";
const char* relevant_files[] = {
    "cpuinfo",
    "meminfo",
    "version",
    "uptime",
    "stat",
};

int main() {
    for(const char* file : relevant_files) {
        std::string path = std::string(proc_dir) + file;
        FILE* f = fopen(path.c_str(), "r");
        if(f == NULL) {
            printf("couldn't open file %s\n", file);
            continue;
        }
        printf("opened file %s\n", file);
        for(int i = 0; i < 100; i++) {
            printf("=");
        }
        printf("\n");

        char buffer[1024];
        while(fgets(buffer, sizeof(buffer), f) != NULL) {
            printf("%s", buffer);
        }
        fclose(f);
    }
}