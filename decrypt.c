#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    char entrada;

    char basura;
    while (read(0, &basura, 1)) {
        for (int i = 0; i < 6; i++) {
            read(0, &basura, 1);
        }
        read(0, &entrada, 1);
        write(1, &entrada, 1);
    }

    return 0;
}