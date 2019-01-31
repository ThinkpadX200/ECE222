/* lab4.c template
 * Richard Coffey  << replace with your name!
 * rrcoffe        << and user name
 * ECE 2220, Fall 2016
 * MP4
 * Subject: ECE222-1,#4
 *
 * Purpose: A file parser that reads in a file, and then can be used to read in bytes, or change strings. 
 *
 *
 * Assumptions: 
    Search strings will only use one wildcard
 *
 *
 * Command line argument
 *   "lab4 filename.txt"
 *
 * Bugs: Does not find string when searched, and give a seg fault when asked to replace a string, most likely because it does not find it.
 *
 * Notes: Much of this code was written with the fundamental misunderstanding that you needed to allocate and free a new block of memory to do every operation. After realizing that this was not the case, it was very difficult to adapt the code to use a single memory block.
 *
 * See the ECE 222 programming guide
 *
 * Format with
 *     astyle --style=kr lab3.c
 *
 * Replace "kr" with: bsd, ansi, java, gnu, linux, vtk, or google. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 128

/*----------------------------------------------------------*/
/* Here is a sketch for a start to searching for a byte
 *
 * Search for a matching byte starting after position.  Wrap around
 *    to beginning of memory until found or return to starting position
 *p /
 * input:
 *     byte is a string with the hex characters for the byte to find
 *     mem_start is starting address of the data
 *     mem_size is the number of bytes in the memory block
 * input/output:
 *     position is the location of the cursor.  If the byte is found
 *     then position is updated to the memory offset for the match
 *     If the byte is NOT found, then position is not changed
 * return value:
 *     true if byte found
 */
int find_next_byte_match(int *position, char *byte, char *mem_start, int mem_size)
{
    int found = 0; 
    int i;   
    int byteT;
    int byteC;
    char *ptr;

    //converts hex string into decimal
    for (i = 0; i < 2; i++) {
        if (*(byte+i) >= 'A' && *(byte+i) <= 'Z') {
            byteT = (*(byte+i) - 'A') + 10;
            if (i == 0) {
                byteC = byteT << 4;
            }
            if (i == 1) {
                byteC = byteC | byteT;
            }
        } else if (*(byte+i) >= 'a' && *(byte+i) <= 'z') { 
            byteT = (*(byte+i) - 'a') + 10;
            if (i == 0) {
                byteC = byteT << 4;
            }
            if (i == 1) {
                byteC = byteC | byteT;
            }
        } else {
            byteT = (*(byte+i) - '0');
            if (i == 0) {
                byteC = byteT << 4;
            }
            if (i == 1) {
                byteC = byteC | byteT;
            }
        }
    }

    if (byteC > 128) {
        byteC -= 256;
    }

    //finds match
    ptr = (mem_start+(*position+1));

    for (i = *position+1; i < mem_size; i++) {
        if (byteC == *ptr) {
            *position = i;
            found = 1;
            break;
        }
        ptr++;
    }
    
    //Loop back to beginning
    ptr = mem_start;

    if (found == 0) {
        for (i = 0; i < *position; i++) {
            if (byteC == *ptr) {
                *position = i;
                found = 1;
                break;
            }
            ptr++;
        }
    } 

    return found;
}
/*----------------------------------------------------------*/
/* Here is a sketch for a start to searching for a string
 *
 * Search for a matching string starting after position.  Wrap around
 *    to beginning of memory until found or return to starting position
 *
 *    Note: the string does not match if it overlaps both the characters
 *          at the end and the beginning of memory.
 *
 * input:
 *     str      : string to find.  It has already been processed to remove escapes
 *     wild_pos : -1 if no wildcard, or position in str with the wildcard
 *     mem_start: starting address of the data
 *     mem_size : the number of bytes in the memory block
 *
 * input/output:
 *     position is the location of the cursor.  If the string is found
 *     the position is updated to the memory offset for the first byte
 *     that matchs the string
 * return value:
 *     true if string found
 *
 */
