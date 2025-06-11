export PATH=$PATH:/home/kevin/spark/bin
spark-submit inverted_index_generator.py

1. sube los archivos a un rdd
2. `files_rdd.flatMap(lambda x: [(word.lower(), os.path.basename(x[0])) for word in x[1].split()])`
  Para cada tupla `(ruta, contenido)`:
  - `x[1].split()`: Divide el contenido del archivo en palabras.
  - `word.lower()`: Convierte cada palabra a minúsculas.
  - `os.path.basename(x[0])`: Extrae solo el nombre del archivo de la ruta.
  - `flatMap`: Crea una nueva tupla `(palabra, nombre_del_archivo)` para cada palabra y aplana la lista de tuplas.

`.groupByKey()`
  Agrupa las tuplas por la palabra (la clave). El resultado es `(palabra, [lista_de_nombres_de_archivos_donde_aparece])`.

`.mapValues(lambda doc_list: list(set(doc_list)))`
  Aplica una función solo a los valores (la lista de nombres de archivo):
  - `set(doc_list)`: Elimina nombres de archivo duplicados para cada palabra usando un `set`.
  - `list(...)`: Convierte el `set` de vuelta a una lista.
  El resultado final es `(palabra, [lista_única_de_nombres_de_archivos])`.