# 📋 Buenas Prácticas en C — Ayudamemoria

## 📁 Organización del código

- Funciones auxiliares usadas **solo por un archivo** → van en ese mismo `.c`
- Funciones **compartidas** entre varios archivos → archivo separado `utils.c` + `utils.h`
- Estructura recomendada de un `.c`:
  1. Comentario de cabecera
  2. `#include`s
  3. `#define`s y constantes
  4. Prototipos de funciones auxiliares
  5. `main()`
  6. Definición de funciones auxiliares

---

## 💬 Comentarios

- Comentá el **por qué**, no el **qué**
- El código bien escrito ya dice qué hace
- **Sí comentar:**
  - Cabecera del archivo (qué hace, autor, fecha)
  - Antes de cada función (parámetros, retorno, propósito)
  - Lógica no obvia o decisiones de diseño
- **No comentar:**
  - Lo que ya es obvio leyendo el código (`i++; // incrementa i`)

```c
// MAL
i = i + 1; // incrementa i

// BIEN
// Cada byte real va precedido por 7 bytes aleatorios (protocolo de encriptación)
write(STDOUT_FILENO, randomBytes, RANDOM_PADDING);
```

---

## 🏷️ Nomenclatura

| Elemento | Convención C clásica | Alternativa aceptable |
|----------|---------------------|-----------------------|
| Variables y funciones | `snake_case` | `camelCase` |
| Constantes y macros | `UPPER_CASE` | `UPPER_CASE` |

> ⚠️ Lo más importante es ser **consistente** en todo el proyecto.

---

## ❌ Sin números mágicos

```c
// MAL
write(STDOUT_FILENO, randomBytes, 7);

// BIEN
#define RANDOM_PADDING 7
write(STDOUT_FILENO, randomBytes, RANDOM_PADDING);
```

---

## ✅ Manejo de errores — siempre verificar retornos

```c
// MAL
read(STDIN_FILENO, buffer, 1);

// BIEN
int bytesRead = read(STDIN_FILENO, buffer, 1);
if (bytesRead < 0) {
    perror("read");
    exit(EXIT_FAILURE);
}
```

- Usar siempre `perror()` para errores de syscalls y funciones de biblioteca
- Usar `EXIT_SUCCESS` y `EXIT_FAILURE` en lugar de `0` y `1`

---

## 📦 Includes — solo los necesarios, en orden

```c
#include <stdio.h>      // perror
#include <stdlib.h>     // rand, srand, exit
#include <unistd.h>     // read, write, STDIN_FILENO, STDOUT_FILENO
#include <time.h>       // time (para srand)
```

---

## 🔧 Otras reglas importantes

- Siempre **inicializá las variables** al declararlas
- Declarar variables **al principio del bloque** (buena práctica en C89/C90)
- Siempre incluir `return 0;` al final del `main()`
- Verificar **todos** los retornos de `read()` y `write()` (pueden devolver menos bytes de los pedidos)
- Usar `srand(time(NULL))` para inicializar la semilla aleatoria **una sola vez** al inicio del programa

---

## 🔁 Ejemplo de cabecera de archivo

```c
/*
 * encrypt.c
 * Encripta un mensaje precediendo cada byte con 7 bytes aleatorios.
 * Autor: MiyoBran
 * Fecha: 2026
 */
```

---

## 🔁 Ejemplo de cabecera de función

```c
/*
 * encryptByte - escribe un byte encriptado en stdout
 * @byte: el byte a encriptar
 * Retorna: 0 en éxito, -1 en error
 */
```

---

## 🧭 Mejoras de Refactor para CLI de `encrypt`/`decrypt` (viable)

Estas mejoras son viables sin romper el enunciado de [README.md](README.md),
manteniendo el uso de `read()`, `write()`, `rand()`, `srand()` y `perror()`.

- [x] Agregar flags en `encrypt` y `decrypt` para modo archivo:
  - `-f` o `--file`: indica entrada/salida por archivos.
  - Implementar parsing robusto de argumentos con validaciones y errores claros.
  - Mantener compatibilidad con el comportamiento actual por `stdin`/`stdout`.
