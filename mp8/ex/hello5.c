
/* Demonstrate "moving" text - draw and then erase */

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
        addstr("Hello world");
        refresh();
        usleep(100000);
        move (i,i+1);	/* absolute position, not relative */
        addstr("           ");	/* draw empty space */
    }

    getch();	/* wait for user input */
    endwin();	/* turn off curses */
    return 0;
}
