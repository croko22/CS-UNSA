# Índice Invertido con OpenMP

Este proyecto genera un índice invertido a partir de archivos de texto. Usa OpenMP para procesar los archivos en paralelo.

## Requisitos

- Compilador C++17
- Soporte para OpenMP
- Sistema Linux, macOS o Windows (con entorno tipo Unix)

## Estructura del Proyecto

```
.
├── README.md
├── Makefile
├── inverted_index.cpp
├── files/
│   ├── archivo1.txt
│   ├── archivo2.txt
│   └── ...
└── output.txt
```

## Uso

### 1. Ejecutar

```bash
make run
```

### 3. Salida

El resultado se guarda en `output.txt`. Muestra cada palabra seguida de los IDs de los archivos donde aparece.

Ejemplo de salida:

```
hola → { 1, 2 }
mundo → { 1, 2, 3 }
```

Los archivos se leen en orden alfabético y sus IDs empiezan desde 1.

## Limpieza

Para eliminar el ejecutable:

```bash
make clean
```

---
