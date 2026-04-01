#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ENCRYPTED_BLOCK_SIZE 8

int read_full(int fd, char *buffer,int size){
    int total = 0;
    while (total < size) {
        ssize_t n = read(fd, buffer + total, size - total);
        if (n == 0) return total; // EOF
        if (n < 0) return -1;    // Error
        total += n;
    }
    return total;
}
int main(int argc, char *argv[])
{
    char buffer[ENCRYPTED_BLOCK_SIZE];

    // Agregar código aquí.
    while (1) {
        int total_read = read_full(STDIN_FILENO, buffer, ENCRYPTED_BLOCK_SIZE);

        if (total_read == 0) // EOF fin del archivo
            break;
        if (total_read < 0) { // Error en la lectura
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(total_read != ENCRYPTED_BLOCK_SIZE) { // Lectura de bytes incompleta
            fprintf(stderr,"Error: datos encriptados corruptos\n");
            exit(EXIT_FAILURE);
        }

        if(write(STDOUT_FILENO,&buffer[7],1) < 0 )
            exit(EXIT_FAILURE);
    
    }
    exit(EXIT_SUCCESS);
}
