#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    char buffer[8];
    ssize_t n;

    while (1) {
        n = read(STDIN_FILENO, buffer, 8);

        if (n == 0) {
            // EOF normal
            break;
        }

        if (n < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (n != 8) {
            fprintf(stderr, "Error: datos encriptados corruptos (bloque incompleto)\n");
            exit(EXIT_FAILURE);
        }

        // escribir el byte real (último byte)
        ssize_t written = 0;
        while (written < 1) {
            ssize_t w = write(STDOUT_FILENO, &buffer[7] + written, 1 - written);

            if (w < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            written += w;
        }
    }

    return EXIT_SUCCESS;
}