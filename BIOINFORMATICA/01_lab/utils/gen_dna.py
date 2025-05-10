import random

def generar_adn(longitud):
    return ''.join(random.choices('ATCG', k=longitud))

def guardar_en_txt(secuencia, nombre_archivo='adn.txt'):
    with open(nombre_archivo, 'w') as f:
        f.write(secuencia)

if __name__ == "__main__":
    longitud = 100 
    adn = generar_adn(longitud)
    guardar_en_txt(adn)
