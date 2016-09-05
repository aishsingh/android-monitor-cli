#include <ncurses.h>

#define COLOR_BLUE_NEW 8
#define COLOR_RED_NEW 9


bool handleInput() {
    int ch = getch();
    return !(ch == 'q' || ch == 27);   // KEY_ESCAPE = 27
}

int main()
{
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
