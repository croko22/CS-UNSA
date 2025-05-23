#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

constexpr int IMAGE_MAGIC_NUMBER = 2051;
constexpr int LABEL_MAGIC_NUMBER = 2049;

uint32_t read_uint32_be(std::ifstream &stream)
{
    uint32_t value = 0;
    stream.read(reinterpret_cast<char *>(&value), sizeof(value));
    return __builtin_bswap32(value);
}

std::vector<std::vector<uint8_t>> load_images(const std::string &filepath, int &image_count, int &rows, int &cols)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open image file: " << filepath << std::endl;
        return {};
    }

    uint32_t magic = read_uint32_be(file);
    if (magic != IMAGE_MAGIC_NUMBER)
    {
        std::cerr << "Invalid magic number for image file." << std::endl;
        return {};
    }

    image_count = read_uint32_be(file);
    rows = read_uint32_be(file);
    cols = read_uint32_be(file);

    std::vector<std::vector<uint8_t>> images(image_count, std::vector<uint8_t>(rows * cols));
    for (int i = 0; i < image_count; ++i)
        file.read(reinterpret_cast<char *>(images[i].data()), images[i].size());

    return images;
}

std::vector<uint8_t> load_labels(const std::string &filepath, int &label_count)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open label file: " << filepath << std::endl;
        return {};
    }

    uint32_t magic = read_uint32_be(file);
    if (magic != LABEL_MAGIC_NUMBER)
    {
        std::cerr << "Invalid magic number for label file." << std::endl;
        return {};
    }

    label_count = read_uint32_be(file);

    std::vector<uint8_t> labels(label_count);
    file.read(reinterpret_cast<char *>(labels.data()), labels.size());

    return labels;
}

void display_image(const std::vector<uint8_t> &image, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            std::cout << static_cast<int>(image[i * cols + j]) << " ";
        std::cout << '\n';
    }
}

int main()
{
    const std::string image_path = "../archive/train-images-idx3-ubyte/train-images-idx3-ubyte";
    const std::string label_path = "../archive/train-labels-idx1-ubyte/train-labels-idx1-ubyte";

    int image_count = 0, rows = 0, cols = 0;
    auto images = load_images(image_path, image_count, rows, cols);

    int label_count = 0;
    auto labels = load_labels(label_path, label_count);

    if (images.empty() || labels.empty() || image_count != label_count)
    {
        std::cerr << "Failed to load dataset or size mismatch." << std::endl;
        return 1;
    }

    std::cout << "Loaded " << image_count << " training images of size " << rows << "x" << cols << ".\n";
    std::cout << "Loaded " << label_count << " training labels.\n";

    const int index = 1115;
    if (index < image_count)
    {
        std::cout << "Label at index " << index << ": " << static_cast<int>(labels[index]) << "\n";
        display_image(images[index], rows, cols);
    }

    return 0;
}
