#include <vector>
#include <random>
#include <iostream>

class Perceptron
{
public:
    Perceptron(int input_size, float learning_rate = 0.01f)
        : weights(input_size + 1, 0.0f), lr(learning_rate)
    {
        std::mt19937 rng(42);
        std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
        for (float &w : weights)
            w = dist(rng);
    }

    int predict(const std::vector<uint8_t> &input) const
    {
        float sum = weights[0]; // bias
        for (size_t i = 0; i < input.size(); ++i)
            sum += weights[i + 1] * (input[i] / 255.0f); // normalize
        return sum >= 0 ? 1 : 0;
    }

    void train(const std::vector<std::vector<uint8_t>> &X,
               const std::vector<uint8_t> &y, int epochs)
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            int errors = 0;
            for (size_t i = 0; i < X.size(); ++i)
            {
                int pred = predict(X[i]);
                int error = y[i] - pred;
                if (error != 0)
                    ++errors;
                weights[0] += lr * error;
                for (size_t j = 0; j < X[i].size(); ++j)
                    weights[j + 1] += lr * error * (X[i][j] / 255.0f);
            }
            std::cout << "Epoch " << epoch + 1 << ": " << errors << " errors\n";
        }
    }

private:
    std::vector<float> weights;
    float lr;
};
