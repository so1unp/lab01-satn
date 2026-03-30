#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define ENCRYPTED_BLOCK_SIZE 8
#define PAYLOAD_BYTE_INDEX 7
#define OUTPUT_FILE_MODE 0644

// Nota: esta funcion esta duplicada intencionalmente en encrypt/decrypt.
// No se movio a utils.h/utils.c para evitar cambios en Makefile en esta etapa.
static void write_all_or_exit(int fd, const char *text) {
    size_t total = 0;
    size_t len = strlen(text);

    while (total < len) {
        ssize_t written = write(fd, text + total, len - total);
        if (written < 0) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        total += (size_t)written;
    }
}

static void print_usage(void) {
    const char *usage =
        "Uso: ./bin/decrypt\n"
        "     ./bin/decrypt -f [-t|-w] archivo_entrada [archivo_salida]\n"
        "     ./bin/decrypt < archivo_encriptado\n\n"
        "Opciones:\n"
        "  -h, --help    Mostrar esta ayuda\n"
        "  -f, --file    Leer desde archivo\n"
        "  -t, --terminal Mostrar salida por terminal (stdout)\n"
        "  -w, --write   Escribir salida a archivo\n\n"
        "Lee datos encriptados y escribe el mensaje desencriptado.\n"
        "Salida por defecto en -f: reemplaza extension por .txt.\n";

    write_all_or_exit(STDOUT_FILENO, usage);
}

static void decrypt_stream(int input_fd, int output_fd) {
    char block[ENCRYPTED_BLOCK_SIZE];

    while (1) {
        size_t total_read = 0;

        while (total_read < sizeof(block)) {
            ssize_t bytes_read =
                read(input_fd, block + total_read, sizeof(block) - total_read);

            if (bytes_read == 0) {
                break;
            }

            if (bytes_read < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            total_read += (size_t)bytes_read;
        }

        if (total_read == 0) {
            break;
        }

        if (total_read != sizeof(block)) {
            write_all_or_exit(
                STDERR_FILENO,
                "Error: datos encriptados corruptos (bloque incompleto)\n");
            exit(EXIT_FAILURE);
        }

        if (write(output_fd, &block[PAYLOAD_BYTE_INDEX], 1) != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
}

static char *build_default_decrypt_output(const char *input_path) {
    const char *last_slash = strrchr(input_path, '/');
    const char *name_start = (last_slash == NULL) ? input_path : last_slash + 1;
    const char *last_dot = strrchr(name_start, '.');
    size_t stem_len = (last_dot == NULL) ? strlen(input_path)
                                         : (size_t)(last_dot - input_path);
    const char *suffix = ".txt";
    size_t output_len = stem_len + strlen(suffix);

    char *output_path = (char *)malloc(output_len + 1);
    if (output_path == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memcpy(output_path, input_path, stem_len);
    memcpy(output_path + stem_len, suffix, strlen(suffix));
    output_path[output_len] = '\0';
    return output_path;
}

int main(int argc, char *argv[]) {
    if (argc > 1 &&
        (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage();
        return EXIT_SUCCESS;
    }

    if (argc > 1 &&
        (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0)) {
        if (argc < 3 || argc > 5) {
            write_all_or_exit(
                STDERR_FILENO,
                "Error: uso invalido de -f. Usa: ./bin/decrypt -f [-t|-w] entrada [salida]\n");
            return EXIT_FAILURE;
        }

        int arg_index = 2;
        int force_terminal = 0;
        int force_write = 0;

        if (arg_index < argc &&
            (strcmp(argv[arg_index], "-t") == 0 ||
             strcmp(argv[arg_index], "--terminal") == 0)) {
            force_terminal = 1;
            arg_index++;
        } else if (arg_index < argc &&
                   (strcmp(argv[arg_index], "-w") == 0 ||
                    strcmp(argv[arg_index], "--write") == 0)) {
            force_write = 1;
            arg_index++;
        }

        if (arg_index >= argc) {
            write_all_or_exit(
                STDERR_FILENO,
                "Error: falta archivo de entrada. Usa: ./bin/decrypt -f [-t|-w] entrada [salida]\n");
            return EXIT_FAILURE;
        }

        const char *input_path = argv[arg_index];
        arg_index++;

        if (arg_index < argc && force_terminal) {
            write_all_or_exit(
                STDERR_FILENO,
                "Error: -t/--terminal no acepta archivo de salida.\n");
            return EXIT_FAILURE;
        }

        if (arg_index + 1 < argc) {
            write_all_or_exit(
                STDERR_FILENO,
                "Error: demasiados argumentos para -f.\n");
            return EXIT_FAILURE;
        }

        int input_fd = open(input_path, O_RDONLY);
        if (input_fd < 0) {
            perror("open input");
            return EXIT_FAILURE;
        }

        char *default_output = NULL;
        const char *output_path = NULL;

        int output_fd = STDOUT_FILENO;
        if (force_terminal) {
            output_fd = STDOUT_FILENO;
        } else if (arg_index < argc) {
            output_path = argv[arg_index];
            output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_FILE_MODE);
            if (output_fd < 0) {
                perror("open output");
                close(input_fd);
                return EXIT_FAILURE;
            }
        } else if (force_write || !force_terminal) {
            default_output = build_default_decrypt_output(input_path);
            output_path = default_output;
            output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_FILE_MODE);
            if (output_fd < 0) {
                perror("open output");
                close(input_fd);
                free(default_output);
                return EXIT_FAILURE;
            }
        } else {
            output_fd = STDOUT_FILENO;
        }

        decrypt_stream(input_fd, output_fd);

        if (close(input_fd) < 0) {
            perror("close input");
            free(default_output);
            return EXIT_FAILURE;
        }

        if (output_fd != STDOUT_FILENO && close(output_fd) < 0) {
            perror("close output");
            free(default_output);
            return EXIT_FAILURE;
        }

        free(default_output);

        return EXIT_SUCCESS;
    }

    if (argc > 1) {
        write_all_or_exit(STDERR_FILENO,
                          "Error: argumento invalido. Usa -h, --help o -f.\n");
        return EXIT_FAILURE;
    }

    decrypt_stream(STDIN_FILENO, STDOUT_FILENO);

    return EXIT_SUCCESS;
}