#ifndef MLP_H
#define MLP_H

#include <vector>
#include <functional>
#include <memory>
#include <random>

class MLP
{
public:
    MLP(const std::vector<int> &layers,
        std::function<double(double)> activationFunction = sigmoid,
        std::function<double(double)> activationDerivativeFunction = sigmoidDerivative);

    ~MLP() = default;

    std::vector<double> predict(const std::vector<double> &inputs) const;
    void train(const std::vector<std::vector<double>> &trainingInputs,
               const std::vector<std::vector<double>> &trainingOutputs,
               double learningRate,
               int epochs);

    static double sigmoid(double x);
    static double sigmoidDerivative(double x);
    static double relu(double x);
    static double reluDerivative(double x);
    static double leaky_relu(double x, double alpha = 0.01);
    static double leaky_relu_derivative(double x, double alpha = 0.01);
    static double tanh(double x);
    static double tanhDerivative(double x);

    void setActivationFunctions(std::function<double(double)> activationFunction,
                                std::function<double(double)> activationDerivativeFunction);
    void setRandomSeed(unsigned int seed);

private:
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;
    std::function<double(double)> activation;
    std::function<double(double)> activationDerivative;

    std::shared_ptr<std::mt19937> randomGenerator;

    void initializeWeights(const std::vector<int> &layers);
    std::vector<std::vector<double>> forwardPass(const std::vector<double> &inputs) const;
    void backwardPass(const std::vector<std::vector<double>> &layerOutputs,
                      const std::vector<double> &expectedOutput,
                      double learningRate);
};

#endif