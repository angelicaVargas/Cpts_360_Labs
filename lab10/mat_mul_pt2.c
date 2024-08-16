#include <stdlib.h> // for malloc()
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include "a2d.h"
#include "tspec_diff.h"
#include "mat_mul_pt2.h"

/*
 *  Good practice: This structure contains all of the "globals" each
 *  thread can access.
 *
 *  A pointer to a shared instance of this one struct is shared by all
 *  threads used for a single matrix multiply, so any values that
 *  might be modified by more than one thread should not be changed by
 *  any thread without being protected by some kind of mutex, a la
 *  "nextRow".
 */
typedef struct {
    /*
     * `nextRow` is modified by every thread, so it needs a mutex.
     */
    int nextRow; /* the next row to be done *and* # of rows already done */
    pthread_mutex_t nextRowMutex; /* restricts access to `nextRow` */

    /*
     * As far as a thread is concerned, these are constants, so no
     * mutex is necessary.
     */
    int nThreads;
    int n, m, p; /* matrix dimensions */
    const double *_a, *_b; /* input (1D) matrices */

    /*
     * Each row of this matrix is modified by only one thread, so
     * again no mutex is necessary. (Note how this decision, which is
     * critical to good threaded performance, requires knowledge of
     * the algorithm.)
     */
    double *_c; /* output (1D) matrix */
} ThreadGlobals;


static void multiplyRow(double *_c, const int i, const int n, const int m,
                        const double *_a, const int p, const double *_b)
{
#define c(i,j) _c[I2D(i, n, j, m)]
#define a(i,j) _a[I2D(i, n, j, p)]
#define b(i,j) _b[I2D(i, p, j, m)]
    int j, k;
    double sum;

    sum = 0.0;
    for (j = 0; j < m; j++) {
        sum = 0.0;
        for (k = 0; k < p; k++)
            sum += a(i,k) * b(k,j);
        c(i,j) = sum;
    }
#undef a
#undef b
#undef c
}


// inThread -- function for each thread
static void *inThread(void *threadGlobals_)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     */
    int i;

    // allocate a MatMulThreadStats instance `matMulThreadStats` malloc())
    MatMulThreadStats *matMulThreadStats = malloc(sizeof(MatMulThreadStats));

    // set `matMulThreadStats->nRowsDone` to 0
    matMulThreadStats->nRowsDone = 0;
    ThreadGlobals *threadGlobals = (ThreadGlobals *)threadGlobals_;
    struct timespec start_timeCPU, end_timeCPU;
    clockid_t clockid;

    // get this thread's cpu clock id (hint: pthread_self() and pthread_getcpuclockid())
    pthread_getcpuclockid(pthread_self(), &clockid);

    //get the thread clock id's start cpu time (hint: clock_gettime())
    clock_gettime(clockid, &start_timeCPU);

    // loop the following indefinitely,
    while (1) {

        // if there are multiple thread
        if (threadGlobals->nThreads > 0) {
            // lock the "nextRow" mutex (hint: pthread_mutex_lock())
            pthread_mutex_lock(&threadGlobals->nextRowMutex);

            // set `i` to the current value of `threadGlobals->nextRow`
            i = threadGlobals->nextRow++;

            // increment `threadGlobals->nextRow` or i = threadGlobals->nextRow++
            // threadGlobals->nextRow++;
        }

        // if there are multiple threads
        if (threadGlobals->nThreads > 0) {
            // unlock the "nextRow" thread mutex (hint: pthread_mutex_unlock())
            pthread_mutex_unlock(&threadGlobals->nextRowMutex);
        }

        //  if `i` >= `n` (the number of rows)
        if (i >= threadGlobals->n) {
            // break out of the loop
            break;
        }

        // multiply row `i` (hint: multiplyRow())
        multiplyRow(threadGlobals->_c, i, threadGlobals->n, threadGlobals->m, threadGlobals->_a, threadGlobals->p, threadGlobals->_b);

        // increment `matMulThreadStats->nRowsDone`
        matMulThreadStats->nRowsDone++;
    }

    // get the thread clock id's end cpu time (hint: clock_gettime())
    clock_gettime(clockid, &end_timeCPU);

    // set `matMulThreadStats->cpuTime` to the difference of thread start
    // stop cpu times (hint: tspecDiff())
    matMulThreadStats->cpuTime = tspecDiff(start_timeCPU, end_timeCPU);

    // return `matMulThreadStats`
    return matMulThreadStats;
}

void mat_mul(double *_c, const int n, const int m, const double *_a, const int p, const double *_b, const int nThreads, MatMulThreadStats allMatMulThreadStats[])
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     */

    /* declare a ThreadGlobals struct and set all fields to the
     *  corresponding parameters and `nextRow` to 0. (No rows have
     *  been done yet.)
     */
    ThreadGlobals globals = {0, PTHREAD_MUTEX_INITIALIZER, nThreads, n, m, p, _a, _b, _c};

    // if `nThreads` > 0
    if (nThreads > 0) {
        // malloc() an array of `nThreads` pthread_t's
        pthread_t *threads = malloc(nThreads * sizeof(pthread_t));
        assert(threads);

        // initialize the `nextRowMutex` thread mutex (hint: pthread_mutex_init())
        // nextRowMutex = pthread_mutex_init();

        // for each of `nThreads` threads,
        for (int i = 0; i < nThreads; i++) {
            pthread_create(&threads[i], NULL, inThread, &globals);
        }

        // for each thread `i` of `nThreads` threads,
        for (int i = 0; i < nThreads; i++) {

            MatMulThreadStats *retMat;
            // wait for the thread to complete (hint: pthread_join())
            pthread_join(threads[i], (void **)&retMat);

            // copy its MatMulThreadStats return to `allMatMulThreadStats[i]`
            allMatMulThreadStats[i] = *retMat;

            // free the returned struct
            free(retMat);
        }

        // destroy the thread mutex (hint: pthread_mutex_destroy())
        pthread_mutex_destroy(&globals.nextRowMutex); // global.nextrow
        
        // free the pthread_t array
        free(threads);

    } // otherwise
    else
    {
        MatMulThreadStats *retMat;

        // call the thread function directly (hint: inThread())
        retMat = inThread(&globals);

        // copy its MatMulThreadStats return to `allMatMulThreadStats[0]`

        allMatMulThreadStats[0] = *retMat;
        
        // free the returned struct
        free(retMat);
    }
}


