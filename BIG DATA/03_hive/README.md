# Laboratorio de Hive - CS-UNSA

Este laboratorio contiene ejemplos prácticos de uso de Apache Hive para el curso de Big Data en la UNSA. Aquí aprenderás a crear tablas, cargar datos y realizar consultas SQL sobre grandes volúmenes de información.

## Archivos principales

- **wordcount.hql**  
  Realiza un conteo de palabras a partir de un archivo de texto plano, demostrando cómo explotar y agrupar datos en Hive.

- **logs.hql**  
  Procesa logs de usuarios, permitiendo analizar la cantidad de entradas por usuario a partir de archivos de logs tabulados.

- **rank.hql**  
  Une información de visitas y pagerank de páginas web, mostrando cómo realizar joins y agregaciones en Hive.

- **avg_visits.hql**  
  Calcula promedios de visitas por usuario o página (según el contenido del script), ejemplificando funciones de agregación.

## Scripts auxiliares

En la carpeta `scripts/` encontrarás scripts en Python para generar datos de prueba compatibles con los ejemplos de Hive:

- `gen_words.py` — Genera archivos de texto con palabras aleatorias para wordcount.
- `gen_logs.py` — Crea logs de usuarios para logs.hql.
- `gen_rank_logs.py` — Genera archivos de visitas y pagerank para rank.hql.

## Cómo ejecutar los ejemplos

1. **Genera los datos de prueba**  
   Ejecuta los scripts Python en la carpeta `scripts/` para crear los archivos de datos necesarios.

2. **Ejecuta los scripts de Hive**  
   Usa los siguientes comandos en tu terminal:
   ```sh
   hive -f wordcount.hql
   hive -f rank.hql
   hive -f logs.hql
   hive -f avg_visits.hql