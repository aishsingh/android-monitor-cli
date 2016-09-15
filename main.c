#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define COLOR_BLUE_NEW 20
#define COLOR_RED_NEW 21
#define VERSION "Pre-alpha"

typedef struct process {
    int mem_total;
    int mem_swap;
} process;

void printUsage() {
    printf("Android Monitor CLI version %s\n", VERSION);
    printf("\n");
    printf("Usage:\n");
    printf("  android-monitor-cli <package>\n");
    printf("\n");
    printf("2016 Aishwarya Singh\n");
    printf("Released under the MIT licence\n");
}

bool handleInput() {
    int ch = getch();
    return !(ch == 'q' || ch == 27);   // KEY_ESCAPE = 27
}

process fetchData(char *package) {
    process data;

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
        if ((s = strstr(buf, "TOTAL:")) != NULL) {
            sscanf(s+6,"%d %*s %*s %*s %d", &data.mem_total, &data.mem_swap);
            break;
        }
    }

    if(pclose(fp))  {
        // Command exited with error status
        printf("Please ensure the android-sdk is installed and adb can detect your device.");
        printf("\n");
        printf("Use --help flag for usage information");
        exit(EXIT_FAILURE);
    }

    free(cmd);
    return data;
}

int main(int argc, char **argv)
{
    if ((argc == 1) || (strcmp(argv[1], "--help") == 0)) {
        printUsage();
        return -1;
    }

    // Fetch data through adb
    process result = fetchData(argv[1]);
    printf("MEMORY:\nTotal = %d KB\nSwap = %d KB\n", result.mem_total, result.mem_swap);

    bool running = true;
    while (running) {
        initscr();          /* Start curses mode        */
        raw();              /* Line buffering disabled  */
        noecho();           /* Don't echo() while we do getch */

        start_color();
        use_default_colors();  // Enable transparency
        init_color(COLOR_BLUE_NEW, 535, 727, 875);
        init_color(COLOR_RED_NEW, 875, 340, 309);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);     // Normal colour
        init_pair(2, COLOR_BLUE_NEW, COLOR_BLACK);  // Memory colour
        init_pair(3, COLOR_RED_NEW, COLOR_BLACK);   // CPU colour

        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvprintw(0, 0, "Memory\n");
        attron(COLOR_PAIR(3));
        mvprintw(10, 0, "CPU\n");
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);

        int win_w, win_h;
        getmaxyx(stdscr, win_h, win_w);

        attron(COLOR_PAIR(2));
        for (int j = 0;  j < win_w;  ++j)
            mvaddch (9, j, ACS_HLINE);
        attron(COLOR_PAIR(3));
        for (int j = 0;  j < win_w;  ++j)
            mvaddch (19, j, ACS_HLINE);
        attroff(COLOR_PAIR(3));

        running = handleInput();

        refresh();
    }

    endwin();
    return 0;
}
