#include "hopfield.h"
#include "utils/utils.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

struct TrainingMetrics
{
    std::vector<double> epoch_loss;
    std::vector<double> epoch_accuracy;
    std::vector<double> test_accuracy;
    std::vector<double> learning_rates;
};

std::string get_class_name(int class_id)
{
    static const std::vector<std::string> class_names = {
        "T-shirt/top", "Trouser", "Pullover", "Dress", "Coat",
        "Sandal", "Shirt", "Sneaker", "Bag", "Ankle boot"};
    return class_names[class_id];
}

void save_metrics_to_csv(const TrainingMetrics &metrics, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    file << "Epoch,Loss,Accuracy,TestAccuracy,LearningRate\n";
    for (size_t i = 0; i < metrics.epoch_loss.size(); ++i)
    {
        file << i + 1 << ","
             << metrics.epoch_loss[i] << ","
             << metrics.epoch_accuracy[i] << ","
             << (i < metrics.test_accuracy.size() ? metrics.test_accuracy[i] : 0) << ","
             << (i < metrics.learning_rates.size() ? metrics.learning_rates[i] : 0) << "\n";
    }
    file.close();
    std::cout << "Métricas guardadas en " << filename << std::endl;
}

double calculate_reconstruction_accuracy(const std::vector<double> &original, const std::vector<double> &reconstructed)
{
    int correct = 0;
    for (size_t i = 0; i < original.size(); ++i)
    {
        if (original[i] == reconstructed[i])
        {
            correct++;
        }
    }
    return static_cast<double>(correct) / original.size();
}

int main()
{
    try
    {
        std::cout << "Cargando datos de entrenamiento desde CSV..." << std::endl;
        auto train_data = load_mnist_csv("data/fashion-mnist_train.csv");

        std::cout << "Cargando datos de prueba desde CSV..." << std::endl;
        auto test_data = load_mnist_csv("data/fashion-mnist_test.csv");

        std::cout << "Training data loaded: " << train_data.images.size() << " samples." << std::endl;
        std::cout << "Test data loaded: " << test_data.images.size() << " samples." << std::endl;

        auto train_patterns = convert_to_bipolar(train_data.images);
        auto test_patterns = convert_to_bipolar(test_data.images);

        int pattern_size = train_patterns[0].size();
        std::cout << "Tamaño de cada patrón: " << pattern_size << " (28x28 = 784 píxeles)" << std::endl;

        Hopfield hopfield(pattern_size);

        const int patterns_per_class = 10;
        std::vector<std::vector<double>> selected_patterns;
        std::vector<int> selected_labels;

        for (int class_id = 0; class_id < 10; ++class_id)
        {
            int count = 0;
            for (size_t i = 0; i < train_data.labels.size() && count < patterns_per_class; ++i)
            {
                if (train_data.labels[i] == class_id)
                {
                    selected_patterns.push_back(train_patterns[i]);
                    selected_labels.push_back(class_id);
                    count++;
                }
            }
            std::cout << "Clase " << class_id << ": " << count << " patrones seleccionados" << std::endl;
        }

        std::cout << "Entrenando la red Hopfield con " << selected_patterns.size()
                  << " patrones (10 por clase)..." << std::endl;

        auto start_train = std::chrono::high_resolution_clock::now();
        hopfield.train(selected_patterns);
        auto end_train = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> train_duration = end_train - start_train;
        std::cout << "Entrenamiento completado en " << train_duration.count() << " segundos\n";
        hopfield.save("hopfield_model.bin");
        std::cout << "Modelo guardado en hopfield_model.bin" << std::endl;
        std::cout << "Patrones entrenados: " << hopfield.getTrainedPatterns().size() << std::endl;

        TrainingMetrics metrics;

        for (int class_id = 0; class_id < 10; ++class_id)
        {
            size_t test_idx = 0;
            while (test_idx < test_data.labels.size() && test_data.labels[test_idx] != class_id)
            {
                test_idx++;
            }

            if (test_idx >= test_data.labels.size())
            {
                std::cerr << "No se encontró patrón para la clase " << class_id << " en datos de prueba" << std::endl;
                continue;
            }

            auto original = test_patterns[test_idx];
            auto noisy = original;

            int noise_points = noisy.size() * 0.2;
            for (int j = 0; j < noise_points; ++j)
            {
                int idx = rand() % noisy.size();
                noisy[idx] *= -1;
            }

            auto reconstructed = hopfield.predict(noisy);

            double accuracy = calculate_reconstruction_accuracy(original, reconstructed);
            metrics.test_accuracy.push_back(accuracy);

            std::cout << "Clase " << class_id << " (" << get_class_name(class_id)
                      << "): " << accuracy * 100 << "% de precisión" << std::endl;
        }

        save_metrics_to_csv(metrics, "hopfield_metrics.csv");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
