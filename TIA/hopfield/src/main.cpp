#include "hopfield.h"
#include "utils/utils.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>

// Función auxiliar para calcular la precisión de reconstrucción
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

// Función auxiliar para calcular F1 Score
double calculate_f1_score(const std::vector<double> &original, const std::vector<double> &reconstructed,
                          double threshold = 0.0)
{
    int tp = 0, fp = 0, fn = 0;
    for (size_t i = 0; i < original.size(); ++i)
    {
        bool o = original[i] > threshold;
        bool r = reconstructed[i] > threshold;

        if (o && r)
            tp++;
        else if (!o && r)
            fp++;
        else if (o && !r)
            fn++;
    }

    double precision = tp + fp == 0 ? 0.0 : static_cast<double>(tp) / (tp + fp);
    double recall = tp + fn == 0 ? 0.0 : static_cast<double>(tp) / (tp + fn);

    if (precision + recall == 0)
        return 0.0;

    return 2 * (precision * recall) / (precision + recall);
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
        hopfield.save("models/hopfield_model.bin");
        std::cout << "Modelo guardado en hopfield_model.bin" << std::endl;
        std::cout << "Patrones entrenados: " << hopfield.getTrainedPatterns().size() << std::endl;

        // Nuevo: mapa para guardar F1 score por clase
        std::map<int, double> f1_scores;

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
            double f1 = calculate_f1_score(original, reconstructed);

            f1_scores[class_id] = f1;

            std::cout << "Clase " << class_id << " (" << get_class_name(class_id)
                      << "): Precisión=" << accuracy * 100 << "%, F1 Score=" << f1 << std::endl;
        }

        // Guardar F1 scores en CSV
        std::ofstream file("results.csv");
        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo results.csv para escritura.");
        }

        file << "clase,f1_score\n";
        for (const auto &[cls, f1] : f1_scores)
        {
            file << cls << "," << f1 << "\n";
        }
        file.close();

        std::cout << "✅ F1 Scores guardados en results.csv" << std::endl;

        // --- Reconstruir y guardar imágenes ruidosas ---
        std::cout << "Cargando imágenes ruidosas desde CSV..." << std::endl;
        auto noisy_data = load_mnist_csv("data/test_images_with_noise.csv");

        auto noisy_patterns = convert_to_bipolar(noisy_data.images);

        std::cout << "Reconstruyendo patrones ruidosos..." << std::endl;

        std::vector<std::vector<double>> reconstructed_patterns;
        for (const auto &pattern : noisy_patterns)
        {
            auto reconstructed = hopfield.predict(pattern);
            reconstructed_patterns.push_back(reconstructed);
        }

        std::cout << "Guardando patrones reconstruidos en CSV..." << std::endl;

        save_reconstructed_to_csv("data/reconstructed_patterns.csv", reconstructed_patterns, &test_data.labels);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
// #include "hopfield.h"
// #include "utils/utils.cpp"
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <algorithm>
// #include <chrono>

// double calculate_reconstruction_accuracy(const std::vector<double> &original, const std::vector<double> &reconstructed)
// {
//     int correct = 0;
//     for (size_t i = 0; i < original.size(); ++i)
//     {
//         if (original[i] == reconstructed[i])
//         {
//             correct++;
//         }
//     }
//     return static_cast<double>(correct) / original.size();
// }

// int main()
// {
//     try
//     {
//         std::cout << "Cargando datos de entrenamiento desde CSV..." << std::endl;
//         auto train_data = load_mnist_csv("data/fashion-mnist_train.csv");

//         std::cout << "Cargando datos de prueba desde CSV..." << std::endl;
//         auto test_data = load_mnist_csv("data/fashion-mnist_test.csv");

//         std::cout << "Training data loaded: " << train_data.images.size() << " samples." << std::endl;
//         std::cout << "Test data loaded: " << test_data.images.size() << " samples." << std::endl;

//         auto train_patterns = convert_to_bipolar(train_data.images);
//         auto test_patterns = convert_to_bipolar(test_data.images);

//         int pattern_size = train_patterns[0].size();
//         std::cout << "Tamaño de cada patrón: " << pattern_size << " (28x28 = 784 píxeles)" << std::endl;

//         Hopfield hopfield(pattern_size);

//         const int patterns_per_class = 10;
//         std::vector<std::vector<double>> selected_patterns;
//         std::vector<int> selected_labels;

//         for (int class_id = 0; class_id < 10; ++class_id)
//         {
//             int count = 0;
//             for (size_t i = 0; i < train_data.labels.size() && count < patterns_per_class; ++i)
//             {
//                 if (train_data.labels[i] == class_id)
//                 {
//                     selected_patterns.push_back(train_patterns[i]);
//                     selected_labels.push_back(class_id);
//                     count++;
//                 }
//             }
//             std::cout << "Clase " << class_id << ": " << count << " patrones seleccionados" << std::endl;
//         }

//         std::cout << "Entrenando la red Hopfield con " << selected_patterns.size()
//                   << " patrones (10 por clase)..." << std::endl;

//         auto start_train = std::chrono::high_resolution_clock::now();
//         hopfield.train(selected_patterns);
//         auto end_train = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> train_duration = end_train - start_train;
//         std::cout << "Entrenamiento completado en " << train_duration.count() << " segundos\n";
//         hopfield.save("models/hopfield_model.bin");
//         std::cout << "Modelo guardado en hopfield_model.bin" << std::endl;
//         std::cout << "Patrones entrenados: " << hopfield.getTrainedPatterns().size() << std::endl;

//         for (int class_id = 0; class_id < 10; ++class_id)
//         {
//             size_t test_idx = 0;
//             while (test_idx < test_data.labels.size() && test_data.labels[test_idx] != class_id)
//             {
//                 test_idx++;
//             }

//             if (test_idx >= test_data.labels.size())
//             {
//                 std::cerr << "No se encontró patrón para la clase " << class_id << " en datos de prueba" << std::endl;
//                 continue;
//             }

//             auto original = test_patterns[test_idx];
//             auto noisy = original;

//             int noise_points = noisy.size() * 0.2;
//             for (int j = 0; j < noise_points; ++j)
//             {
//                 int idx = rand() % noisy.size();
//                 noisy[idx] *= -1;
//             }

//             auto reconstructed = hopfield.predict(noisy);

//             double accuracy = calculate_reconstruction_accuracy(original, reconstructed);

//             std::cout << "Clase " << class_id << " (" << get_class_name(class_id)
//                       << "): " << accuracy * 100 << "% de precisión" << std::endl;
//         }

//         // NOISY TESTING
//         std::cout << "Cargando imágenes ruidosas desde CSV..." << std::endl;
//         auto noisy_data = load_mnist_csv("data/test_images_with_noise.csv");

//         auto noisy_patterns = convert_to_bipolar(noisy_data.images);

//         std::cout << "Reconstruyendo patrones ruidosos..." << std::endl;

//         std::vector<std::vector<double>> reconstructed_patterns;
//         for (const auto &pattern : noisy_patterns)
//         {
//             auto reconstructed = hopfield.predict(pattern);
//             reconstructed_patterns.push_back(reconstructed);
//         }

//         std::cout << "Guardando patrones reconstruidos en CSV..." << std::endl;

//         save_reconstructed_to_csv("data/reconstructed_patterns.csv", reconstructed_patterns, &test_data.labels);
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
