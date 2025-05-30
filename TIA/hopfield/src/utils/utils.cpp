#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>

struct MnistData
{
    std::vector<std::vector<double>> images;
    std::vector<int> labels;
};

MnistData load_mnist_csv(const std::string &filepath)
{
    MnistData data;
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo: " + filepath);
    }

    std::string line;

    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string value;

        std::getline(ss, value, ',');
        data.labels.push_back(std::stoi(value));

        std::vector<double> pixels;
        while (std::getline(ss, value, ','))
        {
            pixels.push_back(std::stod(value) / 255.0);
        }
        data.images.push_back(pixels);
    }

    return data;
}

std::vector<std::vector<double>> convert_to_bipolar(const std::vector<std::vector<double>> &images, double threshold = 0.5)
{
    std::vector<std::vector<double>> bipolar_images;

    for (const auto &image : images)
    {
        std::vector<double> bipolar_image(image.size());

        std::transform(image.begin(), image.end(), bipolar_image.begin(),
                       [threshold](double pixel)
                       {
                           return pixel > threshold ? 1.0 : -1.0;
                       });

        bipolar_images.push_back(bipolar_image);
    }

    return bipolar_images;
}