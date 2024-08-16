#include <stdio.h>
#include <string.h>
#include "hello.h"

void activate_new_feature() {
	printf("Activating new feature...\n");
	// This either will look blue when printed or just look like a bunch of weird text
	// if your terminal somehow doesn't support ANSI codes.
	printf("\x1b[36mNew Feature activated\x1b[0m\n");
}

int main(int argc, char** argv) {

    char name[100];
    
    printf("Please enter your name: ");
    
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';

    activate_new_feature();
    say_hello(name);
}
