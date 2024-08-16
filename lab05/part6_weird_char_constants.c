#include <stdio.h>

int main(void)
{
    char *s = "\
";
    // these are legal (but uncouth) C
    printf("%x\n", '//');
    printf("%x\n", '/**/');
    return 0;
}