int find_next_string(int *position, const char *str, int wild_pos, 
        char *mem_start, int mem_size)
{   
    int found = 0;
    int i = 0;
    int j;
    int strl;
    char *ptr;


    strl = strlen(str);

    if (wild_pos == 0) {
        j = 1;
    }

    //finds next string
    //note, tried to do all pointer arrimetic together, this is easier
    ptr = (mem_start + (*position+1));
    for (i = *position+1; i < mem_size; i++) {
        //checks single character string case
        if (j == strl) {
            found = 1;
            *position = i - strl; 
            break;
        }
        //checks multi char string case until null
        if (str[j] == *ptr || j == wild_pos) {
            j++;
            if (str[j] == '\0') {
                found = 1;
                *position = *position + i;
            }
            ptr++;
        }
        //if not match reset
        if (str[j] != *ptr) {
            j = 0;

        }
        ptr++;
    }


    //Loop back to the beginning
    ptr = mem_start;

    if (found == 0) {
        for (i = 0; i < *position+1; i++) {
            // if i increments back to the beginningi
            if (i == *position) {
                if (j == strl) {
                    found = 1;
                    break;
                }
                for (j = 0; j < strlen(str); j++) {
                    if (str[0] != *ptr) {
                        found = 0;
                        break;
                    }
                    ptr++;
                }
            }
            if (j == strl) {
                *position = (*position+1)-1;
                found = 1;
                break;
            } 
            if (str[j] == *ptr || j == wild_pos) {
                j++;
            }
            if (str[j] != *ptr) {
                j = 0;
                if (wild_pos == 0) {
                    j = 1;
                }
            }
        }
    }

    return found;
}
/*----------------------------------------------------------*/
/* Here is a sketch for a start to replacing a string
 *
 * Search for a matching string starting after position.  Wrap around
 *    to beginning of memory until found or return to starting position
 *
 *    Note: the string does not match if it overlaps both the characters
 *          at the end and the beginning of memory.
 *
 * input:
 *     str      : string to find.  It has already been processed to remove escapes
 *     wild_pos : -1 if no wildcard, or position in str with the wildcard
 *     rstr     : string for replacement
 *     mem_start: starting address of the data
 *     mem_size : the number of bytes in the memory block
 *
 * input/output:
 *     position is the location of the cursor.  If the string is found
 *     the position is updated to the memory offset for the first byte
 *     that matchs the string
 * return value:
 *     true if string found
 */
int replace_next_string(int *position, char *str, int wild_pos, char *rstr, 
        char *mem_start, int mem_size)
{
    int found = 0;
    int i = 0;
    int j;
    int strl;
    int rstrl;
    //int tempP;
    char *ptr;

    strl = strlen(str);
    //
    ptr = (mem_start + (*position+1));

    //find first matching byte
    for (i = *position+1; i < mem_size; i++) {
        if (j == strl) {
            found = 1;
            *position = i-strl;
            break;
        }

        if (str[j] == *ptr || j == wild_pos) {
            j++;
            ptr++;
        }
        else if (str[j] != *ptr) {
            j = 0;
            ptr++;
        }
    }

    ptr = mem_start;
    if (found == 0) {
        for (i = 0; i < *position+1; i++) {
            // if i increments back to the beginning
            if (*position == i) {
                for (j = 0; j < strlen(str); j++) {
                    if (str[0] != *mem_start) {
                        found = 0;
                        break;
                    }
                    ptr++;
                }
            }
            if (j == strl) {
                found = 1;
                break;
            } 
            if (str[j] == *ptr || j == wild_pos) {
                j++;
                ptr++;
            }
            else if (str[j] != *ptr) {
                j = 0;
                if (wild_pos == 0) {
                    j = 1;
                }
            }
        }
    }

    rstrl = strlen(rstr);

    if (found == 1) {
        for (i = 0; i < rstrl; i++) {
            *(mem_start + *position) = rstr[i];
            mem_start++;
        }
    }





    return found;
}
/*----------------------------------------------------------*/
/* Here is a sketch for a start to search and replace byte 
 *
 * Search for a matching byte starting after position.  Wrap around
 *    to beginning of memory until found or return to starting position
 *
 * input:
 *     byte_to_find is a string with the hex characters for the byte to find
 *     byte_to_replace is a string with the hex characters to replace 
 *     mem_start is starting address of the data
 * input/output:
 *     position is the location of the cursor.  If the byte is replaced
 *     then position is updated to the memory offset for the match
 *     If the byte is NOT found, then position is not changed
 * return value:
 *     true if byte replaced
 */
