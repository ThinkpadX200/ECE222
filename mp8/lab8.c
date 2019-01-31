/* 
 *
 * lab8.c template
 * Richard Coffey  << replace with your name!
 * rrcoffe 
 * ECE 2220, Fall 2016
 * MP8
 *
 * Purpose: A chat program.  For this assignment you add code to
 *          display the text using a split screen
 *
 * Assumptions: The port number MYPORT is assumed to be free.  If you
 *              get a failed to bind message try changing the port number
 *              to a larger value (but just increase by +1 until you find
 *              an open port)
 *
 *              To connect to a remote chat program, the port number use by
 *              both programs must be the same.
 *
 *             When debugging may leave terminal in bad state. To reset do:
 *                 stty sane
 *
 * Command line arguments: The hostname to chat with.
 *   
 * Known bugs:
 *
 *
 * You can chat with yourself by using
 *    ./chat localhost
 *
 * to chat between two different machines run chat on each
 * machine with the other machine name as the argument.  However, most
 * packets seem to be blocked by either firewalls or NAT.  Running both
 * programs on different apolloXX.ces.clemson.edu works.
 *
 * This program is based on code posted at Beej's Guide to Network
 * Programming found at http://www.beej.us/guide/bgnet/ 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <curses.h>

#define MYPORT "4950"  // the port users will be connecting to

#define MAXBUFLEN 100

// Collect input from keyboard and send packet to remote host
//
// input: sockfd  -- a socket has already been set up to the remote host
//
// output: 
//    print input from keyboard to bottom half of screen
//
//    return a flag to indicate the program should quit
//         1 -- user typed "quit" so tell main to end
//         0 -- default: do not quit
//
int chat_talker(int sockfd, char **grid)
{
    static char buf[MAXBUFLEN];
    int numbytes;
    int ch;
    int j;
    static int i = 0;

    int screen_middle;
    int quit_flag = 0;
    // change to 1 when user types quit
    screen_middle = LINES/2;
    

    // read keyboard input until end of line
    while ((ch = getch()) != '\n') { 
        if (ch == KEY_BACKSPACE) { //if character equals a backspace decrement index to keep track of typing
            i--;
            clrtoeol();
            refresh();
            return quit_flag;
        } else if (i == COLS-1) { //if we run out of columns break to avoid overflow
            break;
        } else {
            buf[i++] = ch; 
            return quit_flag;
        }
    }
        

        buf[i] = '\0'; //don't forget the null

        if (strcmp(buf, "quit") == 0) {
            quit_flag = 1;
        }

        // Scroll
        for(i = screen_middle+1; i < LINES-2; i++){
            for(j = 0; j < COLS; j++){
                grid[i][j]=grid[i+1][j];
            }
        
            move(i,0);
            clrtoeol();
            addstr(grid[i]);
        }


        //reads buffer into grid and breaks if null
        for (i = 0; i < COLS; i++) {

            if (i < strlen(buf)) {
                grid[LINES-2][i] = buf[i];
            }
            else {

                grid[LINES-2][i] = '\0';
                break;
            }
        }
        move(LINES-2, 0);
        clrtoeol();
        addstr(grid[LINES-2]);
        


        if ((numbytes = send(sockfd, buf, strlen(buf), 0)) == -1) {
            perror("chat: send");
            exit(1);
        }
        
        //clearing buffer and i since they are static        
        for (i = 0; i < strlen(buf)+1; i++) {
            buf[i] = '\0';
        }
        i = 0;

        //moves cursor back
        move(LINES-1, 0);
        clrtoeol();
        refresh();
    
    return quit_flag;
    
}

// Receive packets from a remote host.  
//
// input: sockfd  -- a socket has already been set up to the remote host
//
// output: should print received data to top half of the screen
//
void chat_listener(int sockfd, char **grid)
{
    char buf[MAXBUFLEN];
    int numbytes;
    int i,j,screen_middle,y,x;

    screen_middle = LINES/2;    
    getyx(stdscr, y, x); //records cursor location

    if ((numbytes = recv(sockfd, buf, MAXBUFLEN-1 , 0)) == -1) {
        perror("chat: recv");
        exit(1);
    }

    buf[numbytes] = '\0'; //don't forget the null

    strcpy(grid[screen_middle-1], buf);

    //add null character in listener of chat.c
    //strcpy buf into grid[line number] line right above middle
    // Scroll
    for(i = 0; i < screen_middle-1; i++){
        for(j = 0; j < COLS; j++){
            grid[i][j]=grid[i+1][j];
        }

        move(i+1, 0); //bottom value
        clrtoeol();
        addstr(grid[i]);
        move(LINES-1, 0);
        refresh();
    }

    move(y,x); //resets cursor
    refresh();

}

/* main sets up the structures with IP address information and calls
 * socket, bind, and connect to set up the details of the UDP socket.
 *
 * Use the select() function to wait for input from either the keyboard
 * or from the remote host
 *
 * Changes to main should be minimal.  Just initialize curses
 * and clean up at end.  All other work is in the two
 *    chat_talker
 *    chat_listener
 * functions that collect input that needs to be displayed
 */
