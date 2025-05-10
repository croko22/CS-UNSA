import sys
import requests
import tempfile
import os

def extract_dna_from_genbank(ruta):
    with open(ruta, 'r') as f:
        lines = f.readlines()
    
    secuencia = ''
    en_origen = False

    for line in lines:
        if line.startswith('ORIGIN'):
            en_origen = True
            continue
        if line.startswith('//'):
            break
        if en_origen:
            partes = line.strip().split()
            secuencia += ''.join(partes[1:])

    return secuencia.upper()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python script.py <url_del_genbank>")
        sys.exit(1)

    url = sys.argv[1]
    try:
        respuesta = requests.get(url)
        respuesta.raise_for_status()
    except requests.RequestException as e:
        print("Error al descargar el archivo:", e)
        sys.exit(1)

    with tempfile.NamedTemporaryFile(delete=False, suffix=".gb") as tmp:
        tmp.write(respuesta.content)
        tmp_path = tmp.name

    try:
        adn = extract_dna_from_genbank(tmp_path)
        with open("adn.txt", "w") as archivo_salida:
            archivo_salida.write(adn)
        print("La secuencia de ADN se ha guardado en 'adn.txt'.")
    finally:
        os.remove(tmp_path)