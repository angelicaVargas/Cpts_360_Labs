#include <stdlib.h>    // for exit()
#include <stdio.h>     // for the usual printf(), etc.
#include <getopt.h>    // for getopt()
/*
 * ASSIGNMENT
 *
 * - "#include" any other necessary headers (as indicated by "man"
 *    pages)
 */

/*
 * Note the new #include
 */
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "eprintf.h"
#include "critical_section.h"

// To get `getopt_long()` to work, you need to provide a static
// (usually) array of `struct option` structures.  There are four
// members to be filled in:

// 1. `name` is a (char *) string containing the "long" option name
// (e.g. "--help" or "--format=pdf").

// 2. `has_arg` has one of these values that describe the
// corresponding option:
enum {
    NO_ARG  = 0, // the option takes no argument
    REQ_ARG = 1, // the option must have an argument
    OPT_ARG = 2  // the option takes an optional argument
};

// 3. The "flag" is an int pointer that determines how the function
// will return its value. If it is NULL, getopt_long() will return
// "val" (the fourth member) as its function return. If it is not
// NULL, getopt_long() will return 0 and set "*flag" to "val".

// 4. "val" is an int which is either a character to denote a "short"
// (e.g. "-h" or "-f pdf") option or 0, to denote an option which does
// not have a "short" form.

// The array is terminated by an entry with a NULL name (first
// element).

static struct option options[] = {
    // elements are:
    // name       has_arg   flag   val
    { "children", OPT_ARG,  NULL,  'c'},
    { "help",     NO_ARG,   NULL,  'h'},
    { "nosync",   NO_ARG,   NULL,  'n'},
    { "pgid",     NO_ARG,   NULL,  'g'},
    { "ppid",     NO_ARG,   NULL,  'p'},
    { NULL }  // end of options table
};

/*
 * These globals are set by command line options. Here, they are set
 * to their default values.
 */
int showPpids = 0;   // show parent process IDs
int showPgids = 0;   // show process group IDs
int synchronize = 1; // synchronize outputs (don't use until Part 3)


enum { IN_PARENT = -1 }; // must be negative
/*
 * In the parent, this value is IN_PARENT. In the children, it's set
 * to the order in which they were spawned, starting at 0.
 */
int siblingIndex = IN_PARENT;


// This is a global count of signals received.
int signalCount = 0;

// print identifying information about the current process to stdout
void writeLog(char message[], const char *fromWithin) {
    /*
     * ASSIGNMENT
     *
     * - Insert your previous writeLog() code here with this
     *   modification: If the global `synchronize` flag is set, call
     *   criticalSection_enter() before the first printf() call and
     *   criticalSection_leave() after the last one.
     */
    char processName[128];
    int colonIndent;

    if(siblingIndex == IN_PARENT) {
        strcpy(processName, "parent");
        colonIndent = 20;
    } else {
        snprintf(processName, 128, "child %d", siblingIndex);
        colonIndent = 30;
    }

    if(synchronize == 1) {
        criticalSection_enter();
    }

     /*
     * - use `colonIndent` to set the indent up to the ":"
     *   using this example, which prints `processName`:
     */
    printf("%*s: %s\n", colonIndent, "process name", processName);
    /*
     * - print the process ID with the label "pid:" (hint: getpid(2)) indented as above
    */
    printf("%*s: %d\n", colonIndent, "process ID", getpid());
    /*
     * - if `showPpids` is true,
     *     + print the parent process ID with the label "ppid:" (hint: getppid(2)) indented as above
     * - if `showPgids` is true,
     *     + print the process group ID with the label "pgid:" (hint: getpgrp(2)) indented as above
    */
    if(showPpids == 0) {
        printf("%*s: %d\n", colonIndent, "parent process ID", getppid());
    }

    if(showPgids == 0) {
        printf("%*s: %d\n", colonIndent, "process group ID", getpgrp());
    }
    /*
     * - print `signalCount` with the label "signalCount:" indented as above (with a "%d" format, of course)
     * - print `message` with the label "message:" indented as above
     * - print `fromWithin` with the label "fromWithin:" indented as above
     * - print a blank line to separate this from other log entries 
     *
     * (Note: The second argument to this function, `fromWithin`, should
     *  always be `__func__` (no quotes, just an identifier `__func__`).)
     */
    printf("%*s: %d\n", colonIndent, "signalCount", signalCount);
    printf("%*s: %s\n", colonIndent, "signalCount", message);
    printf("%*s: %s\n", colonIndent, "signalCount", fromWithin);
    printf("\n");

    if(synchronize == 1) {
        criticalSection_leave();
    }
}

// do everything that's supposed to be done in the child
void inChild(int iSibling) {
    /*
     * ASSIGNMENT
     * - insert your previous inChild() code here unchanged
     */
    char buf[32];

    siblingIndex = iSibling;
    snprintf(buf, sizeof(buf), "Child %d is paused", iSibling);
    writeLog(buf, __func__);

    while (1) {
        pause();
    }
}