int replace_next_byte(int *position, char *byte_to_find, char *byte_to_replace, char *mem_start, int mem_size)
{
    int found = 0;
    int i;
    char *ptr;
    int byteTR; //dec temp replace
    int byteCR; //dec correct replace
    int byteTF; //dec temp find
    int byteCF; //dec correct find


    //converts byte_to_find to decimal
    for (i = 0; i < 2; i++) {
        if (*(byte_to_find+i) >= 'A' && *(byte_to_find+i) <= 'Z') {
            byteTF = (*(byte_to_find+i) - 'A') + 10;
            if (i == 0) {
                byteCF = byteTF << 4;
            }
            if (i == 1) {
                byteCF = byteCF | byteTF;
            }
        } else if (*(byte_to_find+i) >= 'a' && *(byte_to_find+i) <= 'z') { 
            byteTF = (*(byte_to_find+i) - 'a') + 10;
            if (i == 0) {
                byteCF = byteTF << 4;
            }
            if (i == 1) {
                byteCF = byteCF | byteTF;
            }
        } else {
            byteTF = (*(byte_to_find+i) - '0');
            if (i == 0) {
                byteCF = byteTF << 4;
            }
            if (i == 1) {
                byteCF = byteCF | byteTF;
            }
        }
    }

    if (byteCF > 128) {
        byteCF -= 256;
    }

    //converts byte to replace to decimal
    for (i = 0; i < 2; i++) {
        if (*(byte_to_replace+i) >= 'A' && *(byte_to_replace+i) <= 'Z') {
            byteTR = (*(byte_to_replace+i) - 'A') + 10;
            if (i == 0) {
                byteCR = byteTR << 4;
            }
            if (i == 1) {
                byteCR = byteCR | byteTR;
            }
        } else if (*(byte_to_replace+i) >= 'a' && *(byte_to_replace+i) <= 'z') { 
            byteTR = (*(byte_to_replace+i) - 'a') + 10;
            if (i == 0) {
                byteCR = byteTR << 4;
            }
            if (i == 1) {
                byteCR = byteCR | byteTR;
            }
        } else {
            byteTR = (*(byte_to_replace+i) - '0');
            if (i == 0) {
                byteCR = byteTR << 4;
            }
            if (i == 1) {
                byteCR = byteCR | byteTR;
            }
        }
    }

    if (byteCR > 128) {
        byteCR -= 256;
    }

    ptr = (mem_start+(*position+1));

    for (i = *position+1; i < mem_size; i++) {
        if (byteCF == *ptr) {
            *position = i;
            found = 1;
            break;
        }
        ptr++;
    }

    ptr = mem_start;

    if (found == 0) {
        for (i = 0; i < *position; i++) {
            if (byteCF == *ptr) {
                *position = i;
                found = 1;
                break;
            }
            ptr++;
        }
    } 

    if (found == 1) { 
        *(mem_start + *position) = byteCR;
    }

    return found;

}
/*----------------------------------------------------------*/
/* Here is a sketch for a start to printing a 16-byte aligned
 * line of text.
 *
 * input:
 *     position is the cursor location 
 *     slen is the length of the matching string (1 if just cursor position)
 *     mem_start is starting address of the data
 *     mem_size is the number of bytes in the memory block
 *
 * output:
 *     prints a 16-byte line of text that is aligned so that starting
 *     address is a multiple of 16
 *
 *     If slen is greater than one then continues to print 16-byte lines
 *     so that all characters in the string are displayed
 */
