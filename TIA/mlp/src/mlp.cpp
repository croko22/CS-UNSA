#include "mlp.h"
#include <cmath>
#include <random>
#include <stdexcept>

double MLP::sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

double MLP::sigmoidDerivative(double x)
{
    double s = sigmoid(x);
    return s * (1 - s);
}

double MLP::leaky_relu(double x, double alpha)
{
    return x > 0 ? x : alpha * x;
}

double MLP::leaky_relu_derivative(double x, double alpha)
{
    return x > 0 ? 1.0 : alpha;
}

double MLP::relu(double x)
{
    return std::max(0.0, x);
}

double MLP::reluDerivative(double x)
{
    return x > 0 ? 1.0 : 0.0;
}

double MLP::tanh(double x)
{
    return std::tanh(x);
}

double MLP::tanhDerivative(double x)
{
    double t = tanh(x);
    return 1 - t * t;
}

MLP::MLP(const std::vector<int> &layers,
         std::function<double(double)> activationFunction,
         std::function<double(double)> activationDerivativeFunction)
    : activation(activationFunction), activationDerivative(activationDerivativeFunction)
{

    if (layers.size() < 2)
    {
        throw std::invalid_argument("MLP requires at least 2 layers (input and output)");
    }

    randomGenerator = std::make_shared<std::mt19937>(std::random_device{}());
    initializeWeights(layers);
}

void MLP::initializeWeights(const std::vector<int> &layers)
{
    weights.clear();
    biases.clear();

    for (size_t i = 1; i < layers.size(); ++i)
    {
        int inputSize = layers[i - 1];
        int outputSize = layers[i];

        double stddev = std::sqrt(2.0 / inputSize);
        std::normal_distribution<> dis(0.0, stddev);

        weights.emplace_back(outputSize, std::vector<double>(inputSize));
        for (auto &neuronWeights : weights.back())
            for (double &weight : neuronWeights)
                weight = dis(*randomGenerator);

        biases.emplace_back(outputSize, 0.0);
    }
}

std::vector<double> MLP::predict(const std::vector<double> &inputs) const
{
    auto layerOutputs = forwardPass(inputs);
    return layerOutputs.back();
}

std::vector<std::vector<double>> MLP::forwardPass(const std::vector<double> &inputs) const
{
    std::vector<std::vector<double>> layerOutputs = {inputs};

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

    return layerOutputs;
}

void MLP::train(const std::vector<std::vector<double>> &trainingInputs,
                const std::vector<std::vector<double>> &trainingOutputs,
                double learningRate,
                int epochs)
{
    if (trainingInputs.size() != trainingOutputs.size())
    {
        throw std::invalid_argument("Training inputs and outputs must have the same size");
    }

    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        for (size_t sample = 0; sample < trainingInputs.size(); ++sample)
        {
            auto layerOutputs = forwardPass(trainingInputs[sample]);
            backwardPass(layerOutputs, trainingOutputs[sample], learningRate);
        }
    }
}

void MLP::backwardPass(const std::vector<std::vector<double>> &layerOutputs,
                       const std::vector<double> &expectedOutput,
                       double learningRate)
{
    std::vector<std::vector<double>> deltas(weights.size());

    for (int layer = weights.size() - 1; layer >= 0; --layer)
    {
        deltas[layer].resize(weights[layer].size());

        for (size_t neuron = 0; neuron < weights[layer].size(); ++neuron)
        {
            if (layer == weights.size() - 1)
            {

                double error = expectedOutput[neuron] - layerOutputs.back()[neuron];
                deltas[layer][neuron] = error * activationDerivative(layerOutputs.back()[neuron]);
            }
            else
            {

                double error = 0.0;
                for (size_t nextNeuron = 0; nextNeuron < weights[layer + 1].size(); ++nextNeuron)
                {
                    error += deltas[layer + 1][nextNeuron] * weights[layer + 1][nextNeuron][neuron];
                }
                deltas[layer][neuron] = error * activationDerivative(layerOutputs[layer + 1][neuron]);
            }
        }
    }

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

void MLP::setActivationFunctions(std::function<double(double)> activationFunction,
                                 std::function<double(double)> activationDerivativeFunction)
{
    activation = activationFunction;
    activationDerivative = activationDerivativeFunction;
}

void MLP::setRandomSeed(unsigned int seed)
{
    randomGenerator = std::make_shared<std::mt19937>(seed);
}