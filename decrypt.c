#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

int main() {
    char bloque_entrada[8];
    while (1) {
        ssize_t n = read(STDIN_FILENO, bloque_entrada, 8);

        if (n == 0) break;
        
        if (n < 0) {
            perror("Fallo en el read.");
            exit(EXIT_FAILURE);
        }

        if (n != 8) {
            fprintf(stderr, "Error, no se leyeron 8 bytes.\n");
            exit(EXIT_FAILURE);
        }
        
        ssize_t escritos = 0;
        while (escritos < 1) {  
            size_t restante = (size_t)(1 - escritos);
            ssize_t r = write(STDOUT_FILENO, &bloque_entrada[7] + escritos, restante);
            if (r < 0) {
                perror("Fallo en el write.");
                exit(EXIT_FAILURE);
            }  

            escritos += r;
        }
    }

    return EXIT_SUCCESS;
}