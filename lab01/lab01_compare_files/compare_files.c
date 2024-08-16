// It's not a bad practice to list *why* you include particular
// headers.
#include <stdio.h> // for FILE, NULL, fopen(), and getc()

#include "eprintf.h" // for eprintf_fail()

// Although not strictly required, its a good practice to include the
// header file that corresponds to this source file to detect any
// discrepancies between the function's declaration and its
// definition.
#include "compare_files.h"

int compareFiles(char *fname0, char *fname1) {
    // ASSIGNMENT
    //
    // This function compares two files named `fname0` and `fname1`
    // and returns true (1) if they are identical or false (0) if they
    // are not. Here's how it would be described in pseudocode (note
    // the indented block structure):
    
    FILE *file0;
    FILE *file1;
    int ch0, ch1;

    file0 = fopen(fname0, "r");    // open file 0 for reading (hint: fopen(3))
    if(file0 == NULL) {    //  if the open fails,
        printf("Error opening file 1\n");    // exit with an error message
        return 0;
    }

    file1 = fopen(fname1, "r");     // open file 0 for reading (hint: fopen(3))
    if(file1 == NULL) {    //  if the open fails,
        printf("Error opening file 2\n");    // exit with an error message
        return 0;
    }

    do {
        ch0 = getc(file0);    //  read a character `ch0` from file 0 (hint: getc(3))
        ch1 = getc(file1);    //  read a character `ch0` from file 0 (hint: getc(3))

        if (ch0 != ch1) {    // compare both characters to each other and to `EOF`,
            return 0;    //  (possibly) returning 0 or 1
        }

    } while(ch0 != EOF && ch1 != EOF);
}