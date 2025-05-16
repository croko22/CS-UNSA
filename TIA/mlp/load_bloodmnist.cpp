#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "npy.hpp"

template <typename T>
std::vector<T> load_npy(const std::string &filename, std::vector<unsigned long> &shape_out)
{
    npy::npy_data data = npy::read_npy<T>(filename);
    std::cout << "shape: ";
    for (const auto &dim : data.shape)
    {
        std::cout << dim << " ";
        shape_out.push_back(dim);
    }
    std::cout << std::endl;
    shape_out = data.shape;
    return data.data;
}

int main()
{
    std::string train_images_path = "bloodmnist/train_images.npy";
    std::string train_labels_path = "bloodmnist/train_labels.npy";

    std::vector<unsigned long> images_shape;
    std::vector<unsigned long> labels_shape;

    // Cargar imágenes como uint8_t
    std::vector<uint8_t> train_images = load_npy<uint8_t>(train_images_path, images_shape);
    std::vector<uint8_t> train_labels = load_npy<uint8_t>(train_labels_path, labels_shape);

    size_t num_samples = images_shape[0]; // Número de imágenes
    size_t height = images_shape[1];      // 28
    size_t width = images_shape[2];       // 28
    size_t channels = images_shape[3];    // 3 (RGB)

    size_t image_size = height * width * channels;
    std::cout << "Número de muestras: " << num_samples << std::endl;
    std::cout << "Dimensiones de imagen: (" << height << ", " << width << ", " << channels << ")" << std::endl;

    // convertir a float y normalizar
    std::vector<float> train_images_float(train_images.begin(), train_images.end());
    for (float &pixel : train_images_float)
    {
        pixel /= 255.0f; // Normalizar a [0, 1]
    }

    std::cout << "Preprocesamiento completado." << std::endl;

    return 0;
}