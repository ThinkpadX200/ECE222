
/* Demonstrate doing blocks of code at different rates within a
** timed loop, using the modulus operator on the iteration count. */

#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()
{
    int	i;

    initscr();
    cbreak();
    noecho();

    for (i=0; i<20; i++) {
        move(i,i+1);
        addstr("Hello!  ");
        if (i%3 == 0) {
            move(i,i+1);
            addstr("       ");
            addstr("Goodbye!");
        }
        refresh();
        usleep(100000);
    }

    getch();
    endwin();
    return 0;
}
