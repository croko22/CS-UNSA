import sys

def multiplicar_archivo(ruta_archivo_entrada, ruta_archivo_salida="files/big_file.txt", factor_multiplicacion=1000):
    """
    Multiplica el contenido de un archivo varias veces para crear uno más grande.

    Args:
        ruta_archivo_entrada (str): La ruta al archivo de entrada.
        ruta_archivo_salida (str): La ruta del archivo de salida (por defecto: big_file.txt).
        factor_multiplicacion (int): El número de veces que se repetirá el contenido (por defecto: 1000).
    """
    try:
        with open(ruta_archivo_entrada, 'r', encoding='utf-8') as archivo_entrada:
            contenido = archivo_entrada.read()
    except FileNotFoundError:
        print(f"Error: El archivo '{ruta_archivo_entrada}' no fue encontrado.")
        sys.exit(1)
    except Exception as e:
        print(f"Error al leer el archivo de entrada: {e}")
        sys.exit(1)

    try:
        with open(ruta_archivo_salida, 'w', encoding='utf-8') as archivo_salida:
            for _ in range(factor_multiplicacion):
                archivo_salida.write(contenido)
        print(f"Archivo '{ruta_archivo_salida}' creado exitosamente.")
        print(f"El contenido de '{ruta_archivo_entrada}' se repitió {factor_multiplicacion} veces.")
    except Exception as e:
        print(f"Error al escribir en el archivo de salida: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python multiplicar_archivo.py <ruta_al_archivo_de_entrada>")
        sys.exit(1)

    ruta_moby_dick = sys.argv[1]
    # Calculamos un factor de multiplicación aproximado para alcanzar ~20GB
    # 20 GB = 20 * 1024 * 1024 KB
    # 1.2 MB = 1.2 * 1024 KB
    # factor = int((20 * 1024 * 1024) / (1.2 * 1024)) + 1 # 20 GB
    factor = int((20 * 1024 * 1024) / (1.2 * 1024)) + 1 # 6 GB
    print(f"Factor de multiplicación estimado para alcanzar ~20GB: {factor}")
    multiplicar_archivo(ruta_moby_dick, factor_multiplicacion=factor)