// handle signal `sigNum`
void handler(int sigNum) {
    /*
     * ASSIGNMENT
     * - insert your previous handler() code here unchanged
     */
    char message[128];
    signalCount++;

    snprintf(message, sizeof(message), "%d-%s", sigNum, strsignal(sigNum));
    writeLog(message, __func__);
}

// initialize all signals
void initSignals(void) {
    /*
    * ASSIGNMENT
    * - insert your previous initSignals() code here unchanged
    */
        
    char buf[64];
    void (*sig)(int);

    for (int i = 1; i < _NSIG; i++) {
        if (i == SIGTRAP || i == SIGQUIT)
            continue;

        sig = signal(i, handler);

        if (sig == SIG_ERR) {
            snprintf(buf, sizeof(buf), "Error Setting Handler for %d: %s", i, strsignal(i));
            writeLog(buf, __func__);
        }
    }
}

// do everything that's supposed to be done in the parent
void inParent(void) {
    /*
     * ASSIGNMENT
     * - insert your previous inParent() code here unchanged
     */
    writeLog("Parent is waiting for children", __func__);
    /*
     * - as long as there are child processes to wait upon (hint: wait(2)),
     *     + if the child exited normally (hint: WIFEXITED()),
     *         ~ add an entry to the log that the child exited normally and
     *           include its process ID and status (hint: writeLog())
     *     + otherwise,
     *         ~ add an entry to the log that the child exited abnormally
     *           and include its process ID (hint: writeLog())
     */
    int stat;
    pid_t pid;
    pid = wait(&stat);
    char buf[64];

    while (pid >= 0) {
        if (WIFEXITED(stat)) {
            writeLog("Process exited normally", __func__);
        } else {
            snprintf(buf, sizeof(buf), "Process %d exited abnormally", pid);
            writeLog(buf, __func__);
        }
    /*
     *     + if the child was signaled (hint: WIFSIGNALED()),
     *         ~ add an entry to the log that the child was terminated
     *           by a signal and include the signal number and its
     *           string equivalent (hint: strsignal(3) and writeLog())
     */
    if (WIFSIGNALED(stat)) {
        snprintf(buf, sizeof(buf), "process %d exited by signal %d-%s", pid, WTERMSIG(stat), strsignal(WTERMSIG(stat)));
        writeLog(buf, __func__);
    }

    pid = wait(&stat);
    }
     /*
     * - if wait() caused an error because there were no children to wait for,
     *     + add an entry to the log to that effect (hint: writeLog())
     * - otherwise
     *     + add an entry to the log that wait() failed for an unknown
     *       reason (hint: writeLog())
     */
    if (pid == -1 && errno == ECHILD) {
        writeLog("wait Could not find any children", __func__);
    } else {
        writeLog("wait excited for an unknown reason", __func__);
    }
}


static void usage(char *progname)
// issue a usage error message
{
    eprintf("usage: %s [{args}]*\n", progname);
    eprintf("%s\n", " {args} are:");
    eprintf("%s",
        "  -c[{arg}] or --children[={arg}]  fork {arg} children (default: 1)\n"
        "  -g or --pgid                     list process group IDs\n"
        "  -n or --nosync                   turn off synchronization\n"
        "  -p or --ppid                     list parent PIDs (default: no)\n"
        "  -h or --help                     help (this message) and exit\n"
        );
    return;
}

int main(int argc, char **argv)
{
    int ch;
    int nSiblings = 0;
    static char *progname = "**UNSET**";

    /*
     * Parse the command line arguments.
     */
    progname = argv[0];
    for (;;) {
        ch = getopt_long(argc, argv, "c::ghnp", options, NULL);
        if (ch == -1)
            break;

        switch (ch) {

        case 'c':
            if (optarg)
                nSiblings = atoi(optarg);
            else
                nSiblings = 1;
            break;

        case 'g':
            showPgids = 1;
            break;

        case 'h':
            usage(progname);
            exit(0);

        case 'n':
            synchronize = 0;
            break;

        case 'p':
            showPpids = 1;
            break;

        default:
            printf("?? getopt returned character code 0x%02x ??\n", ch);
            exit(1);
        }
    }
    /*
     * ASSIGNMENT
     *
     * - Insert your previous main() code here unchanged, except that
     *   if the global `synchronize` flag is set, add a call to
     *   criticalSection_init() before the initSignals() call.
     */
    if(synchronize == 1) {
        criticalSection_init();
    }
    initSignals();
    int pid;

    if(nSiblings == 0) {
        writeLog("Parent is paused waiting for signal", __func__);
        
        while (1) {
            pause();
        }
    } else {
        for (int iSibling = 0; iSibling < nSiblings; iSibling++) {
            pid = fork();

            if (pid == 0) {
                inChild(iSibling);
            } else {
                char buffer[50];

                snprintf(buffer, 50, "Parent forked a child %d", iSibling);
                writeLog(buffer, __func__);
            }
            inParent();
        }
    }
    return 0;
}
