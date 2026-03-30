#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

// Cantidad de bytes aleatorios que se agregan antes de cada byte real.
#define RANDOM_PADDING_SIZE 7
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
        "Uso: ./bin/encrypt [mensaje]\n"
        "     ./bin/encrypt -f archivo_entrada [archivo_salida]\n"
        "     ./bin/encrypt < archivo_entrada > archivo_salida\n\n"
        "Opciones:\n"
        "  -h, --help    Mostrar esta ayuda\n"
        "  -f, --file    Leer desde archivo (si no hay salida, usa nombre por defecto)\n\n"
        "Si se pasa [mensaje], se encripta ese texto.\n"
        "Si no hay mensaje, se lee desde stdin.\n"
        "Salida por defecto en -f: reemplaza extension por .enc.\n";

    write_all_or_exit(STDOUT_FILENO, usage);
}

// Encripta un byte escribiendo primero RANDOM_PADDING_SIZE bytes aleatorios
// y luego el byte original. Todo se escribe por salida estándar.
static void write_encrypted_byte(int output_fd, char byte) {
    for (int padding_idx = 0; padding_idx < RANDOM_PADDING_SIZE; padding_idx++) {
        char random_byte = (char)(rand() % 256);
        if (write(output_fd, &random_byte, 1) != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (write(output_fd, &byte, 1) != 1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

static void encrypt_stream(int input_fd, int output_fd) {
    char buffer[1];
    ssize_t bytes_read;

    while ((bytes_read = read(input_fd, buffer, 1)) > 0) {
        write_encrypted_byte(output_fd, buffer[0]);
    }

    if (bytes_read < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

static char *build_default_encrypt_output(const char *input_path) {
    const char *last_slash = strrchr(input_path, '/');
    const char *name_start = (last_slash == NULL) ? input_path : last_slash + 1;
    const char *last_dot = strrchr(name_start, '.');
    size_t stem_len = (last_dot == NULL) ? strlen(input_path)
                                         : (size_t)(last_dot - input_path);
    const char *suffix = ".enc";
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

    // Inicializa la semilla para que la secuencia pseudo-aleatoria cambie
    // entre ejecuciones.
    srand((unsigned int)time(NULL));

    if (argc > 1 &&
        (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0)) {
        if (argc < 3 || argc > 4) {
            write_all_or_exit(
                STDERR_FILENO,
                "Error: uso invalido de -f. Usa: ./bin/encrypt -f entrada [salida]\n");
            return EXIT_FAILURE;
        }

        int input_fd = open(argv[2], O_RDONLY);
        if (input_fd < 0) {
            perror("open input");
            return EXIT_FAILURE;
        }

        char *default_output = NULL;
        const char *output_path = NULL;
        if (argc == 4) {
            output_path = argv[3];
        } else {
            default_output = build_default_encrypt_output(argv[2]);
            output_path = default_output;
        }

        int output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_FILE_MODE);
        if (output_fd < 0) {
            perror("open output");
            close(input_fd);
            free(default_output);
            return EXIT_FAILURE;
        }

        encrypt_stream(input_fd, output_fd);

        if (close(input_fd) < 0) {
            perror("close input");
            free(default_output);
            return EXIT_FAILURE;
        }

        if (close(output_fd) < 0) {
            perror("close output");
            free(default_output);
            return EXIT_FAILURE;
        }

        free(default_output);

        return EXIT_SUCCESS;
    }

    // Caso 1: si hay argumento, se encripta argv[1] carácter por carácter.
    if (argc > 1) {
        const char *msg = argv[1];
        size_t msg_len = strlen(msg);

        for (size_t i = 0; i < msg_len; i++) {
            write_encrypted_byte(STDOUT_FILENO, msg[i]);
        }
    }
    // Caso 2: si no hay argumento, se lee desde stdin y se encripta byte a byte.
    else {
        encrypt_stream(STDIN_FILENO, STDOUT_FILENO);
    }

    return EXIT_SUCCESS;
}