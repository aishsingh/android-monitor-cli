#ifndef PROCESS_H
#define PROCESS_H

typedef struct process {
    int mem_total;
    int mem_swap;
} process;

process fetchData(char *package);

#endif /* PROCESS_H */
