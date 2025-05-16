#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "perceptron.h"

using namespace std;

void loadTrainingData(const std::string &filename,
                      std::vector<std::vector<double>> &trainingInputs,
                      std::vector<int> &labels)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::vector<double> inputs;
        std::string value;

        while (std::getline(iss, value, ','))
        {
            inputs.push_back(std::stod(value));
        }

        if (inputs.empty())
        {
            throw std::runtime_error("Invalid line in file: " + line);
        }

        int label = static_cast<int>(inputs.back());
        inputs.pop_back();

        trainingInputs.push_back(inputs);
        labels.push_back(label);
    }

    file.close();
}

void normalizeData(std::vector<std::vector<double>> &trainingInputs)
{
    for (size_t j = 0; j < trainingInputs[0].size(); ++j)
    {
        double minVal = trainingInputs[0][j];
        double maxVal = trainingInputs[0][j];

        // Encontrar el mínimo y máximo de cada columna
        for (size_t i = 0; i < trainingInputs.size(); ++i)
        {
            if (trainingInputs[i][j] < minVal)
                minVal = trainingInputs[i][j];
            if (trainingInputs[i][j] > maxVal)
                maxVal = trainingInputs[i][j];
        }

        // Normalizar cada valor en la columna
        for (size_t i = 0; i < trainingInputs.size(); ++i)
        {
            trainingInputs[i][j] = (trainingInputs[i][j] - minVal) / (maxVal - minVal);
        }
    }
}

void testAND()
{
    Perceptron perceptron(2);

    std::vector<std::vector<double>> trainingInputs = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}};

    std::vector<int> labels = {0, 0, 0, 1}; // AND

    std::cout << "Initial weights and bias:" << std::endl;
    perceptron.printWeightsAndBiases();

    std::cout << "Testing AND function:" << std::endl;
    for (const auto &inputs : trainingInputs)
    {
        std::cout << inputs[0] << ", " << inputs[1] << " -> "
                  << perceptron.predict(inputs) << std::endl;
    }

    std::cout << "Training perceptron..." << std::endl;
    perceptron.train(trainingInputs, labels, 0.1, 10);
    std::cout << "Weights and bias after training:" << std::endl;
    perceptron.printWeightsAndBiases();
    std::cout << "Testing AND function after training:" << std::endl;
    for (const auto &inputs : trainingInputs)
    {
        std::cout << inputs[0] << ", " << inputs[1] << " -> "
                  << perceptron.predict(inputs) << std::endl;
    }
}

void testOR()
{
    Perceptron perceptron(2);

    std::vector<std::vector<double>> trainingInputs = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}};

    std::vector<int> labels = {0, 1, 1, 1}; // OR

    std::cout << "Initial weights and bias:" << std::endl;
    perceptron.printWeightsAndBiases();

    std::cout << "Testing OR function:" << std::endl;
    for (const auto &inputs : trainingInputs)
    {
        std::cout << inputs[0] << ", " << inputs[1] << " -> "
                  << perceptron.predict(inputs) << std::endl;
    }

    std::cout << "Training perceptron..." << std::endl;
    perceptron.train(trainingInputs, labels, 0.1, 10);
    std::cout << "Weights and bias after training:" << std::endl;
    perceptron.printWeightsAndBiases();
    std::cout << "Testing AND function after training:" << std::endl;
    for (const auto &inputs : trainingInputs)
    {
        std::cout << inputs[0] << ", " << inputs[1] << " -> "
                  << perceptron.predict(inputs) << std::endl;
    }
}

void testCustom()
{
    Perceptron perceptron(2);

    std::vector<std::vector<double>> trainingInputs;
    std::vector<int> labels;

    loadTrainingData("data/logistic_regression.txt", trainingInputs, labels);
    normalizeData(trainingInputs);

    std::cout << "Initial weights and bias:" << std::endl;
    perceptron.printWeightsAndBiases();

    std::cout << "Training perceptron..." << std::endl;
    perceptron.train(trainingInputs, labels, 0.1, 10);

    std::cout << "Weights and bias after training:" << std::endl;
    perceptron.printWeightsAndBiases();

    std::cout << "Testing custom function with 3 inputs:" << std::endl;
    for (const auto &inputs : trainingInputs)
    {
        for (size_t i = 0; i < inputs.size(); ++i)
        {
            std::cout << inputs[i] << (i < inputs.size() - 1 ? ", " : " -> ");
        }
        std::cout << perceptron.predict(inputs) << std::endl;
    }
}

int main()
{
    try
    {
        testAND();
        std::cout << std::endl;

        testOR();
        std::cout << std::endl;

        testCustom(); // logistic_regression.txt
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}