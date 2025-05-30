import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('training_metrics.csv')
plt.figure(figsize=(12, 4))
plt.subplot(1, 2, 1)
plt.plot(df['Epoch'], df['Loss'], label='Training Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(df['Epoch'], df['Accuracy'], label='Train Accuracy')
plt.plot(df['Epoch'], df['TestAccuracy'], label='Test Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.legend()

plt.tight_layout()
plt.show()