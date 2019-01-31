/* lab2.c
 * Richard Coffey   <<-- you must change to your name and id
 * rrcoffe
 * ECE 222, Fall 2016
 * MP2
 *
 * Purpose:  A template for MP2 -- replace
 *
 * Assumptions:
 *  #1:  The menu driven input was provided and must be used exactly
 *       as written.  A user can enter commands:
 *            enc CUt
 *            dec 0E8A549C
 *            quit
 *       Encoding takes three printable ASCII letters 
 *       Decoding takes up to eight HEX digits. If exactly eight digits are
 *            entered, the first digit must be 0 or 1.
 *            Leading zeros can be dropped.
 *
 *  #2:  The string and character type libraries cannot be used except as
 *       already provided.  These libraries are for checking inputs in main 
 *       and in printing after decoding is complete.  They cannot be used
 *       for anyother purpose.
 *  
 *  #3:  No arrays can be used (excpet as already provided for collecting
 *       keyboard input).  You must use bitwise operators for all encoding 
 *       and decoding.  If you want to use an array as a lookup table you 
 *       must first propose your design and get it approved.  Designs that
 *       use tables to avoid bitwise operators will not be approved.  There
 *       are many good and simple designs that do not require tables.
 *
 *  #4   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.  
 *
 * Bugs:
 *
 * See the ECE 223 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 *
 * To create a nicely formated PDF file for printing install the enscript 
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 100

// function prototypes
void encode(unsigned char first_letter, unsigned char second_letter, 
        unsigned char third_letter);
void decode(unsigned int codeword);

int main()
{
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    int  items;
    int i, invalid;
    unsigned int codeword;

    printf("\nMP2: encoding and decoding (29, 24) Hamming code.\n");
    printf("Commands:\n\tenc 3-letters\n\tdec 8-hex-digits\n\tquit\n");

    // each call to fgets, collects one line of input and stores in line
    while (fgets(line, MAXLINE, stdin) != NULL) {
        items = sscanf(line, "%s%s", command, inputcs);
        if (items == 1 && strcmp(command, "quit") == 0) { 
            break;
        } else if (items == 2 && strcmp(command, "enc") == 0) { 
            // encoding
            if (strlen(inputcs) != 3 || !isprint(inputcs[0]) ||
                    !isprint(inputcs[1]) || !isprint(inputcs[2])) {
                printf("Invalid input to encoder: %s\n", inputcs);
            } else {
                encode(inputcs[0], inputcs[1], inputcs[2]);
            }
        } else if (items == 2 && strcmp(command, "dec") == 0) {  
            // decoding: convert hex digits to integer
            items = sscanf(inputcs, "%x", &codeword);
            if (items != 1 || strlen(inputcs) > 8) {
                printf("Invalid input to decoder: %s\n", inputcs);
            } else {
                // verify all digits are hex characters because
                // scanf does not reject invalid letters
                for (i=0, invalid=0; i < strlen(inputcs) && !invalid; i++) {
                    if (!isxdigit(inputcs[i])) 
                        invalid = 1;
                }
                // if 8 digits, leading digit must be 1 or 0
                if (invalid) {
                    printf("Invalid decoder digits: %s\n", inputcs);
                } else if (strlen(inputcs) == 8 && inputcs[0] != '1'
                        && inputcs[0] != '0') {
                    printf("Invalid decoder leading digit: %s\n", inputcs);
                } else {
                    decode(codeword);
                }
            }
        } else {
            printf("# :%s", line);
        }
    }
    printf("Goodbye\n");
    return 0;
}

/* encode: calculates parity bits and prints codeword
 *
 * input: three ASCII characters
 * assumptions: input is valid
 *
 * Example: if input letters are is 'C', 'U', and 't'
 * the final print must be:
 *  ---01110 10001010 01010100 10011100
 *  Codeword: 0x0E8A549C
 */
