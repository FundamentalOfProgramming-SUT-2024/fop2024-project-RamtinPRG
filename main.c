#include <ncurses.h>
#include <panel.h>

int main()
{
    // Initializing
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    endwin();
    return 0;
}