/*
   Lab.c
   Richard Coffey
   rrcoffe
   ECE-222



 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


int launched;
int rover;
int waypoint;
int alarmrung;
int toMars;
int orbit;

void alrm(int signum) {

    alarmrung = 1;

}

void launch(int signum) {

    if (launched == 0) {
        rover = 0;
        launched += 1;
    }

}

void transmit(int signum) {

    waypoint = waypoint + 10;

}


void SC (int scnum, FILE *fpt) {

    int mcD; //mission control distance    
    int wpcount = 1;
    waypoint = 10;

    signal(SIGALRM, alrm);  
    signal(SIGUSR1, launch);
    signal(SIGUSR2, transmit); 

    srand48(time(NULL)+scnum);
    int marsD = 59 * drand48();
    while (marsD < 30) {
        marsD = 59 * drand48();
    }

    mcD = marsD;
    //use code from alarm2.c
    fprintf(fpt, "Space Craft %d initiating mission. Distance to Mars %d\n", scnum, marsD);

    toMars = 1;
    alarm(1);

    while(1) {
        pause();

        if (toMars == 0 && mcD == 0) {
            fprintf(fpt, "Made it home\n");
            exit(3);
        }

        if (waypoint > 0 && marsD == 0 && launched == 0) {
            fprintf(fpt, "Space Craft %d to Mission Control: entered orbit. Please signal when to launch rover. Waypoints left: %d\n", scnum, waypoint); 
            orbit = 1;
        }

        if (waypoint > 0 && marsD != 0 && orbit == 0) { 
            fprintf(fpt, "Distances to mars: %d Waypoints left: %d\n",marsD, waypoint);
        } 

        if (waypoint > 0 && mcD != 0 && toMars == 0) {
            fprintf(fpt, "Distances to Earth: %d Waypoints left: %d\n",mcD, waypoint);
        }

        if (waypoint == 0) {
            fprintf(fpt, "Space Craft %d. Lost in Space\n", scnum);
            exit(2);
        }

        if ((orbit == 0 && launched == 1) || (orbit == 2 && launched > 1)) { 
            fprintf(fpt, "Error: Launching Rover unsuccessful. Ka-boom!\n"); 
            exit(4);
        }

        if (orbit == 1 && launched == 1) {
            fprintf(fpt, "Rover launched, returning to earth\n");
            orbit = 2;
            toMars = 0;
            
        }

        if (waypoint == 5) {
            fprintf(fpt, "Houston, we have a problem!\n");
        }

        if (toMars == 1 && orbit == 0 && marsD != 0) {
            marsD -= 1;
        }

        if (toMars == 0 && orbit == 2 && mcD != 0) {
            mcD -= 1;
        }

        if (wpcount == 2) {
            waypoint -= 1;
            wpcount = 0;
        }
        wpcount++;

        if (alarmrung > 0) {
            alarmrung = 0;

            alarm(1);
        }


    } 
}

void MC (int scnum, FILE *fpt, int *pid) {

    char text[50];

    while(1) {
        printf("enter command: \n");
        scanf("%s", text);
        if (strcmp(text,"l1") == 0) {
            kill(pid[0], SIGUSR1);
        }
        if (strcmp(text,"l2") == 0) {
            kill(pid[1], SIGUSR1);
        }
        if (strcmp(text,"l3") == 0) {
            kill(pid[2], SIGUSR1);
        }
        if (strcmp(text,"k1") == 0) {
            printf("Terminated Space Craft 1\n");
            kill(pid[0], SIGKILL);
        }
        if (strcmp(text,"k2") == 0) {
            printf("Terminated Space Craft 2\n");
            kill(pid[1], SIGKILL);
        }
        if (strcmp(text,"k3") == 0) {
            printf("Terminated Space Craft 3\n");
            kill(pid[2], SIGKILL);
        }
        if (strcmp(text,"t1") == 0) {
//            printf("Signal 2 to %d\n", pid[0]);
            kill(pid[0], SIGUSR2);
        }
        if (strcmp(text,"t2") == 0) {
  //          printf("Signal 2 to %d\n", pid[1]);
            kill(pid[1], SIGUSR2);
        }
        if (strcmp(text,"t3") == 0) {
    //        printf("Signal 2 to %d\n", pid[2]);
            kill(pid[2], SIGUSR2);
        }
        if (strcmp(text, "q") == 0) {
            kill(pid[0], SIGKILL);
            kill(pid[1], SIGKILL);
            kill(pid[2], SIGKILL);
            exit(5);
        }
      //  printf("Completed signal\n");

    }
}

#define NUMTTYS 4


int main( int argc, char *argv[] ) { 

    FILE *fpt[NUMTTYS];
    int ttyindex;
    char strDev[100];
    int Terminal[NUMTTYS];
    int term_cnt = 0;
    int pid[NUMTTYS], i;
    int wait_ret1, wait_ret2, wait_ret3;
    int child_status1, child_status2, child_status3;
    int exit_byte;
    int exitSC1, exitSC2, exitSC3;
    int success = 0;    


    if (argc != NUMTTYS+1) {
        printf("Usage: ./lab7 1 2 3 4, where the numbers are the terminal pts values\n");
        exit(1);
    }

    // determine which ttys are open and available to this user
    for (term_cnt = 0; term_cnt < NUMTTYS; term_cnt++)
    { 
        ttyindex = -1;
        ttyindex = atoi(argv[term_cnt+1]);
        if (ttyindex < 1) {
            printf("invalid terminal number %s\n", argv[term_cnt+1]);
            exit(1);
        }

        sprintf(strDev, "/dev/pts/%d", ttyindex);

        if ((fpt[term_cnt] = fopen(strDev,"w")) != NULL) {
            Terminal[term_cnt] = ttyindex;
        } else {
            printf("failed to open terminal %s\n", strDev);
            exit(1);
        }
    }

//    printf("This program can write to %d terminals\n", term_cnt);
    for (ttyindex=0; ttyindex < term_cnt; ttyindex++)
    {

       // printf("Terminal[%d] = %d\n", ttyindex, Terminal[ttyindex]);
        if (ttyindex == 0) {
            fprintf(fpt[ttyindex], "Hello terminal %d, you are mission control\n", Terminal[ttyindex]);
        }
        if (ttyindex != 0) {
            fprintf(fpt[ttyindex], "Hello terminal %d, you are spaceship %d\n", Terminal[ttyindex], ttyindex);
        }
    }


    //    signal(SIGALRM);
    //  signal(SIGUSR1); 

    i = 0;
    pid[i] = fork(); //1st child fork
    if (pid[i] != 0) { //parent
        i++;
        pid[i] = fork(); //2nd child fork
        if (pid[i] != 0) { //parent
            i++;
            pid[i] = fork(); //3rd child fork
            if (pid[i] != 0) { //parent
                i++;
                pid[i] = fork(); //4th child fork
                if (pid[i] != 0){ //parent
                    i++;
                }
                else{
                    MC(4, fpt[0], pid);
                }

            } else{
                SC(3, fpt[3]);
            }
        } else{
            SC(2, fpt[2]);    
        }
    } else{
        SC(1, fpt[1]);
    } 

    //study wait2.c
    //remeber to kill mission control after 
    //wait pid child process use 3 waits

    wait_ret1 = wait(&child_status1);
    for (i = 0; i < 3; i++) {
        if (pid[i] == wait_ret1) {
            exitSC1 = i+1;
        }
    }
    exit_byte = child_status1 >> 8;
    if (exit_byte == 2) {
        printf("Vaya Con Dios, Space craft %d\n", exitSC1);       
    }
    if (exit_byte == 3) {
        printf("Welcome home, Space craft %d\n", exitSC1); 
        success++;      
    } 
    if (exit_byte == 4) {
        // printf("Mission Failed, Space craft %d\n", exitSC1);       
    }


    wait_ret2 = wait(&child_status2);
    for (i = 0; i < 3; i++) {
        if (pid[i] == wait_ret2) {
            exitSC2 = i+1;
        }
    }
    exit_byte = child_status2 >> 8;
    if (exit_byte == 2) {
        printf("Vaya Con Dios, Space craft %d\n", exitSC2);       
    }
    if (exit_byte == 3) {
        printf("Welcome home, Space craft %d\n", exitSC2);  
        success++;     
    } 
    if (exit_byte == 4) {
        // printf("Mission Failed, Space craft %d\n", exitSC2);       
    }

    wait_ret3 = wait(&child_status3);
    for (i = 0; i < 3; i++) {
        if (pid[i] == wait_ret3) {
            exitSC3 = i+1;
        }
    }
    exit_byte = child_status3 >> 8;
    if (exit_byte == 2) {
        printf("Vaya Con Dios, Space craft %d\n", exitSC3);       
    }
    if (exit_byte == 3) {
        printf("Welcome home, Space craft %d\n", exitSC3);
        success++;       
    } 
    if (exit_byte == 4) { 
     // printf("Mission Failed, Space craft %d\n", exitSC3);       
    }

    if (success == 3) {
        printf("Congratulations team: Mission successful\n");
    }
    else { 
        printf("Mission failed\n");
    }
    
    //Designing function calls 
    //    if (signal == "ln") {       
    //      }
    kill(pid[3], SIGHUP);

    

    for (ttyindex=0; ttyindex < term_cnt; ttyindex++)
    { 
        fclose(fpt[ttyindex]);
    }

return 0;
}


