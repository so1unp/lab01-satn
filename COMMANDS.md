#  Guía de Comandos - Laboratorio 1

Este documento documenta todos los comandos de `make` disponibles y cómo ejecutar las pruebas del laboratorio.

## 📋 Tabla de Contenidos

- [Comandos Make](#-comandos-make)
- [Compilación](#-compilación)
- [Programas Disponibles](#-programas-disponibles)
- [Pruebas Básicas](#-pruebas-básicas)

---

## �️ Comandos Make

Todos estos comandos se ejecutan desde la raíz del repositorio donde se encuentra el `Makefile`.

### Compilación

```bash
# Compilar TODOS los programas (es el comando por defecto)
make
make all

# Compilar un programa específico
make encrypt    # Crea ./bin/encrypt
make decrypt    # Crea ./bin/decrypt
make democ      # Crea ./bin/democ
```

### Limpieza

```bash
# Eliminar todos los ejecutables compilados
make clean

# Compilar desde cero (limpia y compila todo)
make clean && make all
```

### Documentación

```bash
# Convertir README.md a HTML (para ver en navegador)
make html

# Crear archivo ZIP del código fuente
make zip        # Genera: {username}-lab01.zip
```

### Referencia Rápida

| Comando | Descripción |
|---------|-------------|
| `make` | Compila todos los programas |
| `make all` | Lo mismo que `make` |
| `make encrypt` | Compila solo `encrypt` |
| `make decrypt` | Compila solo `decrypt` |
| `make democ` | Compila solo `democ` |
| `make clean` | Elimina todos los ejecutables |
| `make html` | Genera README.html |
| `make zip` | Crea archivo ZIP |

---

## 🔧 Compilación

### Configuración Actual

El proyecto está configurado con flags robustos de compilación:

```makefile
CC=gcc
CFLAGS=-g -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code
```

- **`-g`**: Incluye símbolos de depuración (para usar con `gdb`)
- **`-Wall`**: Todas las advertencias comunes
- **`-Wextra`**: Advertencias adicionales
- **`-Wshadow`**: Detecta variables con nombres duplicados
- **`-Wconversion`**: Advertencias de conversión de tipos
- **`-Wunreachable-code`**: Detecta código inalcanzable

### Compilación Típica

```bash
# 1. Limpiar ejecutables anteriores
make clean

# 2. Compilar todo
make all

# 3. Verificar que se compiló correctamente
ls -lh ./bin/
```

### Resultado Esperado

```
-rwxr-xr-x 1 user user 19K Mar 28 20:15 decrypt
-rwxr-xr-x 1 user user 19K Mar 28 20:15 democ
-rwxr-xr-x 1 user user 19K Mar 28 20:15 encrypt
```

---

## 📁 Programas Disponibles

### `encrypt` - Encriptador

Encripta un mensaje agregando 7 bytes aleatorios antes de cada byte real.

**Uso:**

```bash
# Opción 1: Mensaje como argumento de línea de comandos
./bin/encrypt "hola"
./bin/encrypt "Hello, World!"
./bin/encrypt "mensaje secreto"

# Opción 2: Mensaje desde entrada estándar (stdin)
echo "hola" | ./bin/encrypt
cat archivo.txt | ./bin/encrypt
```

**Salida:** Bytes encriptados en modo binario (se ve como caracteres no imprimibles)

### `decrypt` - Desencriptador

Desencripta datos que fueron encriptados con `encrypt`. Lee bloques de 8 bytes (7 aleatorios + 1 real).

**Uso:**

```bash
# Siempre lee desde entrada estándar (stdin)
./bin/encrypt "hola" | ./bin/decrypt
cat archivo.enc | ./bin/decrypt
```

**Salida:** Mensaje original desencriptado

### `democ` - Demostración de Conceptos en C

Programa de referencia con ejemplos de conceptos básicos del lenguaje C (estructuras, arrays, funciones, etc.).

**Uso:**

```bash
./bin/democ
```

---

## ✅ Pruebas Básicas

### Test 1: Encriptar y Desencriptar un Mensaje Simple

```bash
echo "hola" | ./bin/encrypt | ./bin/decrypt
```

**Resultado esperado:** `hola`

---

### Test 2: Encriptar desde Argumento

```bash
./bin/encrypt "test"
```

**Resultado esperado:** Caracteres binarios (no imprimibles). Se ve algo como:
```
+.}.%.^h\.+,..[o...
```

Para verlo en formato hexadecimal:

```bash
./bin/encrypt "hola" | xxd
```

**Resultado esperado:**
```
00000000: 2b07 7d09 250e 5e68 5c17 2b2c 0c17 5b6f  +.}.%.^h\.+,..[o
00000010: 1829 0100 5f26 186c 247d 1f15 087b 4061  .).._&.l$}...{@a
```

---

### Test 3: Encriptar Archivo Completo

```bash
# Encriptar el archivo topsecret.txt
./bin/encrypt < topsecret.txt > topsecret.enc

# Desencriptar el archivo
./bin/decrypt < topsecret.enc > recuperado.txt

# Verificar que coinciden
diff topsecret.txt recuperado.txt
```

**Resultado esperado:** Sin salida (archivos idénticos)

---

### Test 4: Pipeline Completo

```bash
# Encriptar contenido de un archivo y desencriptar en el acto
cat topsecret.txt | ./bin/encrypt | ./bin/decrypt
```

**Resultado esperado:** Contenido original de `topsecret.txt`


---

## 📝 Notas Importantes

- ✅ **Mensajes solo ASCII:** El laboratorio funciona con ASCII/UTF-8
- ✅ **Bytes binarios:** La salida de `encrypt` es binaria (no imprimible)
- ✅ **Aleatorios:** Cada encriptación produce bytes aleatorios diferentes
- ✅ **Determinístico (desencriptación):** `decrypt` siempre recupera el mensaje original
- ⚠️ **No seguro:** Este encriptador es un ejercicio educativo, NO es seguro criptográficamente
- ⚠️ **Sin integridad:** No verifica si los datos fueron modificados (no tiene checksum)

---

## 🤝 Contribuir Cambios

Si modificas el Makefile o los programas, recuerda:

```bash
# 1. Probar compilación
make clean && make all

# 2. Probar funcionamiento
echo "test" | ./bin/encrypt | ./bin/decrypt

# 3. Hacer commit
git add -A
git commit -m "descripción de cambios"
git push
```
---
Para guardar la clave si usamos https , se puede usar credential.helper store o si tenemos un token:
```
git remote set-url origin https://MiyoBran:ghp_xxxxx@github.com/usuario/nuevo-repo.git
```
---
---

**Última actualización:** Marzo 28, 2026  
**Plataformas soportadas:** Linux, macOS, Windows (WSL)  
**Compilador:** GCC >= 7.0
