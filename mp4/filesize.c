/* filedump.c
 * Open a file and store all characters in dynamic memory
 */
#include <stdio.h>
#include <stdlib.h> /* for exit() function */
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE    *fpt;
    char    ch;
    int     count;
    char    *ptrFileInMemory;

    if (argc != 2) {
        printf("Usage:  filesize file\n");
        exit(1);
    }

    fpt=fopen(argv[1],"r");
    if (fpt == NULL) {
        printf("Unable to open %s for reading\n",argv[1]);
        exit(1);
    }

    count = 0;
    while (fscanf(fpt,"%c",&ch) == 1) {
        count++;
    }
    printf("The file contains %d bytes\n", count);

    ptrFileInMemory = (char *) malloc(count * sizeof(char));

    if (ptrFileInMemory ==  NULL) {
        printf("Error: Cannot Allocate Space for File\n");
        exit (1);
    }

    // read in the file and save in memory starting at ptrFileInMemory
    count = 0;
    rewind(fpt);
    while (fscanf(fpt,"%c",&ch) == 1) {
        // same as *(ptrFileInMemory + count) = ch;
        ptrFileInMemory[count] = ch;
        // to print address, same as &(ptrFileInMemory[count])
        // use hhu to print char as unsigned integer
        printf("Byte at address %p is %3hhu (char '%c')\n", ptrFileInMemory + count, 
                ch, isprint(ch) ? ch : ' ');
        count++;
    }
    fclose(fpt);
    return 0;
}
