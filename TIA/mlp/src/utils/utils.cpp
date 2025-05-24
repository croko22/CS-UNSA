#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "../../include/npy.hpp"

template <typename T>
std::vector<T> load_npy(const std::string &filename, std::vector<unsigned long> &shape_out)
{
    npy::npy_data data = npy::read_npy<T>(filename);
    shape_out = data.shape;
    return data.data;
}

struct BloodMnistData
{
    std::vector<std::vector<double>> images; // Normalizadas [0,1]
    std::vector<uint8_t> labels;
    std::vector<unsigned long> images_shape;
    std::vector<unsigned long> labels_shape;
};

BloodMnistData load_bloodmnist(const std::string &images_path, const std::string &labels_path)
{
    std::vector<unsigned long> images_shape;
    std::vector<unsigned long> labels_shape;

    std::vector<uint8_t> train_images = load_npy<uint8_t>(images_path, images_shape);
    std::vector<uint8_t> train_labels = load_npy<uint8_t>(labels_path, labels_shape);

    size_t num_samples = images_shape[0];
    size_t height = images_shape[1];
    size_t width = images_shape[2];
    size_t channels = images_shape[3];
    size_t image_size = height * width * channels;

    // Normalizar y convertir a vector de vectores
    std::vector<std::vector<double>> images_vec(num_samples, std::vector<double>(image_size));
    for (size_t i = 0; i < num_samples; ++i)
    {
        for (size_t j = 0; j < image_size; ++j)
        {
            images_vec[i][j] = train_images[i * image_size + j] / 255.0;
        }
    }

    BloodMnistData data;
    data.images = std::move(images_vec);
    data.labels = std::move(train_labels);
    data.images_shape = images_shape;
    data.labels_shape = labels_shape;
    return data;
}

double mse(const std::vector<double> &y_true, const std::vector<double> &y_pred)
{
    double sum = 0.0;
    for (size_t i = 0; i < y_true.size(); ++i)
    {
        sum += (y_true[i] - y_pred[i]) * (y_true[i] - y_pred[i]);
    }
    return sum / y_true.size();
}