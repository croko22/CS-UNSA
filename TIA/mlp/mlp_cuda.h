#ifndef MLP_CUDA_H
#define MLP_CUDA_H

#include <vector>
#include <cmath>
#include <random>
#include <cuda_runtime.h>
#include <iostream>

#define CHECK_CUDA(x)                                                                          \
    {                                                                                          \
        cudaError_t err = x;                                                                   \
        if (err != cudaSuccess)                                                                \
            std::cerr << #x << " failed with error: " << cudaGetErrorString(err) << std::endl; \
    }

// Funciones auxiliares
void randomInit(double *data, int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.5, 0.5);
    for (int i = 0; i < size; ++i)
        data[i] = dis(gen);
}

// Kernel de suma ponderada
__global__ void computeWeightedSumKernel(
    double *weights,
    double *biases,
    double *input,
    double *output,
    int inSize,
    int outSize)
{
    int neuron = threadIdx.x + blockIdx.x * blockDim.x;
    if (neuron < outSize)
    {
        double sum = biases[neuron];
        for (int i = 0; i < inSize; ++i)
            sum += weights[neuron * inSize + i] * input[i];
        output[neuron] = sum;
    }
}

// Función de activación
__device__ double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

__device__ double sigmoidDerivative(double x)
{
    return x * (1.0 - x);
}

__global__ void activateKernel(double *input, double *output, int size)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < size)
        output[idx] = sigmoid(input[idx]);
}

__global__ void derivativeActivateKernel(double *input, double *output, int size)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < size)
        output[idx] = sigmoidDerivative(input[idx]);
}

// Actualización de gradientes
__global__ void updateWeightsKernel(
    double *weights,
    double *deltas,
    double *inputs,
    int inSize,
    int outSize,
    double learningRate)
{
    int neuron = threadIdx.x + blockIdx.x * blockDim.x;
    if (neuron < outSize)
    {
        for (int i = 0; i < inSize; ++i)
            weights[neuron * inSize + i] += learningRate * deltas[neuron] * inputs[i];
    }
}

__global__ void updateBiasesKernel(double *biases, double *deltas, int size, double learningRate)
{
    int neuron = threadIdx.x + blockIdx.x * blockDim.x;
    if (neuron < size)
        biases[neuron] += learningRate * deltas[neuron];
}

struct Layer
{
    double *weights, *biases;     // Host
    double *d_weights, *d_biases; // Device
    int input_size, output_size;

    Layer(int in, int out) : input_size(in), output_size(out)
    {
        weights = new double[in * out];
        biases = new double[out];
        randomInit(weights, in * out);
        randomInit(biases, out);

        CHECK_CUDA(cudaMalloc(&d_weights, in * out * sizeof(double)));
        CHECK_CUDA(cudaMalloc(&d_biases, out * sizeof(double)));

        CHECK_CUDA(cudaMemcpy(d_weights, weights, in * out * sizeof(double), cudaMemcpyHostToDevice));
        CHECK_CUDA(cudaMemcpy(d_biases, biases, out * sizeof(double), cudaMemcpyHostToDevice));
    }

    ~Layer()
    {
        delete[] weights;
        delete[] biases;
        cudaFree(d_weights);
        cudaFree(d_biases);
    }
};

class MLP_CUDA
{
private:
    std::vector<Layer> layers;
    double *d_input, *d_output, *d_weightedSum;
    double *input_h, *output_h;

public:
    MLP_CUDA(const std::vector<int> &layerSizes)
    {
        for (size_t i = 1; i < layerSizes.size(); ++i)
        {
            layers.emplace_back(layerSizes[i - 1], layerSizes[i]);
        }
    }

    ~MLP_CUDA()
    {
        delete[] input_h;
        delete[] output_h;
        cudaFree(d_input);
        cudaFree(d_output);
        cudaFree(d_weightedSum);
    }

    std::vector<double> predict(const std::vector<double> &input)
    {
        int inputSize = input.size();
        input_h = new double[inputSize];
        for (int i = 0; i < inputSize; ++i)
            input_h[i] = input[i];

        CHECK_CUDA(cudaMalloc(&d_input, inputSize * sizeof(double)));
        CHECK_CUDA(cudaMemcpy(d_input, input_h, inputSize * sizeof(double), cudaMemcpyHostToDevice));

        double *currentInput = d_input;
        int currentSize = inputSize;

        for (auto &layer : layers)
        {
            dim3 threads(256);
            dim3 blocks((layer.output_size + threads.x - 1) / threads.x);

            CHECK_CUDA(cudaMalloc(&d_weightedSum, layer.output_size * sizeof(double)));
            CHECK_CUDA(cudaMalloc(&d_output, layer.output_size * sizeof(double)));

            computeWeightedSumKernel<<<blocks, threads>>>(
                layer.d_weights, layer.d_biases, currentInput, d_weightedSum, currentSize, layer.output_size);
            activateKernel<<<blocks, threads>>>(d_weightedSum, d_output, layer.output_size);
            cudaDeviceSynchronize();

            if (currentInput != d_input)
                cudaFree(currentInput);
            currentInput = d_output;
            currentSize = layer.output_size;
        }

        output_h = new double[currentSize];
        CHECK_CUDA(cudaMemcpy(output_h, currentInput, currentSize * sizeof(double), cudaMemcpyDeviceToHost));

        std::vector<double> result(output_h, output_h + currentSize);
        delete[] output_h;

        return result;
    }

    void train(
        const std::vector<std::vector<double>> &X_train,
        const std::vector<std::vector<double>> &y_train,
        double learning_rate,
        int epochs)
    {
        int inputSize = X_train[0].size();
        int outputSize = y_train[0].size();

        double *d_error = nullptr;
        double *d_delta = nullptr;
        double *h_delta = new double[outputSize];

        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (size_t sample = 0; sample < X_train.size(); ++sample)
            {
                auto prediction = predict(X_train[sample]);

                // Calcular error
                std::vector<double> target = y_train[sample];
                std::vector<double> errors(outputSize);
                for (int i = 0; i < outputSize; ++i)
                    errors[i] = target[i] - prediction[i];

                // Copiar a dispositivo
                CHECK_CUDA(cudaMalloc(&d_error, outputSize * sizeof(double)));
                CHECK_CUDA(cudaMemcpy(d_error, errors.data(), outputSize * sizeof(double), cudaMemcpyHostToDevice));

                // Derivada de salida
                CHECK_CUDA(cudaMalloc(&d_delta, outputSize * sizeof(double)));
                derivativeActivateKernel<<<dim3((outputSize + 255) / 256), dim3(256)>>>(d_output, d_delta, outputSize);
                cudaDeviceSynchronize();

                for (int i = 0; i < outputSize; ++i)
                    h_delta[i] = errors[i] * (prediction[i] * (1.0 - prediction[i]));

                CHECK_CUDA(cudaMemcpy(d_delta, h_delta, outputSize * sizeof(double), cudaMemcpyHostToDevice));

                // Actualizar última capa
                Layer &lastLayer = layers.back();
                CHECK_CUDA(cudaMemcpy(lastLayer.d_biases, h_delta, outputSize * sizeof(double), cudaMemcpyHostToDevice));

                updateWeightsKernel<<<dim3((lastLayer.input_size * lastLayer.output_size + 255) / 256), dim3(256)>>>(
                    lastLayer.d_weights, d_delta, d_input, lastLayer.input_size, lastLayer.output_size, learning_rate);
                cudaDeviceSynchronize();

                cudaFree(d_error);
                cudaFree(d_delta);
            }
        }

        delete[] h_delta;
    }
};

#endif // MLP_CUDA_H