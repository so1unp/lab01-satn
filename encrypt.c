#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    char buffer[1];

    if (argc > 1) {   
        for (int i = 0; argv[1][i] != '\0'; i++) {
            for (int j = 0; j < 7; j++) {
                int basura = rand();
                if(write(STDOUT_FILENO, &basura, 1) != 1) {
                    perror("No se escribió solo 1 byte.");
                    exit(EXIT_FAILURE);
                }
            }
            
            if(write(STDOUT_FILENO, &argv[1][i], 1) != 1) {
                perror("No se escribió solo 1 byte.");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        ssize_t n = read(STDIN_FILENO, buffer, 1);
        if (n < 0) {
            perror("Error al leer.");
            exit(EXIT_FAILURE);
        }
        while (n > 0) {        
            for (int i = 0; i < 7; i++) {
                int basura = rand();
                if(write(STDOUT_FILENO, &basura, 1) != 1) {
                    perror("No se escribió solo 1 byte.");
                    exit(EXIT_FAILURE);
                }
            }
            
            if(write(STDOUT_FILENO, buffer, 1) != 1) {
                perror("No se escribió solo 1 byte.");
                exit(EXIT_FAILURE);
            }
            
            n = read(STDIN_FILENO, buffer, 1);

            if (n < 0) {
                perror("Error al leer.");
                exit(EXIT_FAILURE);
            }  
        }
    }

    return 0;
}