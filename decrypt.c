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
            // Convertir explícitamente a size_t para evitar warning de signo
            size_t remaining = (size_t)(1 - written);
            ssize_t w = write(STDOUT_FILENO, &buffer[7] + written, remaining);

            if (w < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            written += w;
        }
    }

    return EXIT_SUCCESS;
}