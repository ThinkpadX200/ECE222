
/* Demonstrate moving an icon around the screen while "background"
** action continues - in this case, the icon drifts right. */

#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()

{
    int	r,c;
    char	ch;

    initscr();	/* turn on curses */
    clear();	/* clear screen */
    noecho();	/* turn off echo */
    cbreak();	/* turn off line buffering */
    halfdelay(2);	/* wait 2/10 seconds for char to appear in buffer */
    r=LINES/2;
    c=COLS/2;

    while (1) {
        move (r,c);
        addstr("M");
        move (LINES-1,0);
        refresh();
//		usleep(100000);
        ch=getch();	/* wait for user input */
        move (r,c);	/* absolute position, not relative */
        addstr(" ");	/* draw empty space */
        if (ch == ERR)
            c++;	/* background action */
        if (ch == 'q')
            break;
        if (ch == 'w')
            r--;
        if (ch == 'a')
            c--;
        if (ch == 's')
            r++;
        if (ch == 'd')
            c++;
        if (r >= LINES)
            r=LINES-1;
        if (r < 0)
            r=0;
        if (c < 0)
            c=0;
        if (c >= COLS)
            c=COLS-1;
    }

    endwin();	/* turn off curses */
    return 0;
}
