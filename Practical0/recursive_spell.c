#include "pell.h"

/*
    The nth pell number: 2 * (n-1) + (n-2)
    The 1st pell number: 0
    The 2nd pell number: 1
    The 3rd pell number: 2*1+0
    The 4th pell number: 2*(2*1+0)+1
    The 5th pell number: 2*(2*(2*1+0))+2*1+0
    ......
    The nth pell bumber: 2*(2*(2*...(2*1+0))) + 2*(2*(2*...(2*1+0)))
*/

int pellcalc(int n) {
    if(n == 0) {
        return 0;
    }
    if(n == 1) {
        return 1;
    }
    return 2 * pellcalc(n-1) + pellcalc(n-2);
}