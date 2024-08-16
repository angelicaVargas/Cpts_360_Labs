#include <stdio.h>
#include <stdlib.h>
#include "gen_perms.h"

int level;
enum {
    NOT_DONE = -1
};
int *val;

void recur(int k, int nElems, void (*handlePerm)(int elems[], int nElems, void *userArg), void *userArg)
{
    int i;

    val[k] = level;
    level++;
    if (level == nElems) {
        for (i = 0; i < nElems; i++)
            printf("%d ", val[i]);
        printf("\n");
    }
    for (i = 0; i < nElems; i++)
        if (val[i] == NOT_DONE)
            recur(i, nElems, handlePerm, userArg);
    level--;
    val[k] = NOT_DONE;
}

void genPerms(int nElems, void (*handlePerm)(int elems[], int nElems, void *userArg), void *userArg)
{
    int i;
    level = 0;
    val = (int *)malloc(nElems * sizeof(int));

    for (i = 0; i < nElems; i++)
        val[i] = NOT_DONE;
    for (i = 0; i < nElems; i++)
        recur(i, nElems, handlePerm, userArg);

    free(val);
}
