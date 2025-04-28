# Big Data - Word Count

## Descripción del enfoque

Este proyecto implementa un programa eficiente en C++ para realizar el conteo de palabras (*word count*) en archivos de gran tamaño (multi-gigabyte), utilizando procesamiento paralelo con OpenMP y procesamiento por fragmentos (*chunking*).

## Estrategia general

1. **División del archivo en fragmentos**:  
   El archivo de entrada se procesa en fragmentos de 2 GB, para evitar cargar archivos enormes en memoria.

2. **Procesamiento paralelo dentro de cada fragmento**:  
   Cada fragmento se divide en sub-fragmentos para ser procesados en paralelo usando OpenMP. Cada hilo crea su propio `unordered_map` local para contar palabras y minimizar la contención entre hilos.

3. **Normalización de palabras**:  
   Se convierten los caracteres alfabéticos a minúsculas y se consideran las secuencias de caracteres no alfabéticos como delimitadores de palabras.

4. **Escritura de resultados parciales**:  
   Cada fragmento genera un archivo parcial con el conteo de palabras encontrado.

5. **Fusión de resultados parciales**:  
   Al final, todos los archivos parciales se fusionan para obtener el conteo final de palabras en un solo archivo de salida.

6. **Limpieza**:  
   Se eliminan los archivos parciales tras completar la fusión.

7. **Medición de rendimiento**:  
   Se mide el tiempo total de ejecución utilizando la librería `chrono`.

## Compilación

Se requiere un compilador de C++ que soporte C++17 y OpenMP.

```bash
g++ -std=c++17 -fopenmp wordcount.cpp -o wordcount
```

## Ejecución

```bash
./wordcount <archivo_de_entrada>
```

Esto generará un archivo `final_wordcount.txt` con el conteo de todas las palabras del archivo de entrada.

## Detalles técnicos

- **Manejo de memoria**:  
  Se utiliza `vector<char>` para almacenar fragmentos de archivo en memoria.

- **Sincronización**:  
  Se evita la sincronización entre hilos en tiempo de ejecución agrupando los conteos en mapas locales antes de combinarlos.

- **Paralelismo**:  
  La carga de trabajo se divide entre los hilos proporcionalmente al tamaño del fragmento en bytes.

- **Robustez**:  
  Se maneja la apertura de archivos y los errores básicos (por ejemplo, archivo no encontrado).