void encode(unsigned char first_letter, unsigned char second_letter,
        unsigned char third_letter)
{
    // you must construct the codeword
    unsigned int codeword = 0;
    int count1;
    int count2;
    int count3;
    //char codeword1;
   // char codeword2;
   // char codeword3;
   int temp3 = third_letter;
   int temp2 = second_letter;
   int temp1 = first_letter;
   //double bin1 = 0;
   //double bin2 = 0;
   //double bin3 = 0;
    unsigned long binCode;
    //int binCodeword = 0x00;
    // char mask1 = 0x08;
   // int mask2;
   // int mask3;
   int i = 1;  // dummy variable

    printf("%9s%9c%9c%9c\n", "Encoding:", third_letter, second_letter, first_letter);
    printf(" 0x    00%9x%9x%9x\n", third_letter, second_letter, first_letter);
    printf(" -------- ");

    //Quick Printing for Binary for ASCII inputs
    for (count1 = 7; count1 >= 0; count1--) {
	printf("%c", (third_letter & (1 << count1)) ? '1' : '0');
	
    }     
    printf(" ");

    for (count2 = 7; count2 >= 0; count2--) {
	printf("%c", (second_letter & (1 << count2)) ? '1' : '0');	
    }
    printf(" ");

    for (count3 = 7; count3 >= 0; count3--) {
	printf("%c", (first_letter & (1 << count3)) ? '1' : '0');
    }
   
    //Calculating 24 bit binary codeword

   binCode = first_letter;
   binCode = binCode | (second_letter << 8);
   binCode = binCode | (third_letter << 16);
/*
   int count = 0;
   int first;
   int second;
   int third;


    while (count < 8) {
	temp1 = first_letter%2;
	first = first + (temp1*i);

	temp2 = second_letter%2;
	second = second + (temp2*i);

	temp3 = third_letter%2;
	third = third + (temp3*i);

        i = i*10;
	count++;
	first/2;
	second/2;
	third/2;
	}
 
     
    binCode = third | 0x000000;
    binCode = (binCode << 8 | second);
    binCode = (binCode << 16 | first);
   // binCode <<= 24; 
    
*/
   // codeword = 0x0E8A549C;
    //binCode = 7624003;
    printf("\n");    
    
    unsigned long binCodeO = binCode; 
    //Translating information word to code word 
    int tempBit;
    int x; 
    //Temporarily fill in the paritybits
    binCode = binCode << 5;

    //reads first bit in old binCode
    tempBit = (binCodeO & (1 << 0 )) >> 0;
    //sets 3rd bit of binCode 
    if (tempBit == 1) {
        binCode = binCode | (1 << 2);
    }
    else {
 	binCode = binCode & ( ~ (1 << 2));
    }
		

    for (x = 1; x <= 3; x++) {
	tempBit = (binCodeO & (1 << x)) >> x;
        if (tempBit == 1) {
	     binCode = binCode |= (1 << (x+3));
	}
	else {
	     binCode = binCode &= ~ (1 << (x+3));
        }
    }
 
    for (x = 4; x <= 10; x++) {
	tempBit = (binCodeO & (1 << x)) >> x;
        if (tempBit == 1) {
	     binCode = binCode |= (1 << (x+4));
	}
	else {
	     binCode = binCode &= ~ (1 << (x+4));
        } 
    }

    //Sets 16th bit to zero as a temporary 0
    binCode = binCode & ( ~ (1 << 15));

    for (x = 11; x <= 23; x++) {
	tempBit = (binCodeO & (1 << x)) >> x;
        if (tempBit == 1) {
	     binCode = binCode |= (1 << (x+5));
	}
	else {
	     binCode = binCode &= ~ (1 << (x+5));
        }
    }
    //End Translating Information word to code word
    int p2Sum = 0;
    int p4Sum = 0;
    int p8Sum = 0;
    int p16Sum = 0;
    int P1,P2,P4,P8,P16;
    //Calculating Parity bits
    //parity bit 1
    int p1Sum = 0;
    for (x = 2; x < 29; x+=2) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p1Sum = p1Sum + tempBit;
    }
    P1 = p1Sum%2;
    if (P1 == 1) {
	binCode = binCode | (1 << 0);
    }   
 
    //parity bit 2
    for (x = 2; x <= 29; x+=4) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p1Sum = p2Sum + tempBit;
        tempBit = (binCode & (1 << (x+1)) >> (x+1));
	p2Sum = p2Sum + tempBit;
    }
    P2 = p2Sum%2;
    if (P2 == 1) {
	binCode = binCode | (1 << 1);
    }   
    
    //parity bit 4 	 
    for (x = 3; x < 7; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    for (x = 11; x < 15; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    for (x = 19; x < 23; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    for (x = 27; x < 29; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    P4 = p4Sum%2;
    if (P4 == 1) {
	binCode = binCode | (1 << 3);
    }   
    
    //parity bit 8
    for (x = 23; x < 29; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p8Sum = p8Sum + tempBit;
    }
    for (x = 7; x < 15; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p8Sum = p8Sum + tempBit;
    }
    P8 = p8Sum%2;
    if (P8 == 1) {
	binCode = binCode | (1 << 7);
    }   

    //parity bit 16
    
    for (x = 15; x < 29; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p16Sum = p16Sum + tempBit;
    }
    P16 = p16Sum%2;
    if (P16 == 1) {
	binCode = binCode | (1 << 15);
    }   

    // print the information word in binary form with spaces

    // print the parity bits, one bit per line.  Do not change
    // the format, but you must change the dummy variable i
    // to match your design
    printf("P1 : %d\n", P1);
    printf("P2 : %d\n", P2);
    printf("P4 : %d\n", P4);
    printf("P8 : %d\n", P8);
    printf("P16: %d\n", P16);
    
    //print dashes
    printf(" ---");

    // print the codeword bits in binary form with spaces
    for (i = 28; i >= 0; i--) {
	printf("%lu", (binCode & (1 << i)) >> i);;
	if (i == 16) {
		printf(" ");
	}
	if (i == 8) {
		printf(" ");
	}
	if (i == 24) {
		printf(" ");
	}
    }

    printf("\n");
    // print the codeword in hex format
    printf(" Codeword: 0x%.8lX\n", binCode);
    printf("\n");
}

/* decode: checks parity bits and prints information characters
 

 * input: A 29-bit codeword
 * assumptions: the codeword has either no or only one error.
 * 
 *              the information characters may not be printable
 *
 * FYI: when a codeword has more than one error the decoding algorithm
 * may generate incorrect information bits.  In a practical system
 * inforamtion is grouped into multiple codewords and a final CRC
 * code verifies if all codewords are correct.  We will not 
 * implement all of the details of the system in this project. 
 *
 * Example: if the codeword is 0x0E8A549C
 * the final print must be:
 *  No error
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt) 
 *
 * Example with one error in codeword bit 21: 0x0E9A549C
 * Notice the 8 in the previous example has been changed to a 9
 * the final print must be:
 *  Corrected bit: 21
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt) 
 */
void decode(unsigned int codeword)
{
    // you must determine these values:
    unsigned int info_word = 0;
    unsigned char first_letter = 0x42;
    unsigned char second_letter = 0x61;
    unsigned char third_letter = 0x64;
    int bit_error_location = 21;
    int par1;
    int par2;
    int par4;
    int par8;
    int par16;   
    unsigned int tempTop = 0;
    unsigned int tempBot = 0;
    unsigned long binCode;
    int p1Sum = 0;
    int p2Sum = 0;
    int p4Sum = 0;
    int p8Sum = 0;
    int p16Sum = 0;
    int P1,P2,P4,P8,P16;
    int E1,E2,E4,E8,E16;
    int x;
    int tempBit;
    codeword = binCode;

    // dummy variable
    int i = -1;
    
    //Calculates codeword paritybit
    for (x = 2; x < 29; x+=2) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p1Sum = p1Sum + tempBit;
    }
    P1 = p1Sum%2;
 
    //parity bit 2
    for (x = 2; x <= 29; x+=4) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p1Sum = p2Sum + tempBit;
        tempBit = (binCode & (1 << (x+1)) >> (x+1));
	p2Sum = p2Sum + tempBit;
    }
    P2 = p2Sum%2;
    
    //parity bit 4 	 
    for (x = 3; x < 7; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    for (x = 11; x < 15; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    for (x = 19; x < 23; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    for (x = 27; x < 29; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p4Sum = p4Sum + tempBit;
    }
    P4 = p4Sum%2;
    
    //parity bit 8
    for (x = 23; x < 29; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p8Sum = p8Sum + tempBit;
    }
    for (x = 7; x < 15; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p8Sum = p8Sum + tempBit;
    }
    P8 = p8Sum%2;

    //parity bit 16
    
    for (x = 15; x < 29; x++) { 
	tempBit = ((binCode & (1 << x)) >> x);
	p16Sum = p16Sum + tempBit;
    }
    P16 = p16Sum%2;
    
    printf("Decoding: 0x%.8X\n", codeword);
   
    
    // find parity bits
    par1 = (codeword & 0);
    par2 = (codeword & (1 << 1)) >> 1;
    par4 = (codeword & (1 << 3)) >> 3;
    par8 = (codeword & (1 << 7)) >> 7;
    par16 = (codeword & (1 << 15)) >> 15;

    //Find corrected bit location
    if (P1 == par1) {
	E1 = 0;
    } else {
	E1 = 1;
    }
    if (P2 == par2) {
	E2 = 0;
    } else {
	E2 = 1;
    }
    if (P4 == par4) {
	E4 = 0;
    } else {
	E4 = 1;
    }
    if (P8 == par8) {
	E8 = 0;
    } else {
	E8 = 1;
    }
    if (P16 == par16) {
	E16 = 0;
    } else {
	E16 = 1;
    }

    //Start here by bit shifting the error positions

    //correcting info word to 24bits
    info_word = codeword & (0x1FFF7F74);
    info_word = info_word >> 2;
    
    //Parity 4 bit shift
    tempTop = info_word & 0x7FFFFFC;
    tempBot = info_word & 1;
    info_word = (tempTop >> 1) | tempBot;

    //parity 8 bit shift
    tempTop = info_word & 0x3FFFFE0;
    tempBot = info_word & 0xF;
    info_word = (tempTop >> 1) | tempBot;	

    //parity 16 bit shift
    tempTop = info_word & 0x1FFF000;
    tempBot = info_word & 0x7FF;
    info_word = (tempTop >> 1) | tempBot;
    


    first_letter = info_word & 0xFF;
    second_letter =  (info_word & 0xFF00) >> 8;
    third_letter = (info_word & 0xFF0000) >> 16;

     
    // ADD your code here to fix a bit error if one exists
    // and determine the three characters
    //

    // print the error location bits, one bit per line.  Do not change
    // the format, but you must change the dummy variable i
    // to match your design
    printf("E1 : %d\n", i);
    printf("E2 : %d\n", i);
    printf("E4 : %d\n", i);
    printf("E8 : %d\n", i);
    printf("E16: %d\n", i);

    // here is the required format for the prints.  Do not
    // change the format but update the variables to match
    // your design
    if (bit_error_location == 0)
        printf(" No error\n");
    else if (bit_error_location > 0 && bit_error_location <= 29) {
        printf(" Corrected bit: %d\n", bit_error_location);
    } else
        printf(" Decoding failure: %d\n", bit_error_location);

    // you must print the info_word in binary format 

    // print the information word in hex:
    printf(" Information Word: 0x%.6X", info_word);

    // You must convert the info_word into three characters for printing
    // only print information word as letters if 7-bit printable ASCII
    // otherwise print a space for non-printable information bits
    if ((first_letter & 0x80) == 0 && isprint(first_letter))
        printf(" (%c", first_letter);
    else
        printf(" ( ");
    if ((second_letter & 0x80) == 0 && isprint(second_letter))
        printf("%c", second_letter);
    else
        printf(" ");
    if ((third_letter & 0x80) == 0 && isprint(third_letter))
        printf("%c)\n", third_letter);
    else
        printf(" )\n");
    printf("\n");
}

