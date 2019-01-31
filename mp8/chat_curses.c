/* chat_curses.c
 * an example using curses to print to top and bottom of screen
 *
 * If the terminal is messed up type
 *     stty sane
 * to reset terminal settings
 *
 * remember to include -lncurses with gcc to link in the curses library
 *
 * If the library is not found then install 
 *    libncurses5-dev
 *
 * Use the Ubuntu Software Center to install this package
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

#define MAXBUFLEN 80

const char *welcome = "Welcome to ECE 222 chat";
const char *remote_name = "apolloXX";

int main(void)
{
    char buf[MAXBUFLEN];
    char buf_count[MAXBUFLEN];
    int i;
    int screen_middle;
    char *screen_divider, *addr;
    int not_done = 1;
    int count = 0;
    int ch;

    initscr();  // Initialize Curses System.  Must be called first to set 
    // global variables such as COLS and LINES

    screen_divider = (char *) malloc((COLS+1) * sizeof(char));
    screen_middle = LINES/2;   // 0 is top of screen

    memset(screen_divider, ' ', COLS);  // fill with spaces
    if (COLS < 5) {
        fprintf(stderr, "Screen is too small %d\n", COLS);
    } else if (COLS < strlen(welcome) + strlen(remote_name) + 1) {
        sprintf(screen_divider, "chat");
    } else {
        memcpy(screen_divider, welcome, strlen(welcome));
        addr = screen_divider + COLS - strlen(remote_name);
        memcpy(addr, remote_name, strlen(remote_name));
    }
    screen_divider[COLS] = '\0';          // last spot with null

    clear();           // Clear the Screen
    move(screen_middle, 0);
    standout();        // makes next line bold
    addstr(screen_divider);
    standend();        // turns off bold
    move(LINES-1, 0);  // Move Cursor to Lower Left

    count = 0;
    while(not_done) {
        // read keyboard input until end of line
        i = 0;
        while((ch = getch()) != '\n') { 
            buf[i++] = ch;         // but what about buffer overflow!
        }
        buf[i] = '\0';
        
        if (strcmp(buf, "quit") == 0) {
            not_done = 0;
        }
        move(LINES-2 - count,count);// Move Cursor to Location
        addstr("Clemson "); // Display some Text
        sprintf(buf_count, "%d", count);
        addstr(buf_count); 
        move(screen_middle - 1 - count, count+1);
        addstr("Tigers ");
        addstr(buf_count); 
        count++;
        if (count >= LINES/2 - 1) {
            clear();    // Clear the Screen
            count = 0;
        }
        move(LINES-1, 0);  // Move Cursor to Lower Left
        clrtoeol();        // Clear current line to end of line
        refresh();         // Update Display
    }

    endwin();    // Quit ncurses system
    free(screen_divider);
    return 0;
}

