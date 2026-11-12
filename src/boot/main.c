/*
 * src/boot/main.c
 * By Reyaansh Mishra
 */

#include <uefi.h>

void main() {
        int x = 0;
        x++;

        while (x != 0) {
                x++;
                printf("%d\n", x);
        };
};
