
#include "defs.h"
int type_compatiable(int *left, int *right, int onlyright) {
    if (*left == P_VOID || *right == P_VOID) {
        return 0; //false
    }
    if(*left == *right) {
        *left = 0;
        *right = 0;
        return 1; //true
    }

    if (*left ==  P_CHAR && *right == P_INT) {
        *left = A_WIDEN;
        *right = 0;
        return 1;
    }

    if(*left == P_INT && *right == P_CHAR) {
        if (onlyright) {
            return 0;
        }

        *left = 0;
        *right = A_WIDEN;
        return 1;
    }

    return 1; //true
}
