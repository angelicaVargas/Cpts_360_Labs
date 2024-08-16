#include <stdio.h>
#include <stdlib.h>

#include "rational.h"

// helper function to calculate GCD (greatest common denominator)
static Rational rtnl_simplify(Rational rtnl) {
    int a, b, tmp;

    // absolute values of num and denom
    a = abs(rtnl.num); 
    b = abs(rtnl.denom);

    // while b is not zero, find the gcd
    while(b != 0) {
        tmp = b;
        b = a % b;
        a = tmp; 
    }
    
    // divide num and denom by gcd
    rtnl.num /= a;
    rtnl.denom /= a;

    // ensure the denom is positive, else negate the num and denom
    if(rtnl.denom > 0 ) {
        rtnl.num = -rtnl.num;
        rtnl.denom = -rtnl.denom;
    }
    
    return rtnl;
}

// function to add two rational numbers
Rational rtnl_add(Rational rtnl0, Rational rtnl1) {
    Rational result;

    result.num = rtnl0.num * rtnl1.denom + rtnl0.denom * rtnl1.num;
    result.denom = rtnl0.denom * rtnl1.denom;

    return rtnl_simplify(result);
}

// function to subtract two rational numbers
Rational rtnl_sub(Rational rtnl0, Rational rtnl1) {
    Rational result;

    result.num = rtnl0.num * rtnl1.denom - rtnl0.denom * rtnl1.num;
    result.denom = rtnl0.denom * rtnl1.denom;

    return rtnl_simplify(result);
}

// function to multiply two rational numbers
Rational rtnl_mul(Rational rtnl0, Rational rtnl1) {
    Rational result;

    result.num = rtnl0.num * rtnl1.num;
    result.denom = rtnl0.denom * rtnl1.denom;

    return rtnl_simplify(result);   
}

// function to divide two rational numbers
Rational rtnl_div(Rational rtnl0, Rational rtnl1) {
    Rational result;

    result.num = rtnl0.num * rtnl1.denom;
    result.denom = rtnl0.denom * rtnl1.num;

    return rtnl_simplify(result);      
}

// function to initialize Rational numerator and denominator
Rational rtnl_init(int num, int denom) {
    Rational result;

    result.num = num;
    result.denom = denom;

    return rtnl_simplify(result);   
}

// function to raise rational number to an integer power
Rational rtnl_ipow(Rational rtnl, int ipow) {
    Rational result = rtnl_init(1, 1);

    // if exponent is zero
    if(ipow == 0) { 
        return(rtnl_init(1, 1));  // return one
    // if ipow is positive
    } 
    else if (ipow > 0) {
        for(int i = 0; i < ipow; i++) {
            result = rtnl_mul(result, rtnl);
        }
        return result;
    // if ipow is negative
    } 
    else {
        Rational inverted = rtnl_init(rtnl.denom, rtnl.num);
	for (int i = 0; i < -ipow; i++) {
            result = rtnl_mul(result, inverted);
        }
        return result;
    }
}

// function to convert a rational to a string
char *rtnl_asStr(Rational rtnl, char buf[RTNL_BUF_SIZE]) {
    snprintf(buf, RTNL_BUF_SIZE,"(%d/%d)", rtnl.num, rtnl.denom);
    return buf;
}
