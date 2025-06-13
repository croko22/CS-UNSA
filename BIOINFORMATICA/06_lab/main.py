import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import dendrogram

def read_distance_matrix(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    n = len(lines)
    matrix = np.zeros((n + 1, n + 1))

    for i, line in enumerate(lines):
        parts = list(map(float, line.strip().split()))
        for j in range(len(parts)):        
            if j >= i:
                matrix[i][j + 1] = parts[j]
                matrix[j + 1][i] = parts[j]
            else:
                matrix[i + 1][j] = parts[j]
                matrix[j][i + 1] = parts[j]
    
    return matrix

def find_pair(original_matrix, clusters, strategy='single'):
    best = None
    best_val = float('inf') if strategy != 'complete' else -float('inf')

    for i in range(len(clusters)):
        for j in range(i + 1, len(clusters)):
            distances = [original_matrix[a][b] for a in clusters[i] for b in clusters[j]]

            if strategy == 'single':
                val = min(distances)
                if val < best_val:
                    best_val = val
                    best = (i, j)
            elif strategy == 'complete':
                val = max(distances)
                if val > best_val:
                    best_val = val
                    best = (i, j)
            elif strategy == 'average':
                val = sum(distances) / len(distances)
                if val < best_val:
                    best_val = val
                    best = (i, j)
    return best, best_val

def save_steps(history, labels, filename="clustering_steps.txt"):
    with open(filename, 'w') as f:
        for step, (c1, c2, dist) in enumerate(history):
            real_c1 = [labels[i] for i in c1]
            real_c2 = [labels[i] for i in c2]
            f.write(f"Paso {step+1}: Se unen {real_c1} y {real_c2} con distancia {dist:.6f}\n")

def plot_dendrogram(history, labels, title, filename):
    Z = []
    cluster_ids = {tuple([i]): i for i in range(len(labels))}
    current_id = len(labels)

    for c1, c2, dist in history:
        id1 = cluster_ids[tuple(sorted(c1))]
        id2 = cluster_ids[tuple(sorted(c2))]
        Z.append([id1, id2, dist, len(c1) + len(c2)])
        new_cluster = sorted(c1 + c2)
        cluster_ids[tuple(new_cluster)] = current_id
        current_id += 1

    plt.figure(figsize=(10, 6))
    dendrogram(np.array(Z))
    plt.title(title)
    plt.xlabel("Objetos")
    plt.ylabel("Distancia")
    plt.savefig(filename)
    plt.close()

def main():
    if len(sys.argv) != 2:
        print("Uso: python main.py <archivo_matriz.txt>")
        return

    file_path = sys.argv[1]

    try:
        original_matrix = read_distance_matrix(file_path)
    except Exception as e:
        print(f"Error leyendo el archivo: {e}")
        return

    strategies = {
        "single": "Single Linkage",
        "complete": "Complete Linkage",
        "average": "Average Linkage"
    }

    labels = list(range(1, len(original_matrix)+1))

    for strategy, name in strategies.items():
        print(f"\n{'-' * 40}\nEjecutando {name}...\n{'-' * 40}")
        clusters = [[i] for i in range(len(original_matrix))]
        history = []

        while len(clusters) > 1:
            (i, j), distance = find_pair(original_matrix, clusters, strategy=strategy)
            c1 = clusters[i]
            c2 = clusters[j]
            print(f"Uniendo {[labels[idx] for idx in c1]} y {[labels[idx] for idx in c2]} con distancia {distance:.6f}")
            history.append((c1.copy(), c2.copy(), distance))
            clusters[i] = c1 + c2
            clusters.pop(j)

        save_steps(history, labels, f"{strategy}_linkage.txt")
        plot_dendrogram(history, labels, name, f"{strategy}_dendrogram.png")
        print(f"Resultados guardados para {name}")

if __name__ == "__main__":
    main()
