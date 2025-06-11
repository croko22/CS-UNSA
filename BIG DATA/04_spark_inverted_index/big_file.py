import sys
import os

def multiplicar_archivo(ruta_archivo_entrada, ruta_archivo_salida, factor_multiplicacion):
    """
    Multiplica el contenido de un archivo varias veces para crear uno más grande.

    Args:
        ruta_archivo_entrada (str): La ruta al archivo de entrada.
        ruta_archivo_salida (str): La ruta del archivo de salida.
        factor_multiplicacion (int): El número de veces que se repetirá el contenido.
    """
    try:
        with open(ruta_archivo_entrada, 'r', encoding='utf-8') as archivo_entrada:
            contenido = archivo_entrada.read()
    except FileNotFoundError:
        print(f"Error: El archivo '{ruta_archivo_entrada}' no fue encontrado.")
        return
    except Exception as e:
        print(f"Error al leer el archivo de entrada: {e}")
        return

    try:
        with open(ruta_archivo_salida, 'w', encoding='utf-8') as archivo_salida:
            for _ in range(factor_multiplicacion):
                archivo_salida.write(contenido)
        print(f"Archivo '{ruta_archivo_salida}' creado exitosamente.")
        print(f"El contenido de '{ruta_archivo_entrada}' se repitió {factor_multiplicacion} veces.")
    except Exception as e:
        print(f"Error al escribir en el archivo de salida: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python big_file.py <ruta_de_la_carpeta>")
        sys.exit(1)

    ruta_carpeta = sys.argv[1]

    if not os.path.isdir(ruta_carpeta):
        print(f"Error: La ruta '{ruta_carpeta}' no es una carpeta válida.")
        sys.exit(1)

    for archivo in os.listdir(ruta_carpeta):
        ruta_archivo_entrada = os.path.join(ruta_carpeta, archivo)
        if os.path.isfile(ruta_archivo_entrada):
            # Calculamos el tamaño del archivo
            tamano_archivo = os.path.getsize(ruta_archivo_entrada)  # Tamaño en bytes
            tamano_deseado = 5 * 1024 * 1024 * 1024  # 2 GB en bytes
            factor = max(1, tamano_deseado // tamano_archivo)  # Factor de multiplicación

            ruta_archivo_salida = os.path.join(ruta_carpeta, f"big_{archivo}")
            print(f"Procesando archivo: {archivo}")
            print(f"Factor de multiplicación estimado: {factor}")
            multiplicar_archivo(ruta_archivo_entrada, ruta_archivo_salida, factor)