#include "mlp.h"
#include "load_bloodmnist.cpp"
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

double calculate_accuracy(const MLP &model,
                          const std::vector<std::vector<double>> &X,
                          const std::vector<std::vector<double>> &Y)
{
    int correct = 0;
    for (size_t i = 0; i < X.size(); ++i)
    {
        auto pred = model.predict(X[i]);
        int pred_class = std::distance(pred.begin(), std::max_element(pred.begin(), pred.end()));
        int true_class = std::distance(Y[i].begin(), std::max_element(Y[i].begin(), Y[i].end()));
        if (pred_class == true_class)
        {
            ++correct;
        }
    }
    return static_cast<double>(correct) / X.size();
}

int main()
{

    const std::string train_images_path = "bloodmnist/train_images.npy";
    const std::string train_labels_path = "bloodmnist/train_labels.npy";
    const std::string test_images_path = "bloodmnist/test_images.npy";
    const std::string test_labels_path = "bloodmnist/test_labels.npy";
    const std::string metrics_file = "training_metrics.csv";

    const int epochs = 100;
    const int batch_size = 32;
    double initial_learning_rate = 0.1;
    const double learning_rate_decay = 0.95;
    const int eval_every = 5;

#ifdef USE_CUDA
    std::cout << "Ejecutando con CUDA (GPU)" << std::endl;
#else
    std::cout << "Ejecutando en CPU" << std::endl;
#endif

    std::cout << "Cargando dataset bloodmnist..." << std::endl;
    auto start_load = std::chrono::high_resolution_clock::now();

    BloodMnistData train_data = load_bloodmnist_train(train_images_path, train_labels_path);
    BloodMnistData test_data = load_bloodmnist_train(test_images_path, test_labels_path);

    std::vector<std::vector<double>> X_train = train_data.images;
    std::vector<std::vector<double>> Y_train(train_data.labels.size(), std::vector<double>(8, 0.0));
    for (size_t i = 0; i < train_data.labels.size(); ++i)
    {
        Y_train[i][train_data.labels[i]] = 1.0;
    }

    std::vector<std::vector<double>> X_test = test_data.images;
    std::vector<std::vector<double>> Y_test(test_data.labels.size(), std::vector<double>(8, 0.0));
    for (size_t i = 0; i < test_data.labels.size(); ++i)
    {
        Y_test[i][test_data.labels[i]] = 1.0;
    }

    auto end_load = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> load_duration = end_load - start_load;
    std::cout << "Datos cargados en " << load_duration.count() << " segundos\n";

    std::cout << "Inicializando MLP..." << std::endl;
#ifdef USE_CUDA
    MLP_CUDA mlp({2352, 128, 64, 8});
#else
    MLP mlp({2352, 128, 64, 8}, MLP::sigmoid, MLP::sigmoidDerivative);
#endif

    std::cout << "Entrenando..." << std::endl;
    TrainingMetrics metrics;
    double learning_rate = initial_learning_rate;

    auto start_train = std::chrono::high_resolution_clock::now();

    for (int epoch = 1; epoch <= epochs; ++epoch)
    {

        learning_rate *= learning_rate_decay;

        mlp.train(X_train, Y_train, learning_rate, 1);

        double epoch_loss = 0.0;
        for (size_t i = 0; i < X_train.size(); ++i)
        {
            epoch_loss += mse(Y_train[i], mlp.predict(X_train[i]));
        }
        epoch_loss /= X_train.size();

        double train_acc = calculate_accuracy(mlp, X_train, Y_train);

        metrics.epoch_loss.push_back(epoch_loss);
        metrics.epoch_accuracy.push_back(train_acc);
        metrics.learning_rates.push_back(learning_rate);

        if (epoch % eval_every == 0 || epoch == 1 || epoch == epochs)
        {
            double test_acc = calculate_accuracy(mlp, X_test, Y_test);
            metrics.test_accuracy.push_back(test_acc);

            std::cout << "Epoch " << epoch << "/" << epochs
                      << " - Loss: " << epoch_loss
                      << " - Train Acc: " << train_acc * 100 << "%"
                      << " - Test Acc: " << test_acc * 100 << "%"
                      << " - LR: " << learning_rate << std::endl;
        }
        else
        {
            std::cout << "Epoch " << epoch << "/" << epochs
                      << " - Loss: " << epoch_loss
                      << " - Train Acc: " << train_acc * 100 << "%"
                      << " - LR: " << learning_rate << std::endl;
        }
    }

    auto end_train = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> train_duration = end_train - start_train;
    std::cout << "Entrenamiento completado en " << train_duration.count() << " segundos\n";

    save_metrics_to_csv(metrics, metrics_file);

    std::cout << "\nEvaluación final:" << std::endl;
    double final_train_acc = calculate_accuracy(mlp, X_train, Y_train);
    double final_test_acc = calculate_accuracy(mlp, X_test, Y_test);

    std::cout << "Accuracy en train set: " << final_train_acc * 100 << "%" << std::endl;
    std::cout << "Accuracy en test set: " << final_test_acc * 100 << "%" << std::endl;

    return 0;
}