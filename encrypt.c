#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define RANDOM_PADDING_SIZE 7

static void write_byte_or_exit(int fd, char value) {
    if (write(fd, &value, 1) != 1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

static void encrypt_one_byte(char payload) {
    for (int i = 0; i < RANDOM_PADDING_SIZE; i++) {
        char random_byte = (char)(rand() % 256);
        write_byte_or_exit(STDOUT_FILENO, random_byte);
    }

    write_byte_or_exit(STDOUT_FILENO, payload);
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    if (argc > 2) {
        fprintf(stderr, "Uso: %s [mensaje]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        const char *message = argv[1];
        size_t len = strlen(message);
        for (size_t i = 0; i < len; i++) {
            encrypt_one_byte(message[i]);
        }
        return EXIT_SUCCESS;
    }

    char input_byte;
    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, &input_byte, 1)) > 0) {
        encrypt_one_byte(input_byte);
    }

    if (bytes_read < 0) {
        perror("read");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
