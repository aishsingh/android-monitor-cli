#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process {
    int mem_total;
    int mem_alloc;
    int mem_free;
    int cpu_usage;
} Process;

Process fetchData(char *package);

#endif /* PROCESS_H */
