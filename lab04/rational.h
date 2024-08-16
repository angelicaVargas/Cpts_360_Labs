typedef struct {
    int num;
    int denom;
} Rational;

// RTNL_BUF_SIZE is the size of a buffer needed to hold any
// "(num/denom)" string representation of a rational. The buffer must
// hold 2 signed decimals with parentheses and a slash. We allow 4
// digits for each byte of the two values and another 8 bytes for
// additional characters.
enum { RTNL_BUF_SIZE = 2 * 4*sizeof(int) + 8 };

extern Rational rtnl_add(Rational rtnl0, Rational rtnl1);
extern Rational rtnl_sub(Rational rtnl0, Rational rtnl1);
extern Rational rtnl_mul(Rational rtnl0, Rational rtnl1);
extern Rational rtnl_div(Rational rtnl0, Rational rtnl1);
extern Rational rtnl_init(int num, int denom);
extern Rational rtnl_ipow(Rational rtnl, int ipow);

extern char *rtnl_asStr(Rational rtnl, char buf[RTNL_BUF_SIZE]);
