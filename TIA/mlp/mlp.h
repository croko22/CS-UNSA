#include <vector>
#include <functional>
#include <iostream>
#include <cmath>
#include <random>

class MLP
{
private:
    std::vector<std::vector<std::vector<double>>> weights; // Pesos por capa
    std::vector<std::vector<double>> biases;               // Sesgos por capa
    std::function<double(double)> activation;              // Función de activación
    std::function<double(double)> activationDerivative;    // Derivada de la función de activación

public:
    MLP(const std::vector<int> &layers, // Número de neuronas por capa
        std::function<double(double)> activationFunction,
        std::function<double(double)> activationDerivativeFunction)
        : activation(activationFunction), activationDerivative(activationDerivativeFunction)
    {
        // Inicializar pesos y sesgos para cada capa
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);

        for (size_t i = 1; i < layers.size(); ++i)
        {
            int inputSize = layers[i - 1];
            int outputSize = layers[i];

            // Pesos para la capa actual
            weights.push_back(std::vector<std::vector<double>>(outputSize, std::vector<double>(inputSize)));
            for (auto &neuronWeights : weights.back())
            {
                for (double &weight : neuronWeights)
                {
                    weight = dis(gen);
                }
            }

            // Sesgos para la capa actual
            biases.push_back(std::vector<double>(outputSize));
            for (double &bias : biases.back())
            {
                bias = dis(gen);
            }
        }
    }

    std::vector<double> predict(const std::vector<double> &inputs)
    {
        std::vector<double> activations = inputs;

        for (size_t layer = 0; layer < weights.size(); ++layer)
        {
            std::vector<double> newActivations(weights[layer].size());

            for (size_t neuron = 0; neuron < weights[layer].size(); ++neuron)
            {
                double weightedSum = biases[layer][neuron];
                for (size_t input = 0; input < activations.size(); ++input)
                {
                    weightedSum += activations[input] * weights[layer][neuron][input];
                }
                newActivations[neuron] = activation(weightedSum);
            }

            activations = newActivations;
        }

        return activations;
    }

    void train(const std::vector<std::vector<double>> &trainingInputs,
               const std::vector<std::vector<double>> &trainingOutputs,
               double learningRate,
               int epochs)
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (size_t sample = 0; sample < trainingInputs.size(); ++sample)
            {
                // Forward pass
                std::vector<std::vector<double>> layerOutputs = {trainingInputs[sample]};
                for (size_t layer = 0; layer < weights.size(); ++layer)
                {
                    std::vector<double> newActivations(weights[layer].size());
                    for (size_t neuron = 0; neuron < weights[layer].size(); ++neuron)
                    {
                        double weightedSum = biases[layer][neuron];
                        for (size_t input = 0; input < layerOutputs.back().size(); ++input)
                        {
                            weightedSum += layerOutputs.back()[input] * weights[layer][neuron][input];
                        }
                        newActivations[neuron] = activation(weightedSum);
                    }
                    layerOutputs.push_back(newActivations);
                }

                // Backward pass (retropropagación)
                std::vector<std::vector<double>> deltas(weights.size());
                for (int layer = weights.size() - 1; layer >= 0; --layer)
                {
                    deltas[layer] = std::vector<double>(weights[layer].size());
                    for (size_t neuron = 0; neuron < weights[layer].size(); ++neuron)
                    {
                        if (layer == weights.size() - 1)
                        {
                            // Capa de salida
                            double error = trainingOutputs[sample][neuron] - layerOutputs.back()[neuron];
                            deltas[layer][neuron] = error * activationDerivative(layerOutputs.back()[neuron]);
                        }
                        else
                        {
                            // Capas ocultas
                            double error = 0.0;
                            for (size_t nextNeuron = 0; nextNeuron < weights[layer + 1].size(); ++nextNeuron)
                            {
                                error += deltas[layer + 1][nextNeuron] * weights[layer + 1][nextNeuron][neuron];
                            }
                            deltas[layer][neuron] = error * activationDerivative(layerOutputs[layer + 1][neuron]);
                        }
                    }
                }

                // Actualizar pesos y sesgos
                for (size_t layer = 0; layer < weights.size(); ++layer)
                {
                    for (size_t neuron = 0; neuron < weights[layer].size(); ++neuron)
                    {
                        for (size_t input = 0; input < layerOutputs[layer].size(); ++input)
                        {
                            weights[layer][neuron][input] += learningRate * deltas[layer][neuron] * layerOutputs[layer][input];
                        }
                        biases[layer][neuron] += learningRate * deltas[layer][neuron];
                    }
                }
            }
        }
    }
};