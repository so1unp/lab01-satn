#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define RANDOM_PADDING_SIZE 7
#define MAX_NUM_ASCII 255

static void write_random_byte(char byte){
    
    for(int i = 0; i < RANDOM_PADDING_SIZE; i++){
        char random_byte = (char) (rand() % (MAX_NUM_ASCII + 1));
        if(write(STDOUT_FILENO, &random_byte,1) != 1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    if(write(STDOUT_FILENO,&byte,1)!=1){
            perror("write");
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    srand(time(NULL));
    
    if(argc > 0){
        for(int i = 1;i < argc;i++){
            int messageLength = strlen(argv[i]);
            for(int j = 0; j < messageLength;j++)
                write_random_byte(argv[i][j]);
            if(i < argc - 1)
                write_random_byte(' ');
        }
    }
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
