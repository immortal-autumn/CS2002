#include "pell.h"

extern int start1, start2;

int pellcalc(int n) {
    int result = 0;
    int beforeplus = start1, before = start2;

    if (n == 0) {
        return start1;
    }
    if (n == 1) {
        return start2;
    }
    for (int i = 1; i < n; i++) {
        result = 2 * before + beforeplus;
        beforeplus = before;
        before = result;
    }
    return result;
}