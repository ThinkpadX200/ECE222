
/* Bring time into play - add text over time */

#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()

{
    int	i;

    initscr();	/* turn on curses */
    clear();	/* clear screen */

    for (i=0; i<LINES; i++) {
        move (i,i+1);
        if (i%2 == 1)
            standout();
        addstr("Hello world");
        if (i%2 == 1)
            standend();
        sleep(1);
        refresh();
    }

    getch();	/* wait for user input */
    endwin();	/* turn off curses */
    return 0;
}
