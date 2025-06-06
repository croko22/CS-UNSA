import numpy as np

files = {
    "train_images": "../data/train_images.npy",
    "train_labels": "../data/train_labels.npy",
    "val_images": "../data/val_images.npy",
    "val_labels": "../data/val_labels.npy",
    "test_images": "../data/test_images.npy",
    "test_labels": "../data/test_labels.npy"
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