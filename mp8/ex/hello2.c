
/* Demonstrate changing buffering, echoing, and blocking */

#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()

{
    int	i;

    initscr();		/* turn on curses */

    nocbreak();		/* uncomment to turn on line buffering */
    /* by default keyboard input is unbuffered */

//noecho();		/* uncomment to turn off echoing */
    /* by default keyboard input is echoed */

    nodelay(stdscr,TRUE);	/* uncomment to turn off blocking */
//halfdelay(5);		/* uncomment for 5/10 second blocking */
    /* by default keyboard input is indefinitely blocked */

    clear();	/* clear screen */
    move(10,20);	/* row 10, column 20 */
    addstr("Hello world");	/* add a string */
    move(LINES-1,0);	/* move to LL */

    refresh();	/* update the screen */
    for (i=0; i<5; i++) {
        getch();	/* wait for user input */
        usleep(500000);	/* uncomment this line for nodelay() (no blocking) */
    }

    endwin();	/* turn off curses */
    return 0;
}


