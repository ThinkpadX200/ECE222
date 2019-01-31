
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>	/* for rand() function, and RAND_MAX */
#include <time.h>

int main()
{
    int	row,col,i;
    int	monst_row[4],monst_col[4];
    char	ch,last_ch;
    int	loop_counter,collide,buffer_flushed;
    int	powerup;

    initscr();		/* initialize the curses library */
    nodelay(stdscr,TRUE);	/* non-blocking */
    noecho();		/* turn off echoing */
    clear();		/* clear screen */

    srand(time(NULL));	/* init rand# generator with time */

    row=LINES/2;		/* start in middle of screen */
    col=COLS/2;
    monst_row[0]=1;
    monst_col[0]=1;			/* init to top left */
    monst_row[1]=LINES-2;
    monst_col[1]=1;		/* init to bottom left */
    monst_row[2]=LINES-2;
    monst_col[2]=COLS-2;	/* init to bottom right */
    monst_row[3]=1;
    monst_col[3]=COLS-2;		/* init to top right */
    powerup=0;

    loop_counter=0;
    while (1) {
        /* erase graphic */
        move (row,col);
        addstr(" ");

        /* move graphic */
        if (ch == 'q') break;		/* end game */
        if (ch == ' ') powerup=(powerup+1)%2;
        if (ch == 'w') row--;
        if (ch == 'a') col--;
        if (ch == 's') row++;
        if (ch == 'd') col++;
        if (row < 0) row=0;
        if (row >= LINES) row=LINES-1;
        if (col < 0) col=0;
        if (col >= COLS) col=COLS-1;

        /* draw graphic */
        move (row,col);
        addstr("M");

        if (loop_counter % 4 == 0) {
            /* erase monsters */
            for (i=0; i<4; i++) {
                move (monst_row[i],monst_col[i]);
                addstr(" ");
            }

            /* move monsters */
            for (i=0; i<4; i++) {
                if (powerup == 0) {
                    /* move towards the player */
                    if (monst_row[i] < row) monst_row[i]++;
                    if (monst_row[i] > row) monst_row[i]--;
                    if (monst_col[i] < col) monst_col[i]++;
                    if (monst_col[i] > col) monst_col[i]--;
                } else {
                    /* run away from the player */
                    if (monst_row[i] < row) monst_row[i]--;
                    if (monst_row[i] > row) monst_row[i]++;
                    if (monst_col[i] < col) monst_col[i]--;
                    if (monst_col[i] > col) monst_col[i]++;
                }
                if ((double)rand() > (double)RAND_MAX*0.8) {
                    if ((double)rand() > (double)RAND_MAX*0.5) monst_col[i]--;
                    else monst_col[i]++;
                    if ((double)rand() > (double)RAND_MAX*0.5) monst_row[i]--;
                    else monst_row[i]++;
                }
                if (monst_row[i] < 0) monst_row[i]=0;
                if (monst_row[i] > LINES-1) monst_row[i]=LINES-1;
                if (monst_col[i] < 0) monst_col[i]=0;
                if (monst_col[i] > COLS-1) monst_col[i]=COLS-1;
            }

            /* draw monsters */
            for (i=0; i<4; i++) {
                move (monst_row[i],monst_col[i]);
                if (powerup == 0) addstr("X");
                else addstr("O");
            }
        }

        /* check for game over */
        collide=0;
        for (i=0; i<4; i++)
            if (monst_row[i] == row  &&  monst_col[i] == col) {
                collide=1;
                break;
            }
        if (collide == 1) {
            move (LINES/2,COLS/2-5);
            addstr("GAME OVER!");
            refresh();	/* must flush the buffer to see! */
            sleep(1);
            break;	/* quit game */
        }

        move (LINES-1,0);	/* keep cursor out of the way (bottom left) */

        refresh();
        usleep(50000);

        /* flush the input buffer */
        buffer_flushed=0;
        last_ch=-1;		/* if buffer empty, we get this output */
        while (buffer_flushed == 0) {
            ch=getch();		/* poll for input */
            if (ch == -1) {
                ch=last_ch;	/* "output" last character read */
                buffer_flushed=1;
            }
            last_ch=ch;		/* store up the last character polled */
        }

        loop_counter++;
    }
    getch();
    endwin();
    return 0;
}
