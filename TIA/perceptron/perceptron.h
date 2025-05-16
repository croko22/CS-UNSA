#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <functional>

int stepFunction(double x)
{
    return x >= 0 ? 1 : 0;
}

class Perceptron
{
private:
    std::vector<double> weights;
    std::function<int(double)> activation;
    double bias;

public:
    Perceptron(int inputSize, std::function<int(double)> activationFunction = stepFunction) : activation(activationFunction)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);

        weights.resize(inputSize);
        for (double &weight : weights)
        {
            weight = dis(gen);
        }
        bias = dis(gen);
    }

    int predict(const std::vector<double> &inputs)
    {
        double weightedSum = 0.0;

        for (size_t i = 0; i < inputs.size(); ++i)
        {
            weightedSum += inputs[i] * weights[i];
        }
        weightedSum += bias;

        return activation(weightedSum);
    }

    void train(const std::vector<std::vector<double>> &trainingInputs,
               const std::vector<int> &labels,
               double learningRate,
               int epochs,
               bool verbose = false)
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            if (verbose)
            {
                std::cout << "Epoch " << epoch + 1 << "/" << epochs << std::endl;
            }

            for (size_t i = 0; i < trainingInputs.size(); ++i)
            {
                if (verbose)
                {
                    std::cout << "  Training sample " << i + 1 << ": ";
                    for (double input : trainingInputs[i])
                        std::cout << input << " ";
                    std::cout << "Label: " << labels[i] << std::endl;
                }

                int prediction = predict(trainingInputs[i]);
                int error = labels[i] - prediction;

                if (verbose)
                {
                    std::cout << "    Prediction: " << prediction << ", Error: " << error << std::endl;
                }

                for (size_t j = 0; j < weights.size(); ++j)
                {
                    double oldWeight = weights[j];
                    weights[j] += learningRate * error * trainingInputs[i][j];
                    if (verbose)
                    {
                        std::cout << "    Weight[" << j << "] updated: " << oldWeight << " -> " << weights[j] << std::endl;
                    }
                }

                double oldBias = bias;
                bias += learningRate * error;
                if (verbose)
                {
                    std::cout << "    Bias updated: " << oldBias << " -> " << bias << std::endl;
                }
            }

            if (verbose)
            {
                std::cout << "End of epoch " << epoch + 1 << "\n"
                          << std::endl;
            }
        }
    }

    void printWeightsAndBiases()
    {
        std::cout << "Weights: ";
        for (const double &weight : weights)
        {
            std::cout << weight << " ";
        }
        std::cout << "\nBias: " << bias << std::endl;
    }
};
