import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import math

def parse_hopfield_data(filename):
    """
    Lee un archivo de texto con los pesos y patrones de una red de Hopfield.
    """
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
    except FileNotFoundError:
        print(f"Error: No se pudo encontrar el archivo '{filename}'.")
        return None, None

    weights = []
    patterns = []
    
    
    reading_state = None 

    for line in lines:
        line = line.strip()
        if not line:
            continue

        if "Weights:" in line:
            reading_state = 'weights'
            continue
        elif "Trained Patterns:" in line:
            reading_state = 'patterns'
            continue

        if reading_state == 'weights':
            try:
                row = [float(val) for val in line.split()]
                weights.append(row)
            except ValueError:
                
                continue
        
        elif reading_state == 'patterns':
            
            
            parts = line.split()
            if len(parts) > 1:
                try:
                    pattern = [float(val) for val in parts]
                    patterns.append(pattern)
                except ValueError:
                    
                    continue

    if not weights or not patterns:
        print("Error: No se pudieron leer los pesos o los patrones del archivo.")
        return None, None

    return np.array(weights), np.array(patterns)

def visualize_data(weights, patterns):
    """
    Genera las visualizaciones para los pesos y patrones.
    """
    
    print(f"Visualizando la matriz de pesos de tamaño {weights.shape}...")
    plt.figure(figsize=(10, 8))
    sns.heatmap(weights, cmap='coolwarm', center=0)
    plt.title("Mapa de Calor de la Matriz de Pesos")
    plt.xlabel("Neurona j")
    plt.ylabel("Neurona i")
    plt.show()

    
    num_patterns = len(patterns)
    if num_patterns == 0:
        return
        
    print(f"Visualizando {num_patterns} patrones entrenados...")
    
    
    
    
    
    pattern_height = 28
    pattern_width = 26
    
    if patterns[0].shape[0] != pattern_height * pattern_width:
        print(f"ADVERTENCIA: El tamaño del patrón ({patterns[0].shape[0]}) no coincide con las dimensiones {pattern_height}x{pattern_width}.")
        print("Ajusta 'pattern_height' y 'pattern_width' en el código.")
        return

    
    cols = math.ceil(math.sqrt(num_patterns))
    rows = math.ceil(num_patterns / cols)
    
    fig, axes = plt.subplots(rows, cols, figsize=(cols * 2, rows * 2.2))
    axes = axes.flatten()

    for i, ax in enumerate(axes):
        if i < num_patterns:
            pattern_image = patterns[i].reshape(pattern_height, pattern_width)
            ax.imshow(pattern_image, cmap='binary')
            ax.set_title(f"Patrón {i+1}")
            ax.axis('off')
        else:
            ax.axis('off')

    plt.suptitle("Patrones Entrenados", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.show()



if __name__ == "__main__":
    
    filename = '../models/hopfield_model.bin' 
    
    weights_matrix, trained_patterns = parse_hopfield_data(filename)

    if weights_matrix is not None and trained_patterns is not None:
        visualize_data(weights_matrix, trained_patterns)