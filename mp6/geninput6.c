/* geninput.c template
 * Richard Coffey
 * rrcoffe        << replace
 * ECE 2220, Fall 2016
 * MP6
 *
 * Purpose: Generate input for MP6
 *
 * Assumptions: Student must complete generation of randomized
 * data in the records as defined in the MP6.pdf specification.
 *
 * Command line argument:
 *    number of items 
 *    seed for random number generator (optional)
 *
 * Output:
 *    binary data to stdout.  The first 4 bytes are the number of 
 *    records, and the remaining binary data are the structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NUMINTS 4
#define NUMLTRS 8
struct Record {
    int id;
    float value;
    int a[NUMINTS];
    char name[NUMLTRS];
};

/* builds a random permutation of the numbers 1 through n.  
 * Use Knuth shuffle to create the random permutation.
 *
 * This function is complete and should not be changed.
 *
 * Output:  a malloced array of ints 
 *    the user must free the array 
 */
int *build_random(int nodes)
{
    int *narray;
    int i, key, temp;
    narray = (int *) malloc(nodes*sizeof(int));
    for (i = 0; i<nodes; i++)
        narray[i] = i+1;
    for (i = 0; i<nodes; i++) {
        key = (int) (drand48() * (nodes - i)) + i;
        assert(i <= key && key < nodes);
        temp = narray[i]; narray[i] = narray[key]; narray[key] = temp;
    }
    return narray;
}

int main(int argc, char *argv[])
{ 
    int num_records = -1;
    int seed = 1477614271;

    if (argc != 2 && argc != 3) { 
        fprintf(stderr, "Usage: geninput num-records [seed]\n");
        exit(1);
    }
    num_records = atoi(argv[1]);
    if (num_records < 2) {
        fprintf(stderr, "geninput: Invalid number records: %d\n", num_records);
        exit(1);
    }
    if (argc == 3) {
        int new_seed = -1;
        new_seed = atoi(argv[2]);
        if (new_seed < 1) {
            fprintf(stderr, "geninput: Invalid seed: %d\n", new_seed);
            exit(1);
        }
        seed = new_seed;
    }
    srand48(seed);

    int i, j, *rand_ids; 
    int rand_val = 0;
    char l;
    struct Record *rec_ptr;
    rec_ptr = (struct Record *) malloc(num_records * sizeof(struct Record));
    rand_ids = build_random(num_records);

    for(i = 0; i < num_records; i++) {
        // sets the id to a random permutation from 1 to num_records
        rec_ptr[i].id = rand_ids[i];

        // you must set value, a, and name here.  These value are WRONG
        //create random value
        rec_ptr[i].value = 1000 * drand48();
        //create random array
        for (j = 0; j < NUMINTS; j++)
            rec_ptr[i].a[j] = 10 * drand48();
        //creates random number of uppercase letters
        rand_val = 7 * drand48 ();
        while (rand_val < 2) {
            rand_val = 7 * drand48 ();
        }
        for (j = 0; j < rand_val; j++) {
            l = 'A' + (25 * drand48());           
            rec_ptr[i].name[j] = l;
        }
        rec_ptr[i].name[rand_val] = '\0';    
    

    }

    fprintf(stderr, "geninput: num records %d\n", num_records);
    fwrite(&num_records, sizeof(num_records), 1, stdout);
    fwrite(rec_ptr, sizeof(struct Record), num_records, stdout);
    free(rec_ptr);
    free(rand_ids);

    return EXIT_SUCCESS;
}
