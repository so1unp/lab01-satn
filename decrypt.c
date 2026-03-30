#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ENCRYPTED_BLOCK_SIZE 8
#define PAYLOAD_INDEX 7

static void write_byte_or_exit(int fd, char value) {
    if (write(fd, &value, 1) != 1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    char block[ENCRYPTED_BLOCK_SIZE];

    while (1) {
        size_t total_read = 0;

        // read() en pipes puede devolver lecturas parciales; acumulamos hasta 8 bytes.
        while (total_read < ENCRYPTED_BLOCK_SIZE) {
            ssize_t bytes_read = read(
                STDIN_FILENO,
                block + total_read,
                ENCRYPTED_BLOCK_SIZE - total_read
            );

            if (bytes_read == 0) {
                break;
            }

            if (bytes_read < 0) {
                perror("read");
                return EXIT_FAILURE;
            }

            total_read += (size_t)bytes_read;
        }

        if (total_read == 0) {
            return EXIT_SUCCESS;
        }

        if (total_read != ENCRYPTED_BLOCK_SIZE) {
            fprintf(stderr, "Error: datos encriptados corruptos (bloque incompleto)\n");
            return EXIT_FAILURE;
        }

        write_byte_or_exit(STDOUT_FILENO, block[PAYLOAD_INDEX]);
    }
}