void print_line(int position, int slen, int wild_pos, char *mem_start, int mem_size)
{
    // char *line ;
    // char *c_ptr;
    // char ch;
    int i;
    int temploc = 0;
    int n; //offset
    int lineend = 0;
    int again = 0;


    n = position % 16;
    //temploc is used to find the arrow location

    if (position % 16 == 0) { 
        temploc = position;
    } else if (position < 16) {
        temploc = 0;

    } else {
        temploc = position - (position % 16);
    }

    while (again == 0) {

        printf("\t  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");

        if (position < 16) {
            printf("[     %i] ", (position-(position%16)));

        } else if (position < 100) {
            printf("[    %i] ", (position-(position%16)));    

        } else if (position < 1000) {
            printf("[   %i] ", (position-(position%16)));

        } else if (position < 10000) {
            printf("[  %i] ", (position-(position%16)));

        } else {
            printf("[ %i] ", (position-(position%16)));
        }     


        // n = 0; //(int) (position - temploc);  // offset from start of array
        // printf("ptrc = &[%d], *ptrc = %c\n",  , *ptrc);
        for (i=temploc; i<temploc+16; i++) {
            if (isprint(mem_start[i])) { 
                printf(" %c ", mem_start[i]);
            } else {
                printf("   ");
            }
        }
        printf("\n\t ");
        for (i=temploc; i<temploc+16; i++) {
            printf(" %02hhX", mem_start[i]);
        }
        printf("\n\t");
        for (i=0; i<3*n; i++){
            printf(" ");     
        }  

        if (wild_pos != 0 && slen == 1) {
            printf("  ^\n");
            again = 1;
            lineend = 0;
        }

        else if (wild_pos == 0 && slen == 1) {
            printf("  ^*\n");
            again = 1;
            lineend = 0;
        }

        //if we have more then one string
        else if (slen != 1) {

            for (i=0; i < slen-1; i++) {
                printf("---");
            }
            lineend = 1;

            if (lineend == 1) {

                if (wild_pos != -1) {
                    printf("-*|\n");
                    again = 1;
                }

                else {
                    printf("--|\n");
                    again = 1;
                }
            }


            // free(line);     
    }
}
}

/*----------------------------------------------------------*/
/* Process the search string to remove escapes and identify the location
 * of the first wildcard symbol.
 *
 * input: inputstr is the string from the command
 * output:
 *    searchst    r: a copy of the input string with the escapes removed
 *
 * the return value is
 *    -1: no wildcard symbol found
 *     X: the position in the searchstr with the first wildcard
 array = (double *) malloc (10 * sizeof(double));
 if (array == NULL) {    // make sure O/S returned valid address
 printf("Unable to allocate array\n");
 exit(1);
 }
 *
 * This function does not change the input string, and assumes that the pesky '\n'
 *      has been removed.
 *
 * Note that unlike for the s command to search and replace, it is NOT
 * possible for the input string to be invalid.  So there cannot be
 * an invalid input string for searches. 
 *
 * The only possible issue is if the '\' character is the LAST character
 * in the search string. We will take the convention that if the LAST
 * character is the '\' then it is NOT an escape, and should be a literal '\'.
 *
 * Example:
 *
 *   "\"  means search for '\' since the \ is the last character
 *   "\\" also means search for '\' since the first \ is an escape
 *
 *
 * This is not true for the s command (because that makes the dividing '/'
 * poorly defined).
 */
int process_search_string(const char *inputstr, char *searchstr)
{
    int wild_position = -1;  // -1 means no wildcard found

    int i = 0;
    int j = 0;

    while (inputstr[i] != '\0') {
        if (inputstr[i] == '.') {
            wild_position = i;
        }
        else if (inputstr[i] == '\\') {
            i++;
        }
        searchstr[j] = inputstr[i];
        j++;
        i++;
    }
    searchstr[j] = '\0';


    return wild_position;
}


/* Simple test to verify that the replacement string has correct form.
 *
 * Input: inputstr
 * Output:
 *    searchstr: the search string with all escape '\' symbols removed
 *    replacestr: the string used to replace the search string.
 *
 *    the return value:
 *      -2 if there is any error in the strings
 *      -1 if the strings are correct and there is no wildcard
 *       X for some integer X if the input is correct and the first wildcard 
 *         is found at position X in the searchstr.
 *
 * This function does not change the input string, and assumes that the pesky '\n'
 *      has been removed.
 *
 * The string must start and end with a '/', and a '/' divides the input into
 * the searchstr and the replacestr.
 * .  
 * The shortest pattern has the form /a/b/
 *
 * The pattern must have the form /string1/string2/  
 * The string1 many contain one wildcard '.' symbol.  In addition, multiple 
 *    escape '\' symbols may be included.  
 *
 *    Process string1 to create the output searchstr.  Remove the escape symbols 
 *    and save the location of the first wildcard sybmol, if found.  
 *
 *    The length of searchstr must match the length of replacestr.
 *
 *    Note that the rule that the replacestr must have the same length as the
 *    searchstr (after escapes have been removed) means that there is no need
 *    for escapes '\' in the replacement string.  No wildcard symbols can be 
 *    included in the replacement string.
 *
 *    Examples
 *      s /Clems.n/Clemson/ -- a wildcard matches any byte but replaces it with 'o'
 *      s /Cl.ms.n/Clemson/ -- The first '.' is wildcard but the second '.' is 
 *                             a literal '.' and must be matched
 *      s /.ear./Here!/ -- The first '.' is a wildcard by the second is not.
 *      s /a\.b/a/b/  -- find the literal pattern "a.b" and change to "a/b". This
 *                       will not match a*b because the '.' is not a wildcard. 
 *                       Note that "a/b" does not cause confusion in finding the 
 *                       replacement string because the length of the replacement
 *                       string is known once "a\.b" is processed to "a.b"
 *      s /a\/b/a+b/ -- find the literal pattern "a/b" and replace with "a+b"
 *      s /a\\b/a/b/ -- find the literal pattern "a\b" and replace with "a/b"
 *
 */
