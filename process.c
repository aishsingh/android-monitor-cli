#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "process.h"

Process fetchData(char *package) {
    Process data = {0};

    // Get Memory data
    FILE *fp;
    char buf[128];
    char *cmd_adb = (char *) "adb shell dumpsys meminfo ";
    char *cmd = (char *) malloc(1 + strlen(cmd_adb) + strlen(package));
    strcpy(cmd, cmd_adb);
    strcat(cmd, package);

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buf, 128, fp) != NULL) {
        // Print ADB Output
        // printf("OUTPUT: %s", buf);

        char *s;
        if ((s = strstr(buf, "TOTAL")) != NULL) {
            sscanf(s+6,"%*d %*d %*d %*d %d %d %d", &data.mem_total, &data.mem_alloc, &data.mem_free);
            break;
        }
    }

    if(pclose(fp))  {
        // Command exited with error status
        printf("Please ensure the android-sdk is installed and adb can detect your device.\n");
        printf("\n");
        printf("Use --help flag for usage information");
        exit(EXIT_FAILURE);
    }

    // Get CPU data
    // TODO

    free(cmd);
    return data;
}
