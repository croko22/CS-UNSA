import numpy as np
import matplotlib.pyplot as plt
import math

def view_one_image_per_class(image_path: str, label_path: str, class_names: dict = None):
    """
    Carga y muestra una imagen por cada clase única encontrada en el dataset.

    Args:
        image_path (str): Ruta al archivo .npy que contiene las imágenes.
        label_path (str): Ruta al archivo .npy que contiene las etiquetas.
        class_names (dict, optional): Un diccionario para mapear etiquetas numéricas a nombres de clase.
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

    
    indices_to_show = []
    for label in unique_labels:
        
        
        first_index_for_label = np.where(labels.flatten() == label)[0][0]
        indices_to_show.append(first_index_for_label)

    
    num_classes = len(unique_labels)
    
    cols = int(math.ceil(math.sqrt(num_classes)))
    rows = int(math.ceil(num_classes / cols))
    
    fig, axes = plt.subplots(rows, cols, figsize=(cols * 2.5, rows * 2.5))
    fig.suptitle(f'Una Muestra por Clase de {image_path.split("/")[-1]}', fontsize=16)
    axes = axes.flatten()

    
    for i, ax in enumerate(axes):
        if i < num_classes:
            idx = indices_to_show[i]
            image = images[idx]
            label = labels[idx].flatten()[0]

            
            if image.ndim == 2 or image.shape[-1] == 1:
                ax.imshow(image.squeeze(), cmap='gray')
            else:
                ax.imshow(image)
            
            
            display_label = class_names.get(label, label) if class_names else label
            ax.set_title(f"Clase: {display_label}")
            ax.axis('off')
        else:
            
            ax.axis('off')

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.show()


if __name__ == '__main__':
    
    files = {
        "train_images": "../data/train_images.npy",
        "train_labels": "../data/train_labels.npy",
        "val_images": "../data/val_images.npy",
        "val_labels": "../data/val_labels.npy",
        "test_images": "../data/test_images.npy",
        "test_labels": "../data/test_labels.npy"
    }
    
    
    dataset_to_view = "train"

    print(f"--- Visualizando una muestra por clase del dataset de '{dataset_to_view}' ---")
    
    image_file_path = files.get(f"{dataset_to_view}_images")
    label_file_path = files.get(f"{dataset_to_view}_labels")

    if image_file_path and label_file_path:
        
        view_one_image_per_class(
            image_path=image_file_path,
            label_path=label_file_path
        )
    else:
        print(f"Error: No se encontraron las rutas para '{dataset_to_view}'. Revisa el diccionario 'files'.")