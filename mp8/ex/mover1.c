
/* Demonstrate moving an icon around the screen. */

#include <stdio.h>
#include <curses.h>

int main()
{
    int	r,c;
    char	ch;

    initscr();	/* turn on curses */
    clear();	/* clear screen */
    noecho();	/* turn off echoing */
    nocbreak();	/* turn on line buffering */
    halfdelay(2);	/* turn off blocking after 2/10 seconds */
    r=LINES/2;
    c=COLS/2;

    while (1) {
        move (r,c);
        addstr("M");
        move (LINES-1,0);
        refresh();
        ch=getch();
        move (r,c);
        addstr(" ");
//		if (ch == ERR)
//			c=c+1;
        if (ch == 'i')
            r=r-1;
        if (ch == 'j')
            c=c-1;
        if (ch == 'k')
            c=c+1;
        if (ch == 'm')
            r=r+1;
        if (ch == 'q')
            break;
        if (r < 0)
            r=0;
        if (r >= LINES)
            r=LINES-1;
        if (c < 0)
            c=0;
        if (c >= COLS)
            c=COLS-1;
    }

    endwin();	/* turn off curses */
    return 0;
}
