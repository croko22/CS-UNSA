# PySpark - Indice invertido

Este script genera un índice invertido que asocia cada palabra con los archivos donde aparece.

## Cómo ejecutar

```bash
export PATH=$PATH:/home/kevin/spark/bin
spark-submit inverted_index_generator.py
```

## Lógica

1. **Carga de archivos**
   Se leen como `(ruta, contenido)` y se paralelizan en un RDD.

2. **Mapeo y normalización**

```python
files_rdd.flatMap(
    lambda x: [
        (word.lower(), os.path.basename(x[0]))
        for word in x[1].split()
    ]
)
```

Convierte cada palabra en una tupla `(palabra, nombre_archivo)`.

3. **Agrupamiento y deduplicación**

```python
.groupByKey()
.mapValues(lambda doc_list: list(set(doc_list)))
```

Agrupa por palabra y elimina archivos duplicados.

## Resultado

Un RDD con entradas tipo:

```python
("palabra", ["archivo1.txt", "archivo2.txt"])
```