- [x] Definir reglas de nombres por defecto para salida:
  - Si no se especifica archivo de salida, generar nombre automáticamente a partir
    del archivo de entrada.
  - En modo `-f`, primer parámetro: archivo de entrada; segundo parámetro
    opcional: archivo de salida.
  - Ejemplo sugerido:
    - `encrypt -f input.txt` -> `input.enc`
    - `decrypt -f test-archivo.enc` -> `test-archivo.txt`
- [x] Agregar modos explícitos de salida para `decrypt`:
  - `-t` o `--terminal`: mostrar resultado por terminal (`stdout`).
  - `-w` o `--write`: escribir resultado a archivo.
  - Casos esperados:
    - `bin/decrypt -f -t test-archivo.enc`
    - `bin/decrypt -f -w test-archivo.enc test-decrypt.txt`
    - Si falta salida en `-w`, usar nombre por defecto derivado.
- [x] Incorporar mensajes de error y ayuda de uso:
  - Validar cantidad y combinación de argumentos por flag.
  - Si `-f` recibe argumentos inválidos o insuficientes, mostrar error de uso.
  - Usar mensajes consistentes y orientados a corrección inmediata.
- [x] Agregar `-h` y `--help` en ambos binarios:
  - Incluir descripción breve, sintaxis y ejemplos de uso.
  - Documentar comportamiento por defecto y en modo archivo.

### Proceso de Refactoring (resumen)

1. Se priorizó mantener compatibilidad con el flujo original por `stdin`/`stdout`.
2. Se incorporó ayuda (`-h`, `--help`) primero, para hacer visible el contrato CLI antes de ampliar opciones.
3. Se agregó modo archivo (`-f`) en ambos binarios con validaciones simples y mensajes de error claros.
4. Se añadieron reglas de salida por defecto para reducir fricción de uso (`.enc` en `encrypt`, `.txt` en `decrypt`).
5. En `decrypt` se agregaron modos explícitos de salida (`-t` y `-w`) para distinguir terminal vs archivo.
6. Se evitó extraer utilidades compartidas a `utils.h/.c` en esta etapa para no modificar `Makefile`, y se documentó la duplicación intencional.

## 📚 Documentación Final (CLI actual)

### `encrypt`

```bash
# Ayuda
./bin/encrypt -h
./bin/encrypt --help

# Encriptar mensaje literal (sale por stdout)
./bin/encrypt "hola"

# Encriptar desde stdin (sale por stdout)
echo -n "hola" | ./bin/encrypt

# Modo archivo con salida por defecto (.enc)
./bin/encrypt -f topsecret.txt
# genera: topsecret.enc

# Modo archivo con salida explícita
./bin/encrypt -f topsecret.txt test-archivo.enc
```

### `decrypt`

```bash
# Ayuda
./bin/decrypt -h
./bin/decrypt --help

# Desencriptar desde stdin (flujo original)
./bin/decrypt < topsecret.enc

# Modo archivo con salida por defecto (.txt)
./bin/decrypt -f test-archivo.enc
# genera: test-archivo.txt

# Modo archivo, mostrar por terminal
./bin/decrypt -f -t test-archivo.enc

# Modo archivo, escribir por defecto
./bin/decrypt -f -w test-archivo.enc

# Modo archivo, escribir en salida explícita
./bin/decrypt -f -w test-archivo.enc test-decrypt.txt
```

### Pruebas rápidas recomendadas

```bash
# 1) Compilar
make clean && make all

# 2) Round-trip básico por pipe
echo -n "hola" | ./bin/encrypt | ./bin/decrypt

# 3) Round-trip por archivos con nombres explícitos
./bin/encrypt -f topsecret.txt prueba.enc
./bin/decrypt -f -w prueba.enc recuperado.txt
diff topsecret.txt recuperado.txt

# 4) Ver salida en terminal desde archivo encriptado
./bin/decrypt -f -t prueba.enc
```