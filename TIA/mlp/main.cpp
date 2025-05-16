#include "mlp.h"
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
    // dataset bloodmnist

    std::vector<std::vector<double>> X = {{0}, {1}};
    std::vector<std::vector<double>> Y = {{0}, {1}};

    // MLP: 1 entrada, 2 ocultas, 1 salida
    MLP mlp({1, 4, 1}, relu, relu_derivative);

    // Antes de entrenar
    std::cout << "Antes de entrenar:\n";
    for (size_t i = 0; i < X.size(); ++i)
    {
        auto pred = mlp.predict(X[i]);
        std::cout << "Entrada: " << X[i][0] << " - Predicción: " << pred[0] << " - Real: " << Y[i][0] << "\n";
    }

    // Entrenamiento
    mlp.train(X, Y, 0.01, 1000);

    // Después de entrenar
    std::cout << "\nDespués de entrenar:\n";
    for (size_t i = 0; i < X.size(); ++i)
    {
        auto pred = mlp.predict(X[i]);
        std::cout << "Entrada: " << X[i][0] << " - Predicción: " << pred[0] << " - Real: " << Y[i][0] << "\n";
    }

    // Calcular MSE
    double total_mse = 0.0;
    for (size_t i = 0; i < X.size(); ++i)
        total_mse += mse(Y[i], mlp.predict(X[i]));
    std::cout << "\nMSE final: " << total_mse / X.size() << std::endl;

    return 0;
}