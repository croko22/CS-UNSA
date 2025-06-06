# Hopfield Network Implementation

## 📌 Overview

This project implements a **Hopfield Network**, a type of recurrent artificial neural network that serves as content-addressable memory. The network can store patterns and recall them even when presented with noisy or incomplete versions of those patterns.

## 🧠 Key Features

- **Pattern Storage**: Store multiple binary/bipolar patterns
- **Pattern Recall**: Reconstruct original patterns from noisy inputs
- **Energy Function**: Measure of network stability
- **Save/Load Functionality**: Persist trained networks to disk
- **Fashion-MNIST Support**: Tested with fashion image dataset

## 📂 Project Structure

```
hopfield-network/
├── src/
│   ├── hopfield.cpp       # Core network implementation
│   ├── hopfield.h         # Header file
│   └── main.cpp           # Example usage with Fashion-MNIST
├── utils/
│   └── utils.cpp          # Helper functions (data loading, etc.)
├── data/                  # Dataset directory
├── Makefile               # Build configuration
```

## 🛠️ Build & Run

Compile the project:

```bash
make build
```

Run the program:

```bash
make run
```

## 🧪 Example Usage

```cpp
// Create network
Hopfield network(784); // 28x28 images

// Train with patterns
network.train(patterns);

// Save trained network
network.save("hopfield_network.dat");

// Recall pattern from noisy input
auto reconstructed = network.predict(noisy_input);

// Load saved network
Hopfield loaded_net(784);
loaded_net.load("hopfield_network.dat");
```

## 📊 Metrics

The program generates:
- `hopfield_metrics.csv` - Reconstruction accuracy per class
- `hopfield_network.dat` - Saved network state

## 📚 References

- Original Hopfield Network paper (1982)
- Neural Networks and Deep Learning (Michael Nielsen)
- Fashion-MNIST dataset

## 🚀 Future Work

- Add visualization of pattern reconstruction
- Implement asynchronous updates
- Add support for larger pattern sizes
- Implement pattern removal capability

This implementation provides a solid foundation for experimenting with associative memory networks in C++. The included Fashion-MNIST example demonstrates practical application to image pattern recognition.