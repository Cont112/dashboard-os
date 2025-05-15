#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/cpuinfo");
        return 1;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
    }

    free(line);
    fclose(fp);
    return 0;
}
