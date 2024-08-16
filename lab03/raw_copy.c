#include <stdlib.h> /* for malloc and free */
#include <unistd.h> /* for read, write and close */
#include <fcntl.h> /* for O_RDONLY, O_CREAT and O_TRUNC */

#include "syscall_check.h"
#include "allocarray.h"

int main(int argc, char *argv[]) {
    int bufferSize;
    char* buffer;
    int inputFile;
    int outputFile;
    int bytesRead;

    // compute the buffer size from argv[] (hint: atoi(3))
    bufferSize = atoi(argv[1]);

    // allocate a buffer of the given size bytes (hint: malloc(3))
    buffer = (char*) malloc(bufferSize);

    // open the input file read-only (hint: open(2))
    SYSCALL_CHECK(inputFile = open(argv[2], O_RDONLY));
    SYSCALL_CHECK(inputFile);

    // open the output file write-only and with "truncate" and "create"
    // flags set, making sure that permissions permit writing (hint: open(2))
    // outputFile = open(argv[3], O_WRONLY | O_TRUNC, S_IRUSR | O_CREAT | S_IWUSR);
    outputFile = open(argv[3], O_WRONLY | O_TRUNC , S_IRUSR | O_CREAT | S_IWUSR);
    SYSCALL_CHECK(outputFile);

    // read given size bytes from input file and write bytes from buffer to output file
    while((bytesRead = read(inputFile, buffer, bufferSize)) > 0) {
        SYSCALL_CHECK(write(outputFile, buffer, bytesRead));
    }

    // free the buffer (hint: free(3))
    free(buffer);
    
    // close the input and output file (hint: close(2))
    SYSCALL_CHECK(close(inputFile));
    SYSCALL_CHECK(close(outputFile));

    exit(0);
}
