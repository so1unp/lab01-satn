#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // Convertir time_t a unsigned int para srand
    srand((unsigned int)time(NULL));

    char buffer[1];
    ssize_t n;

    // Caso 1: argumento por línea de comandos
    if (argc > 1) {
        char *msg = argv[1];
        size_t msg_len = strlen(msg);  // Usar size_t para la longitud
        
        for (size_t i = 0; i < msg_len; i++) {
            // 7 bytes aleatorios
            for (int j = 0; j < 7; j++) {
                char r = (char)(rand() % 256);  // Conversión explícita
                if (write(STDOUT_FILENO, &r, 1) != 1) {
                    perror("write");
                    exit(1);
                }
            }

            // byte real
            if (write(STDOUT_FILENO, &msg[i], 1) != 1) {
                perror("write");
                exit(1);
            }
        }
    }
    // Caso 2: stdin
    else {
        while ((n = read(STDIN_FILENO, buffer, 1)) > 0) {
            for (int j = 0; j < 7; j++) {
                char r = (char)(rand() % 256);  // Conversión explícita
                if (write(STDOUT_FILENO, &r, 1) != 1) {
                    perror("write");
                    exit(1);
                }
            }

            if (write(STDOUT_FILENO, buffer, 1) != 1) {
                perror("write");
                exit(1);
            }
        }

        if (n < 0) {
            perror("read");
            exit(1);
        }
    }

    return 0;
}