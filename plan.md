## Plan: Refactor Clean Code Simple en C

Objetivo: mantener la funcionalidad actual y el cumplimiento de la consigna, mejorando legibilidad con cambios chicos, locales y fáciles de entender para estudiantes.

**Pasos**
1. Fase 1: baseline y criterios de no regresión.
2. Validar baseline con los casos del README: encrypt+decrypt por argumento, por stdin y roundtrip de archivo.
3. Fijar criterio de éxito: mismo comportamiento observable, misma interfaz de uso en terminal, mismo formato de datos.
4. Fase 2: limpieza mínima en decrypt (depende de fase 1).
5. Simplificar la escritura del byte real a una operación directa con control de error claro.
6. Mantener lectura en bloques de 8 y validación de bloque incompleto.
7. Fase 3: limpieza mínima en encrypt (depende de fase 1; paralelizable con fase 2).
8. Extraer un helper pequeño para la lógica repetida de 7 bytes aleatorios + 1 byte real.
9. Reemplazar número mágico 7 por una constante con nombre.
10. Estandarizar éxito/error con EXIT_SUCCESS y EXIT_FAILURE.
11. Mantener flujo simple de lectura byte a byte para no introducir complejidad innecesaria.
12. Fase 4: validación final (depende de fases 2 y 3).
13. Recompilar y verificar que no aparezcan warnings nuevos por el refactor.
14. Repetir pruebas funcionales del README y roundtrip con archivo.
15. Comparar resultado final con baseline para confirmar cero regresiones.

**Archivos relevantes**
- encrypt.c — reducir duplicación y mejorar claridad con helper simple.
- decrypt.c — simplificar escritura del byte real sin cambiar lógica principal.
- README.md — fuente de verdad de la consigna y casos obligatorios.
- Makefile — verificación de compilación y ejecución.

**Verificación**
1. make clean && make all
2. bin/encrypt hola | bin/decrypt debe devolver hola
3. echo -n hola | bin/encrypt | bin/decrypt debe devolver hola
4. bin/encrypt < topsecret.txt | bin/decrypt > recuperado.txt y luego diff topsecret.txt recuperado.txt
5. Verificar factor 8 en tamaño de salida del encriptado para entradas pequeñas
6. Confirmar que no se agregaron mecanismos rebuscados ni nuevos archivos innecesarios

**Decisiones**
- Incluido: legibilidad, consistencia de errores, eliminación de duplicación.
- Excluido: optimizaciones avanzadas, modularización compleja, cambios criptográficos, cambios de interfaz.
- Regla guía: código simple, didáctico y mantenible para nivel de Algoritmos/Programación 1.