int process_replace_string(const char *inputstr, char *searchstr, char *replacestr)
{
    int wild_position = -1;  // -1 means no wildcard found
    int i = 1;
    int j = 0;
    int k = 0;
    int run = 0;

    while (inputstr[i] != '/') {
        if (inputstr[i] == '.' && run == 0) {
            wild_position = i;
            run = 1;
        }
        else if (inputstr[i] == '\\') {
            i++;
        }

        searchstr[j] = inputstr[i];
        i++;
        j++;

    }
    searchstr[j] = '\0';


    while (inputstr[i+1] != '/') {
        replacestr[k] = inputstr[i+1];
        i++;
        k++;
    }
    replacestr[k] = '\0';


    return wild_position;
}

/*----------------------------------------------------------*/
/* The function to open the file, find the size of the file,
 * malloc memory to hold the contents of the file.
 *
 * There are two return values
 *   1.  the return value is a pointer to the starting
 *       memory location of the data
 *   2.  the size of the memory block in bytes is also
 *       returned in file_size
 */
char *store_file(char * filename, int * file_size)
{
    FILE *fpt;
    char ch;
    int count;
    char *ptrFileInMemory;

    if ((fpt = fopen(filename, "r")) == NULL) {
        printf("Cannot Read from File \"%s\"\n", filename);
        exit (1);
    }

    // hint: see the example filesize.c
    count = 0;
    while (fscanf(fpt,"%c",&ch) == 1) {
        count++;
    }
    *file_size = count;
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

    return ptrFileInMemory;

}


// 
// Functions from here to end of this file should NOT be changed
//
/* print the menu of choices to the user 
 *
 */
void print_menu(void)
{
    printf("Simple editor commands\n\n");
    printf("f FF    : Search for next matching byte (in hex) after current location\n");
    printf("r AB CD : Same as search, then replace byte if found\n");
    printf("/Blue Ridge  : Search for next matching string after current location\n");
    printf("s /Blue/Red / : Same as search, then replace string of same length\n");
    printf("G num : Goto byte at position 'num' from start of memory\n");
    printf("j     : Move forward 16 bytes from current location\n");
    printf("k     : Move backward 16 bytes from current location\n");
    printf("q     : Quit\n");
    printf("?     : Print this menu\n");
}

