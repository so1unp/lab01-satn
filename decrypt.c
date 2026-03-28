#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    // Cada bloque encriptado tiene 8 bytes:
    // 7 aleatorios + 1 byte real (posición 7).
    char buffer[8];
    ssize_t n;

    while (1) {
        // Acumulamos lectura hasta completar un bloque de 8 bytes.
        size_t total_read = 0;

        while (total_read < sizeof(buffer)) {
            // read puede devolver menos bytes que los pedidos (lectura parcial),
            // por eso seguimos leyendo hasta completar el bloque.
            n = read(STDIN_FILENO, buffer + total_read, sizeof(buffer) - total_read);

            if (n == 0) {
                // EOF: no hay más datos para leer.
                break;
            }

            if (n < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            total_read += (size_t)n;
        }

        if (total_read == 0) {
            // EOF normal justo al inicio de un bloque.
            break;
        }

        // Si no se completó el bloque, la entrada está truncada/corrupta.
        if (total_read != sizeof(buffer)) {
            fprintf(stderr, "Error: datos encriptados corruptos (bloque incompleto)\n");
            exit(EXIT_FAILURE);
        }

        // El byte real siempre es el último byte del bloque (índice 7).
        if (write(STDOUT_FILENO, &buffer[7], 1) != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}