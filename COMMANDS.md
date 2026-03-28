# 🔨 Guía de Comandos - Laboratorio 1

Este documento documenta todos los comandos de `make` disponibles y cómo ejecutar las pruebas del laboratorio.

## 📋 Tabla de Contenidos

- [Comandos Make](#-comandos-make)
- [Compilación](#-compilación)
- [Programas Disponibles](#-programas-disponibles)
- [Pruebas Básicas](#-pruebas-básicas)
- [Pruebas Avanzadas](#-pruebas-avanzadas)
- [Solución de Problemas](#-solución-de-problemas)

---

## 🛠️ Comandos Make

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

## 🚀 Pruebas Avanzadas

### Test 5: Múltiples Mensajes en Secuencia

```bash
# Encriptar varios mensajes
(
  ./bin/encrypt "mensaje1"
  ./bin/encrypt "mensaje2"
  ./bin/encrypt "mensaje3"
) | ./bin/decrypt
```

**Resultado esperado:**
```
mensaje1mensaje2mensaje3
```

---

### Test 6: Mensaje Vacío

```bash
# Enviar cadena vacía
echo -n "" | ./bin/encrypt | ./bin/decrypt
```

**Resultado esperado:** (sin salida / línea vacía)

---

### Test 7: Caracteres Especiales

```bash
# Encriptar y desencriptar con caracteres especiales
./bin/encrypt "¡Hola! @#$%^&*()" | ./bin/decrypt
```

**Resultado esperado:** `¡Hola! @#$%^&*()`

---

### Test 8: Comparar Múltiples Ejecuciones

Cada ejecución debe producir diferentes bytes aleatorios pero desencriptar al mismo mensaje:

```bash
# Primera ejecución
echo "prueba" | ./bin/encrypt > salida1.bin

# Segunda ejecución
echo "prueba" | ./bin/encrypt > salida2.bin

# Los archivos binarios son diferentes (bytes aleatorios diferentes)
diff salida1.bin salida2.bin
# Resultado: archivos diferentes (esperado)

# Pero ambos desencriptan al mismo mensaje
./bin/decrypt < salida1.bin
./bin/decrypt < salida2.bin
# Resultado: ambos muestran "prueba"
```

---

### Test 9: Manejo de Errores - Bloque Incompleto

```bash
# Crear un archivo con número incorrecto de bytes
printf "1234567" > corrupted.bin  # 7 bytes en lugar de 8

# Intentar desencriptar
./bin/decrypt < corrupted.bin
```

**Resultado esperado:** Mensaje de error como:
```
Error: datos encriptados corruptos (bloque incompleto)
```

---

## 🔍 Monitoreo y Depuración

### Ver Tamaño de Archivos

```bash
# Verificar tamaño original vs encriptado
stat topsecret.txt topsecret.enc

# El archivo encriptado debe ser ~8 veces más grande
# (8 bytes por cada byte original: 7 aleatorios + 1 real)
```

### Usar `xxd` para Inspeccionar Datos Binarios

```bash
# Ver primeros 16 bytes en hexadecimal
./bin/encrypt "hola" | xxd

# Ver 256 bytes
./bin/encrypt "a" | xxd | head -20
```

### Depuración con `gdb`

```bash
# Compilar con símbolos de depuración (ya incluido con -g)
make clean && make

# Iniciar depurador
gdb ./bin/encrypt

# Dentro de gdb:
(gdb) run "hola"
(gdb) break main
(gdb) next
...
(gdb) quit
```

---

## 🖥️ Multiplataforma

### Linux / macOS / WSL

Todos los comandos anteriores funcionan sin cambios:

```bash
make clean && make all
echo "test" | ./bin/encrypt | ./bin/decrypt
```

### Windows (CMD - No recomendado)

Si usas CMD de Windows:

```cmd
REM Compilar
mingw32-make clean
mingw32-make all

REM O usar WSL (recomendado)
wsl make clean && wsl make all
```

**⚠️ Recomendación:** Usa **WSL terminales integrada en VS Code** (como en tu caso) para mejor compatibilidad.

---

## 🐛 Solución de Problemas

### Error: "make: command not found"

**Solución:**
```bash
# En Debian/Ubuntu
sudo apt-get install build-essential

# En macOS
xcode-select --install

# En WSL (sigue el comando de arriba)
```

### Error: "gcc: command not found"

**Solución:**
```bash
# En Debian/Ubuntu
sudo apt-get install gcc

# En macOS
xcode-select --install
```

### Los ejecutables no funcionan en Windows CMD

**Solución:** Usa WSL. Los ejecutables de Linux no funcionan directamente en Windows CMD. Usa:
- WSL integrada en VS Code
- Git Bash
- Windows Terminal con WSL

### Error: "permission denied"

**Solución:**
```bash
# Los binarios compilados ya tienen permisos (+x)
# Si falta, restaurar:
chmod +x ./bin/encrypt ./bin/decrypt ./bin/democ
```

### Archivo `.enc` corruptos

**Solución:**
```bash
# Eliminar archivos binarios generados
rm -f *.enc recuperado.txt salida.bin

# Recompilar y generar nuevos
make clean && make all
./bin/encrypt < topsecret.txt > topsecret.enc
```

---

## 📊 Ejemplo de Ejecución Completa

```bash
# 1. Compilar
$ make clean && make all
rm -f ./bin/decrypt ./bin/democ ./bin/encrypt
gcc -g -Wall -Wextra ... -o bin/encrypt encrypt.c
gcc -g -Wall -Wextra ... -o bin/decrypt decrypt.c
gcc -g -Wall -Wextra ... -o bin/democ democ.c

# 2. Probar pipeline básico
$ echo "secreto" | ./bin/encrypt | ./bin/decrypt
secreto

# 3. Probar con archivo
$ ./bin/encrypt < topsecret.txt > topsecret.enc
$ ./bin/decrypt < topsecret.enc > recuperado.txt
$ diff topsecret.txt recuperado.txt
$ # Sin salida = archivos idénticos ✅

# 4. Inspeccionar datos binarios
$ ./bin/encrypt "hola" | xxd | head -5
00000000: 2b07 7d09 250e 5e68 5c17 2b2c 0c17 5b6f  +.}.%.^h\.+,..[o
```

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

**Última actualización:** Marzo 28, 2026  
**Plataformas soportadas:** Linux, macOS, Windows (WSL)  
**Compilador:** GCC >= 7.0