/*----------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char *filename;        // the input file name
    char *file_in_memory;  // starting address of memory block to store file
    int file_size;
    int fn_len;            // length of the input file name
    int found = 0;         // if search was successful
    int location = 0;      // current location in memory [0, file_size)
    int items;
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    char replacecs[MAXLINE];
    char searchstr[MAXLINE];
    char replacestr[MAXLINE];

    if (argc != 2) {
        printf("Usage: lab4 filename\n");
        exit(1);
    }

    // prepare filename
    fn_len = strlen(argv[1]);
    // remember the null
    filename = (char *) malloc((fn_len + 1) * sizeof(char));
    strcpy(filename, argv[1]);

    // open file and store in memory starting at pointer
    file_in_memory = store_file(filename, &file_size);

    print_menu();

    printf("> ");
    while (fgets(line, MAXLINE, stdin) != NULL) {
        printf("\n");
        items = sscanf(line, "%s%s%s", command, inputcs, replacecs);
        if (items == 2 && strcmp(command, "f") == 0) { 
            if (strlen(inputcs) != 2 || !isxdigit(inputcs[0]) ||
                    !isxdigit(inputcs[1])) {
                printf("f Invalid byte: %s\n", inputcs);
            } else {
                found = find_next_byte_match(&location, inputcs, file_in_memory, file_size);
                if (!found) {
                    printf("Did not find byte: %s\n", inputcs);
                } else {
                    print_line(location, 1, -1, file_in_memory, file_size);
                }
            }
        } else if (items == 3 && strcmp(command, "r") == 0) {  
            if (strlen(inputcs) != 2 || !isxdigit(inputcs[0]) ||
                    !isxdigit(inputcs[1]) || strlen(replacecs) != 2 ||
                    !isxdigit(replacecs[0]) || !isxdigit(replacecs[1])) {
                printf("r Invalid bytes: %s %s\n", inputcs, replacecs);
            } else {
                found = replace_next_byte(&location, inputcs, replacecs, file_in_memory, file_size);
                if (!found) {
                    printf("Did not replace byte: %s\n", inputcs);
                } else {
                    print_line(location, 1, -1, file_in_memory, file_size);
                }
            }
        } else if (strncmp(line, "/", 1) == 0) {  
            strcpy(inputcs, line + 1);
            // chomp the pesky \n
            if (inputcs[strlen(inputcs)-1] == '\n')
                inputcs[strlen(inputcs)-1] = '\0';
            int wild_pos = process_search_string(inputcs, searchstr);
            found = find_next_string(&location, searchstr, wild_pos, 
                    file_in_memory, file_size);
            if (!found) {
                if (wild_pos == -1) {
                    printf("String not found: '%s' (no wildcard)\n", searchstr);
                } else {
                    printf("String not found: '%s' wildcard at %d\n", 
                            searchstr, wild_pos);
                }
            } else {
                print_line(location, strlen(searchstr), wild_pos, file_in_memory, file_size);
            }
        } else if (strncmp(line, "s /", 3) == 0) {  
            strcpy(inputcs, line + 2);
            // chomp the pesky \n
            if (inputcs[strlen(inputcs)-1] == '\n')
                inputcs[strlen(inputcs)-1] = '\0';
            int wild_pos = process_replace_string(inputcs, searchstr, replacestr);
            if (wild_pos == -2) {
                printf("s Invalid input: %s\n", inputcs);
            } else {
                found = replace_next_string(&location, searchstr, wild_pos, 
                        replacestr, file_in_memory, file_size);
                if (!found) {
                    if (wild_pos == -1) {
                        printf("String not replaced: '%s' (no wildcard)\n", searchstr);
                    } else {
                        printf("String not replaced: '%s' wildcard at %d\n", 
                                searchstr, wild_pos);
                    }
                } else {
                    print_line(location, strlen(searchstr), wild_pos, file_in_memory, file_size);
                }
            }
        } else if (items == 2 && strcmp(command, "G") == 0) {  
            int new_location = -1;
            new_location = atoi(inputcs);
            if (new_location < 0 || new_location >= file_size) {
                printf("Invalid goto: %s\n", inputcs);
            } else {
                location = new_location;
                print_line(location, 1, -1, file_in_memory, file_size);
            }
        } else if (items == 1 && strcmp(command, "j") == 0) { 
            if (location + 16 >= file_size) {
                printf("Invalid move down: %d\n", location);
            } else {
                location += 16;
                print_line(location, 1, -1, file_in_memory, file_size);
            }
        } else if (items == 1 && strcmp(command, "k") == 0) { 
            if (location - 16 < 0) {
                printf("Invalid move up: %d\n", location);
            } else {
                location -= 16;
                print_line(location, 1, -1, file_in_memory, file_size);
            }
        } else if (items == 1 && strcmp(command, "q") == 0) { 
            break;
        } else if (items == 1 && strcmp(command, "?") == 0) { 
            print_menu();
        } else {
            printf("# :%s", line);
        }
        printf("> ");
    }

    // for every malloc there must be a free
    free(file_in_memory);
    free(filename);

    printf("Goodbye\n");
    return EXIT_SUCCESS;
}
