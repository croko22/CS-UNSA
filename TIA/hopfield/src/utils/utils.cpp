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

std::string get_class_name(int class_id)
{
    static const std::vector<std::string> class_names = {
        "T-shirt/top", "Trouser", "Pullover", "Dress", "Coat",
        "Sandal", "Shirt", "Sneaker", "Bag", "Ankle boot"};
    return class_names[class_id];
}

void save_reconstructed_to_csv(const std::string &filename, const std::vector<std::vector<double>> &patterns, const std::vector<int> *labels)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo para escritura: " + filename);
    }

    file << "label";
    for (size_t i = 1; i <= (patterns.empty() ? 0 : patterns[0].size()); ++i)
    {
        file << ",pixel" << i;
    }
    file << "\n";

    for (size_t idx = 0; idx < patterns.size(); ++idx)
    {

        int label = (labels && idx < labels->size()) ? (*labels)[idx] : -1;
        file << label;

        for (size_t i = 0; i < patterns[idx].size(); ++i)
        {
            int pixel_value = static_cast<int>((patterns[idx][i] + 1.0) / 2.0 * 255);
            file << "," << pixel_value;
        }
        file << "\n";
    }

    file.close();
}