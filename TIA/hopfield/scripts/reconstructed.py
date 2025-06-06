import numpy as np
import csv
from skimage.util import random_noise
import pandas as pd
import matplotlib.pyplot as plt

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

def load_reconstructed_csv(path):
    df = pd.read_csv(path, header=0, usecols=range(1, 785))
    patterns = df.values.astype(np.float32)
    if np.max(patterns) <= 1.0 and np.min(patterns) >= -1.0:
        patterns = (patterns + 1) / 2.0
    return patterns

def visualize_reconstruction(original_images, noisy_csv_path, reconstructed_csv_path, sample_size=5, class_names=None):
    test_labels = np.load("../data/test_labels.npy")
    noisy_data = load_reconstructed_csv(noisy_csv_path)
    reconstructed_data = load_reconstructed_csv(reconstructed_csv_path)

    unique_labels = np.unique(test_labels)[:5]

    fig, axes = plt.subplots(len(unique_labels), sample_size, figsize=(sample_size * 3, len(unique_labels) * 3))
    fig.suptitle("Comparativa: Original | Ruido | Hopfield", fontsize=16)

    for row, label in enumerate(unique_labels):
        indices = np.where(test_labels == label)[0][:sample_size]
        for col, idx in enumerate(indices):
            ax = axes[row, col]

            original = original_images[idx].reshape(28, 28)
            noisy = noisy_data[idx].reshape(28, 28)
            reconstructed = reconstructed_data[idx].reshape(28, 28)

            combined = np.hstack((original, noisy, reconstructed))

            ax.imshow(combined, cmap='gray')
            ax.axis('off')

            if col == 0:
                display_label = class_names.get(label, label) if class_names else label
                ax.set_title(f"{display_label}", fontsize=10, loc='left', pad=40)

        if row == 0:
            titles = ['Original', 'Ruido', 'Hopfield']
            for i, t in enumerate(titles):
                fig.text(0.1 + i*0.3, 0.92, t, ha='center', fontsize=12)

    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
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

    # Guardar imágenes con ruido
    save_images_with_noise_to_csv(
        image_path="../data/test_images.npy",
        label_path="../data/test_labels.npy",
        output_csv_path="../data/test_images_with_noise.csv",
        noise_var=0.05
    )

    # Visualizar comparación
    test_images = np.load("../data/test_images.npy")

    visualize_reconstruction(
        original_images=test_images,
        noisy_csv_path="../data/test_images_with_noise.csv",
        reconstructed_csv_path="../data/reconstructed_patterns.csv",
        sample_size=3,
        class_names=class_names
    )