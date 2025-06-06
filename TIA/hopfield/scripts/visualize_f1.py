import pandas as pd
import matplotlib.pyplot as plt

# Cargar resultados
df = pd.read_csv("../results.csv")

# Mapeo de clases a nombres
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

df['clase'] = df['clase'].map(class_names)

# Graficar F1 Score por clase
plt.figure(figsize=(10, 6))
plt.bar(df['clase'], df['f1_score'], color='skyblue')
plt.xlabel('Clase')
plt.ylabel('F1 Score')
plt.title('F1 Score por clase')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()
plt.show()