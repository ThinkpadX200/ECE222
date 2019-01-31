/* lab3.c
 * Richard Coffey          <<-- you must change to your name
 * rrcoffe
 * ECE 222, Fall 2016
 * MP3
 * Subject: ECE222-1,#3
 *
 * Purpose: This program takes two ASCII strings as inputs, and either adds, multiplies, inverts, or powers them together. The result is returned as an ASCII string.
 *
 * Assumptions:
 *  #1   The user is prompted to enter a pseudo arithmetic command.  The 
 *       input must be verified to be grammatically correct.
 *
 *  #2:  The string and character type library cannot be used under
 *       any circumstances.  You are encouraged to develop your own
 *       functions to perform any similar operations that are needed.
 *  
 *  #3   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.  
 *
 * Bugs: None that I'm aware of
 *
 * Notes:
 *
 * See the ECE 222 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  In a terminal on the command line do
 *
 *     astyle --style=kr lab3.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 */

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings 
enum operations { NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);
int stringlen(const char *op);
int caseinvert(int letter);
char transResult(char result);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main()
{
    char input_line[MAXLINE];
    char left_operand[MAXOPER];
    char right_operand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nMP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {

        // clear for next round
        left_operand[0] = right_operand[0] = answer[0] = '\0';

        // check for valid grammar
        operator = process_input(input_line, left_operand, right_operand);

        if (operator == ADD || operator == MUL 
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operand);

            // perform pseudo arithmetic 
            calc_output(left_operand, operator, right_operand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");
    return 0;
}

/* Parse input of the form SOS where S is a string and O is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.  
 * The operand O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be no spaces anywhere in the input, including between 
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason 
 *      then the output must be NOOP.  Otherwise the return value 
 *      corresponds to operand O. 
 *
 *   If the input is grammatically correct, then two strings are also 
 *   returned, one for each of the left and right operands.  If the input 
 *   in invalid the two output strings are undefined.
 */
int process_input(const char *input, char *op_left, char *op_right)
{
    //temp var locations
    int operator;
    int i = 1;
    //int j;
    int rightcount = 0;
    int leftcount = 1;

    if (stringlen(input) > 78) {
        return NOOP;
    }

    

    // checks first letter to make sure it's valid
    if ((input[0] > 96 && input[0] < 123) || (input[0] > 64 && input[0] < 86)) {
        op_left[0] = input[0];
    }
    else {
        return NOOP;
    }
    
    //checks remainder of left op and watches for operator
    while (input[i] != '\0') {
        if ((input[i] > 96 && input[i] < 123) || (input[i] > 64 && input[i] != 94 && input[i] < 86)) {
            op_left[leftcount] = input[i];
            leftcount++;
            if (leftcount > 12) {
                return NOOP;
            }
        }

        else if(input[i] == 43 || input[i] == 42 || input[i] == 47 || input[i] == 94) {
            operator = input[i];
            op_left[leftcount] = '\0';
            break;

        }
        else {
            return NOOP;
        }
        i++;

    }

    //once the operator is found, the rest of the input is put into right op
    while (input[i+1] != '\0') {
        if ((input[i+1] > 96 && input[i+1] < 123) || (input[i+1] > 64 && input[i+1] < 86)) {
            op_right[rightcount] = input[i+1];
            rightcount++;
            if (rightcount > 12) {
                return NOOP;
            }
        }

        else if(input[i+1] == 10 || input[i+1] == '\0') {
            op_right[rightcount] = '\0';
            break;
        }
        else {
            return NOOP;
        }
        i++;

    }

    //returns operator
    if (operator == 43) {
        return ADD;
    } else if (operator == 42) {
        return MUL;
    } else if (operator == 47) {
        return DIV;
    } else if (operator == 94) {
        return POW;
    } else {
        return NOOP;
    }
    return NOOP;
}

/* Pseudo mathematical opertions on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using 
 * math on a finite field with no carries.  
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character 
 * from the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be 
 *         changed.
 */
void calc_output(const char *l_op, int op, const char *r_op, char *result) 
{

    int x; //length type
    int i = 0; //trans l count
    int j = 0; //trans r count
    int rightlen; //right len
    int leftlen; //left len
    char left_op[MAXOPER]; //trans left op
    char right_op[MAXOPER]; //trans right op
    int lc; //length counter
    int rem; //string remainder
    //char answer[MAXOPER];  
    int z; //Z value for inversion
    int y; //y vlue for inversion  
    int tLen; //result length
    int powTemp; //temp int for pow calc

    //Tranlate letters:
    while (l_op[i] != '\0') {
        if (l_op[i] < 'V') {
            left_op[i] = l_op[i] - 39;
            i++;
        }
        else { 	
            left_op[i] = l_op[i] - 97;
            i++;
        }
    }
    left_op[i] = '\0';

    while (r_op[j] != '\0') {
        if (r_op[j] < 'V') {
            right_op[j] = r_op[j] - 39;
            j++;
        }
        else {
            right_op[j] = r_op[j] - 97;
            j++;
        }
    }
    right_op[j] = '\0';

    //Calculate string lengths
    rightlen = stringlen(r_op);
    leftlen = stringlen(l_op);

    //splits up possible cases into: Right string longer, Left string longer, and same string length
    if (rightlen > leftlen) {
        x = 0;
        rem = rightlen - leftlen;
        tLen = rightlen;
    } else if (leftlen > rightlen) {
        x = 1;
        rem = leftlen - rightlen;
        tLen = leftlen;
    } else {
        x = 2;
        tLen = leftlen;
    }
    
    //begin Add
    //Adds each translated integer value together
    //if one string is longer, remaining characters are sent to caseinvert function
    //null is added at the end 

    if (op == 1) {

        if (x == 2) {
            for (lc = 0; lc < rightlen; lc++) { 
                result[lc] = (left_op[lc] + right_op[lc]) % 47;
            }
            result[rightlen] = '\0';
        }
        if (x == 1) {	
            for (lc = 0; lc < rightlen; lc++) { 
                result[lc] = (left_op[lc] + right_op[lc]) % 47;
            }
            for (lc = 0; lc <= rem; lc++) {
                result[rightlen+lc] = caseinvert(left_op[rightlen+lc]);
            }
            result[leftlen] = '\0'; 
        }
        if (x == 0) { 	
            for (lc = 0; lc < leftlen; lc++) { 
                result[lc] = (left_op[lc] + right_op[lc]) % 47;
            }
            for (lc = 0; lc <= rem; lc++) {
                result[leftlen+lc] = caseinvert(right_op[leftlen+lc]);
            }
            result[rightlen] = '\0';
        }

        //Tranlating result back

        for(i = 0; i < tLen; i++) {
            result[i] = transResult(result[i]);
        }
        result[i+1] = '\0';
    }		 

    //Multiplying	
    //identical to adding, except each integer value is multipled

    if (op == 2) {

        if (x == 2) {
            for (lc = 0; lc < rightlen; lc++) { 
                result[lc] = (left_op[lc] * right_op[lc]) % 47;
            }
            result[rightlen] = '\0';
        }
        if (x == 1) {	
            for (lc = 0; lc < rightlen; lc++) { 
                result[lc] = (left_op[lc] * right_op[lc]) % 47;
            }
            for (lc = 0; lc <= rem; lc++) {
                result[rightlen+lc] = caseinvert(left_op[rightlen+lc]);
            } 
            result[leftlen] = '\0'; 
        } 
        if (x == 0) { 	
            for (lc = 0; lc < leftlen; lc++) { 
                result[lc] = (left_op[lc] * right_op[lc]) % 47;
            }
            for (lc = 0; lc <= rem; lc++) {
                result[leftlen+lc] = caseinvert(right_op[leftlen+lc]);
            }
            result[rightlen] = '\0';
        }

        //Tranlating result back
        for(i = 0; i < tLen; i++) {
            result[i] = transResult(result[i]);
        }
        result[i+1] = '\0';


    }

    // Inversion
    //Tries out a Z value from 0-46 to see if the equation comes out correct
    //when a z is found, it is placed into the result string
    //if the right character is 0, the resulting character is set to 0

    if (op == 3) {
        if (x == 2) {
            for (lc = 0; lc < rightlen; lc++) {
                if (right_op[lc] == 0) {
                    result[lc] = 0;
                }
                for (z = 0; z < 47; z++) { //cycles z from 0 - 46
                    if ((right_op[lc] * z) % 47 == left_op[lc]) { //if the right character * Z mod 47 is equal to the left character, then Z is the answer
                        result[lc] = z;
                    }
                }
            }

            result[rightlen] = '\0';
        }

        if (x == 1) {	
            for (lc = 0; lc < rightlen; lc++) { 
                if (right_op[lc] == 0) {
                    result[lc] = 0;
                }
                for (z = 0; z < 47; z++) {
                    if ((right_op[lc] * z) % 47 == left_op[lc]) {
                        result[lc] = z;
                    }
                }
            }
            for (lc = 0; lc <= rem; lc++) {
                result[rightlen+lc] = caseinvert(left_op[rightlen+lc]);
            }
            result[leftlen] = '\0';
        }

        if (x == 0) { 	
            for (lc = 0; lc < leftlen; lc++) { 
                if (right_op[lc] == 0) {
                    result[lc] = 0;
                }
                for (z = 0; z < 47; z++) {
                    if ((right_op[lc] * z) % 47 == left_op[lc]) {
                        result[lc] = z;
                    }
                }
            }
            for (lc = 0; lc <= rem; lc++) {
                result[leftlen+lc] = caseinvert(right_op[leftlen+lc]);
            }
            result[rightlen] = '\0';
        }

        //translate result
        for(i = 0; i < tLen; i++) {
            result[i] = transResult(result[i]);
        } 
        result[i+1] = '\0';
    }

    //Power
    //the left op is times equalled to a temporary value inside a loop
    //the loop runs until the counter is 1 less than the right op to account for the counter starting at 0
    //the temp value is modded by 47 every loop to avoid buffer overflow

    if (op == 4) {

        if (x == 2) {
            for (lc = 0; lc < rightlen; lc++) {
                powTemp = 1; 
                for (y = 0; y < right_op[lc]; y++) { 

                    powTemp *= left_op[lc];
                    powTemp = powTemp % 47; 
                }

                result[lc] =  powTemp % 47;

                if (right_op[lc] == 1) {
                    result[lc] = left_op[lc];
                }
                if (right_op[lc] == 0) {
                    result[lc] = 1;
                }
            }
            result[rightlen] = '\0';
        }

        if (x == 1) {	
            for (lc = 0; lc < rightlen; lc++) { 
                powTemp = 1;
                for (y = 0; y < right_op[lc]; y++) { 

                    powTemp *= left_op[lc];
                    powTemp = powTemp % 47; 
                }

                result[lc] =  powTemp % 47;

                if (right_op[lc] == 1) {
                    result[lc] = left_op[lc];
                }
                if (right_op[lc] == 0) {
                    result[lc] = 1;
                }
            }
                for (lc = 0; lc <= rem; lc++) {
                    result[rightlen+lc] = caseinvert(left_op[rightlen+lc]);
                }
            
            result[leftlen] = '\0';
        }

        if (x == 0) { 		
            for (lc = 0; lc < rightlen; lc++) {
                powTemp = 1; 
                for (y = 0; y < right_op[lc]; y++) { 

                    powTemp *= left_op[lc];
                    powTemp = powTemp % 47; 
                }

                result[lc] =  powTemp % 47;

                if (right_op[lc] == 1) {
                    result[lc] = left_op[lc];
                }
                if (right_op[lc] == 0) {
                    result[lc] = 1;
                }
            }
             
            for (lc = 0; lc <= rem; lc++) {
                    result[leftlen+lc] = caseinvert(right_op[leftlen+lc]);
                }
        
            result[rightlen] = '\0';

        } 

        //Tranlating result back

        for(i = 0; i < tLen; i++) {
            result[i] = transResult(result[i]);
        }
        result[i+1] = '\0';
    }
}

//calculates string length
int stringlen(const char *op) {
    int len = 0;
    int i;
    for(i=0; op[i] != 0; i++) {
        len++;
    }
    return (len);
}

//inverts the case of the character
int caseinvert(int letter) {
    if (letter >= 0 && letter <= 25) {
        letter = letter + 26;
    }
    else if (letter >= 25 && letter < 47) {
        letter = letter - 26;
    }
    else {
        return letter;
    } 
    return letter;
}

//translates an inputted character
char transResult(char result) {

    if (result < 26 && result >= 0) {
        result = result + 97;
        // i++;
    }
    else if (result > 25 && result < 52) { 	
        result = result + 39;
        // i++;
    }
    else {
        result = result;
    }
    return result;
}
