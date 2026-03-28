#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Cantidad de bytes aleatorios que se agregan antes de cada byte real.
#define RANDOM_PADDING_SIZE 7

// Encripta un byte escribiendo primero RANDOM_PADDING_SIZE bytes aleatorios
// y luego el byte original. Todo se escribe por salida estándar.
static void write_encrypted_byte(char byte) {
    for (int j = 0; j < RANDOM_PADDING_SIZE; j++) {
        char random_byte = (char)(rand() % 256);
        if (write(STDOUT_FILENO, &random_byte, 1) != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (write(STDOUT_FILENO, &byte, 1) != 1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // Inicializa la semilla para que la secuencia pseudo-aleatoria cambie
    // entre ejecuciones.
    srand(time(NULL));

    // Buffer de 1 byte para leer stdin de forma simple y didáctica.
    char buffer[1];
    ssize_t n;

    // Caso 1: si hay argumento, se encripta argv[1] carácter por carácter.
    if (argc > 1) {
        const char *msg = argv[1];
        size_t msg_len = strlen(msg);

        for (size_t i = 0; i < msg_len; i++) {
            write_encrypted_byte(msg[i]);
        }
    }
    // Caso 2: si no hay argumento, se lee desde stdin y se encripta byte a byte.
    else {
        while ((n = read(STDIN_FILENO, buffer, 1)) > 0) {
            write_encrypted_byte(buffer[0]);
        }

        // Si read devuelve negativo, ocurrió un error de lectura.
        if (n < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}