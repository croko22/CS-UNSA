#ifdef USE_CUDA
#include "mlp.cu"
#else
#include "mlp.h"
#endif
#include "load_bloodmnist.cpp"
#include <vector>
#include <iostream>
#include <cmath>

// ReLU y su derivada
double relu(double x) { return x > 0 ? x : 0; }
double relu_derivative(double x) { return x > 0 ? 1 : 0; }

// MSE
double mse(const std::vector<double> &y_true, const std::vector<double> &y_pred)
{
    double sum = 0.0;
    for (size_t i = 0; i < y_true.size(); ++i)
        sum += (y_true[i] - y_pred[i]) * (y_true[i] - y_pred[i]);
    return sum / y_true.size();
}

int main()
{
#ifdef USE_CUDA
    std::cout << "Ejecutando con CUDA (GPU)" << std::endl;
#else
    std::cout << "Ejecutando en CPU" << std::endl;
#endif

    std::cout << "Cargando dataset bloodmnist..." << std::endl;
    BloodMnistData data = load_bloodmnist_train("bloodmnist/train_images.npy", "bloodmnist/train_labels.npy");
    std::vector<std::vector<double>> X = data.images;
    std::vector<std::vector<double>> Y(data.labels.size(), std::vector<double>(8, 0.0));

    // One-hot encoding para Y
    for (size_t i = 0; i < data.labels.size(); ++i)
        Y[i][data.labels[i]] = 1.0;

    std::cout << "Inicializando MLP..." << std::endl;

#ifdef USE_CUDA
    MLP_CUDA mlp({2352, 128, 64, 8});
#else
    MLP mlp({2352, 128, 64, 8}, relu, relu_derivative);
#endif

    std::cout << "Entrenando..." << std::endl;
    int epochs = 100;
    for (int epoch = 1; epoch <= epochs; ++epoch)
    {
        mlp.train(X, Y, 0.1, 1); // Entrena 1 epoch por vez
        if (epoch % 10 == 0 || epoch == 1)
            std::cout << "Epoch " << epoch << "/" << epochs << " completado." << std::endl;
    }

    std::cout << "\nDespués de entrenar:\n";
    for (size_t i = 0; i < X.size(); ++i)
    {
        auto pred = mlp.predict(X[i]);
        std::cout << "Entrada: " << X[i][0] << " - Predicción: " << pred[0] << " - Real: " << Y[i][0] << "\n";
    }

    double total_mse = 0.0;
    for (size_t i = 0; i < X.size(); ++i)
        total_mse += mse(Y[i], mlp.predict(X[i]));
    std::cout << "\nMSE final: " << total_mse / X.size() << std::endl;

    return 0;
}