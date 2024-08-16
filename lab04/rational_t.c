#include <stdio.h> // for printf()
#include <stdlib.h> // for exit()

#include "rational.h"


int main(int argc, char *argv[])
{
    int num = 17*258;
    int denom = 64*258;
    Rational rtnl0 = rtnl_init(42, 19);
    Rational rtnl1 = rtnl_init(22, 13);
    char buf0[RTNL_BUF_SIZE], buf1[RTNL_BUF_SIZE], buf2[RTNL_BUF_SIZE];
    int ipow;

    printf("simplification of %d/%d = %s  (should be 17/64)\n",
           num, denom, rtnl_asStr(rtnl_init(num, denom), buf0));
    printf("%s + %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_add(rtnl0, rtnl1), buf2));
    printf("%s - %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_sub(rtnl0, rtnl1), buf2));
    printf("%s * %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_mul(rtnl0, rtnl1), buf2));
    printf("%s / %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_div(rtnl0, rtnl1), buf2));
    printf("warning: integer overflow about to occur\n"
           "  watch for negative signs\n");
    for (ipow = 0; ipow < 10; ipow++) {
        printf("%s**%d = %s\n", rtnl_asStr(rtnl0, buf0), ipow,
               rtnl_asStr(rtnl_ipow(rtnl0, ipow), buf1));
    }
    exit(EXIT_SUCCESS);
}
