#include "pell.h"

/**
 * Stage 2 will use iltertative function to achieve the same function as recursive function
 */
int pellcalc(int n) {
    int before = 1, beforeplus = 0, result = 0;

    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    for (int i = 1; i < n; i++) {
        result = 2 * before + beforeplus;
        beforeplus = before;
        before = result;
    }
    return result;
}