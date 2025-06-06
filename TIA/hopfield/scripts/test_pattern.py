import numpy as np
import matplotlib.pyplot as plt
import csv
from skimage.util import random_noise

def save_images_with_noise_to_csv(image_path, label_path, output_csv_path, noise_var=0.1):
    images = np.load(image_path)
    labels = np.load(label_path)
    
    if images.shape[0] != labels.shape[0]:
        raise ValueError("Mismatched image and label counts")

    with open(output_csv_path, 'w', newline='') as f:
        writer = csv.writer(f)
        header = ['label'] + [f'pixel{i}' for i in range(1, 785)]
        writer.writerow(header)

        for img, label in zip(images, labels):
            noisy_img = random_noise(img, mode='gaussian', var=noise_var)
            flat_img = (noisy_img * 255).astype(np.uint8).flatten()
            writer.writerow([label] + flat_img.tolist())

def view_image_with_gaussian_noise(image_path: str, label_path: str, 
                                 sample_size=5, noise_var=0.1,
                                 class_names: dict = None):
    """
    Carga y muestra imágenes de test con su versión con ruido gaussiano.

    Args:
        image_path (str): Ruta al archivo .npy que contiene las imágenes.
        label_path (str): Ruta al archivo .npy que contiene las etiquetas.
        sample_size (int): Número de imágenes a mostrar por clase.
        noise_var (float): Varianza del ruido gaussiano (0-1).
        class_names (dict, optional): Diccionario para mapear etiquetas a nombres.
    """
    try:
        print(f"Cargando imágenes desde: {image_path}")
        images = np.load(image_path)
        print(f"Cargando etiquetas desde: {label_path}")
        labels = np.load(label_path)
        
        if images.shape[0] != labels.shape[0]:
            print("Error: El número de imágenes y etiquetas no coincide.")
            return
            
        print(f"El dataset contiene {images.shape[0]} imágenes.")
        
    except FileNotFoundError as e:
        print(f"❌ Error: No se pudo encontrar el archivo. Asegúrate de que la ruta sea correcta.\n{e}")
        return
    except Exception as e:
        print(f"Ocurrió un error al cargar los archivos: {e}")
        return

    unique_labels = np.unique(labels)
    print(f"Se encontraron {len(unique_labels)} clases únicas: {unique_labels}")

    # Configurar la figura
    num_classes = len(unique_labels)
    rows = num_classes
    cols = sample_size * 2  # Original + con ruido
    fig, axes = plt.subplots(rows, cols, figsize=(cols * 1.5, rows * 1.5))
    fig.suptitle(f'Imágenes Originales vs con Ruido Gaussiano (σ²={noise_var})', 
                fontsize=16, y=1.02)

    if num_classes == 1:
        axes = axes.reshape(1, -1)

    for row, label in enumerate(unique_labels):
        # Obtener índices de esta clase
        class_indices = np.where(labels.flatten() == label)[0]
        selected_indices = np.random.choice(class_indices, size=sample_size, replace=False)

        for col, idx in enumerate(selected_indices):
            image = images[idx]
            
            # Normalizar imagen a [0,1] si no lo está
            if image.max() > 1.0:
                image = image.astype(np.float32) / 255.0
            
            # Añadir ruido gaussiano
            noisy_image = random_noise(image, mode='gaussian', var=noise_var)
            
            # Mostrar imagen original
            ax_orig = axes[row, col*2]
            ax_orig.imshow(image.squeeze(), cmap='gray')
            if col == 0:
                display_label = class_names.get(label, label) if class_names else label
                ax_orig.set_ylabel(f"Clase: {display_label}", rotation=0, ha='right', va='center')
            ax_orig.set_title("Original")
            ax_orig.axis('off')
            
            # Mostrar imagen con ruido
            ax_noisy = axes[row, col*2+1]
            ax_noisy.imshow(noisy_image.squeeze(), cmap='gray')
            ax_noisy.set_title("Con ruido")
            ax_noisy.axis('off')

    plt.tight_layout()
    
    plt.show()

if __name__ == '__main__':
    # Configuración de rutas
    files = {
        "train_images": "../data/train_images.npy",
        "train_labels": "../data/train_labels.npy",
        "val_images": "../data/val_images.npy",
        "val_labels": "../data/val_labels.npy",
        "test_images": "../data/test_images.npy",
        "test_labels": "../data/test_labels.npy"
    }
    
    # Ejemplo de nombres de clases (ajustar según tu dataset)
    class_names = {
        0: "T-shirt/top",
        1: "Trouser",
        2: "Pullover",
        3: "Dress",
        4: "Coat",
        5: "Sandal",
        6: "Shirt",
        7: "Sneaker",
        8: "Bag",
        9: "Ankle boot"
    }
    save_images_with_noise_to_csv(
        image_path=files["test_images"],
        label_path=files["test_labels"],
        output_csv_path="../data/test_images_with_noise.csv",
        noise_var=0.05  # Varianza del ruido (ajustable)
    )
    
    # Visualizar imágenes de test con ruido
    print("--- Visualizando imágenes de test con ruido gaussiano ---")
    view_image_with_gaussian_noise(
        image_path=files["test_images"],
        label_path=files["test_labels"],
        sample_size=1,          # 3 imágenes por clase
        noise_var=0.05,         # Varianza del ruido (ajustable)
        class_names=class_names
    )