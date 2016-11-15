#include <ncurses.h>
#include <stdio.h>
#include <memory.h>

#include "process.h"

#define COLOR_BLUE_NEW 20
#define COLOR_RED_NEW 21
#define VERSION "Pre-alpha"

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

int main(int argc, char **argv)
{
    if ((argc == 1) || (strcmp(argv[1], "--help") == 0)) {
        printUsage();
        return -1;
    }

    initscr();          /* Start curses mode        */
    raw();              /* Line buffering disabled  */
    noecho();           /* Don't echo() while we do getch */
    curs_set(0);
    timeout(1000);

    start_color();
    use_default_colors();  // Enable transparency
    init_color(COLOR_BLUE_NEW, 535, 727, 875);
    init_color(COLOR_RED_NEW, 875, 340, 309);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);     // Normal colour
    init_pair(2, COLOR_BLUE_NEW, COLOR_BLACK);  // Memory colour
    init_pair(3, COLOR_RED_NEW, COLOR_BLACK);   // CPU colour

    int win_w, win_h;
    getmaxyx(stdscr, win_h, win_w);

    Process data = {0};

    bool running = true;
    int elapsed_time = 0;
    while (running) {
        // Setup headings
        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvprintw(0, 0, "Memory: %d KB\n", data.mem_alloc);
        attron(COLOR_PAIR(3));
        mvprintw(10, 0, "CPU: %d \%\n", data.cpu_usage);
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
        mvprintw(21, win_w-20, "Elapsed time: %d s", elapsed_time);

        // Setup lines
        attron(COLOR_PAIR(2));
        for (int j = 0;  j < win_w;  ++j)
            mvaddch (9, j, ACS_HLINE);
        attron(COLOR_PAIR(3));
        for (int j = 0;  j < win_w;  ++j)
            mvaddch (19, j, ACS_HLINE);
        attroff(COLOR_PAIR(3));

        running = handleInput();

        // Refresh
        data = fetchData(argv[1]); // Fetch data through adb
        refresh();
        elapsed_time++;
    }
    endwin();

    printf("MEMORY: Alloc = %d KB, Free = %d KB\n", data.mem_alloc, data.mem_free);
    printf("CPU: Usage = %d \%\n", data.cpu_usage);

    return 0;
}
