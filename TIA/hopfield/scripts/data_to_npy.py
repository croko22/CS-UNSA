import numpy as np
import pandas as pd
import struct # Necesario para leer el formato .ubyte

def load_mnist_ubyte_images(filepath):
    with open(filepath, 'rb') as f:
        # Lee la magia, el número de imágenes, filas y columnas
        magic, num_images, num_rows, num_cols = struct.unpack(">IIII", f.read(16))
        # Lee todos los píxeles de las imágenes
        images = np.fromfile(f, dtype=np.uint8).reshape(num_images, num_rows, num_cols)
    return images

def load_mnist_ubyte_labels(filepath):
    with open(filepath, 'rb') as f:
        # Lee la magia y el número de ítems (etiquetas)
        magic, num_items = struct.unpack(">II", f.read(8))
        # Lee todas las etiquetas
        labels = np.fromfile(f, dtype=np.uint8)
    return labels

# Cargar las imágenes y etiquetas de entrenamiento
train_images_ubyte_path = '../data/train-images-idx3-ubyte'
train_labels_ubyte_path = '../data/train-labels-idx1-ubyte'

train_images = load_mnist_ubyte_images(train_images_ubyte_path)
train_labels = load_mnist_ubyte_labels(train_labels_ubyte_path)

print(f"Forma de las imágenes de entrenamiento: {train_images.shape}") # Debería ser (60000, 28, 28)
print(f"Forma de las etiquetas de entrenamiento: {train_labels.shape}") # Debería ser (60000,)

# Cargar las imágenes y etiquetas de prueba
test_images_ubyte_path = '../data/t10k-images-idx3-ubyte'
test_labels_ubyte_path = '../data/t10k-labels-idx1-ubyte'

test_images = load_mnist_ubyte_images(test_images_ubyte_path)
test_labels = load_mnist_ubyte_labels(test_labels_ubyte_path)

print(f"Forma de las imágenes de prueba: {test_images.shape}") # Debería ser (10000, 28, 28)
print(f"Forma de las etiquetas de prueba: {test_labels.shape}") # Debería ser (10000,)

# Opcional: Guardar como archivos .npy
np.save('../data/train_images.npy', train_images)
np.save('../data/train_labels.npy', train_labels)
np.save('../data/test_images.npy', test_images)
np.save('../data/test_labels.npy', test_labels)

print("\nArchivos .npy creados con éxito.")