int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *addr_result, *p;
    int rv;
    int screen_middle;
    int i;
    char **grid;
    char *screen_divider, *addr;
    const char *remote_name = "apolloXX";
    const char *welcome = "Welcome to ECE222 chat";

    initscr(); 
    keypad(stdscr, TRUE); //enable keypad for backspace, arrows, and function keys

    // grid
    grid = (char **)malloc(LINES * sizeof(char *));
    for (i=0; i<LINES; i++) {
        grid[i] = (char *)malloc(COLS * sizeof(char));
    }
    //allocate screen divider
    screen_divider = (char *) malloc((COLS+1) * sizeof(char));
    screen_middle = LINES/2;
    // WELCOME
    
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

    refresh(); //to avoid the "starting keypress" bug


    if (argc != 2) {
        fprintf(stderr, "usage: chat hostname\n");
        exit(1);
    }

    // set up details for address information data structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    // first: open socket and bind to port on my host so
    // we can receive packets on this port

    // This fills in structure with IP details for the local host
    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &addr_result)) != 0) {
        fprintf(stderr, "getaddrinfo my host: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and bind to the first we can
    for(p = addr_result; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("chat: socket");
            continue;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("chat: bind");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "chat: failed to bind to socket\n");
        return 2;
    }
    freeaddrinfo(addr_result);

    // second: repeat with remote host IP details so we can
    // connect this socket to the remote host only.  As an extension
    // you could use a different port number for the remote host.
    if ((rv = getaddrinfo(argv[1], MYPORT, &hints, &addr_result)) != 0) {
        fprintf(stderr, "getaddrinfo remote host: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and connect to the first we can
    for(p = addr_result; p != NULL; p = p->ai_next) {
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("chat: connect");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "chat: failed to connect socket\n");
        return 2;
    }
    freeaddrinfo(addr_result);


    // set up master set of file descriptors to use with select
    fd_set master;
    fd_set read_fds;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(0, &master);   // standard input is zero
    FD_SET(sockfd, &master);

    int quit_flag = 0; // changed by chat_talker when user types quit
    while(quit_flag == 0) {
        // specify who to monitor on this trial.  We want both
        read_fds = master;
        // block and wait for input from stdin or socket
        if (select(sockfd+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("chat: select");
            exit(4);
        }

        // which one is set?  Rare, but both could be seti
        
        if (FD_ISSET(0, &read_fds)) {
            quit_flag = chat_talker(sockfd, grid);      // keyboard input
        } 
        if (FD_ISSET(sockfd, &read_fds)) {
            chat_listener(sockfd, grid);                // remote host sent data
        } 
    }

    //kill curses and free stuff

    refresh();
    close(sockfd);
    endwin();   
     
    for (i = 0; i < LINES; i++) {
        free(grid[i]);
    }
    free(grid);
    free(screen_divider);

    return 0;
}
