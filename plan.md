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