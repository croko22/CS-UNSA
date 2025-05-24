import numpy as np

files = {
    "train_images": "bloodmnist/train_images.npy",
    "train_labels": "bloodmnist/train_labels.npy",
    "val_images": "bloodmnist/val_images.npy",
    "val_labels": "bloodmnist/val_labels.npy",
    "test_images": "bloodmnist/test_images.npy",
    "test_labels": "bloodmnist/test_labels.npy"
}

for name, path in files.items():
    try:
        data = np.load(path)
        print(f"{name}: {data.dtype}, shape={data.shape}")
        if "images" in name:
            print(f"  min={data.min()}, max={data.max()}, mean={data.mean():.4f}")
        if "labels" in name:
            print(f"  unique labels: {np.unique(data)}")
            print(f"  label counts: {np.bincount(data.flatten())}")
            print(f"  first 10 labels: {data.flatten()[:10]}")
    except Exception as e:
        print(f"{name}: Error al cargar - {e}")