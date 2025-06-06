#include "hopfield.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

Hopfield::Hopfield(int size) : size_(size)
{
    if (size <= 0)
    {
        throw std::invalid_argument("Network size must be positive");
    }

    weights_.resize(size, std::vector<double>(size, 0.0));
}

void Hopfield::train(const std::vector<std::vector<double>> &patterns)
{
    if (patterns.empty())
        return;

    trained_patterns_ = patterns;

    weights_ = std::vector<std::vector<double>>(size_, std::vector<double>(size_, 0.0));

    for (const auto &pattern : patterns)
    {
        if (pattern.size() != size_)
        {
            throw std::invalid_argument("Pattern size doesn't match network size");
        }

        for (int i = 0; i < size_; ++i)
        {
            for (int j = 0; j < size_; ++j)
            {
                if (i == j)
                {
                    weights_[i][j] = 0;
                }
                else
                {
                    weights_[i][j] += pattern[i] * pattern[j];
                }
            }
        }
    }

    double norm_factor = 1.0 / patterns.size();
    for (int i = 0; i < size_; ++i)
    {
        for (int j = 0; j < size_; ++j)
        {
            weights_[i][j] *= norm_factor;
        }
    }
}

std::vector<double> Hopfield::predict(const std::vector<double> &pattern, int max_iterations)
{
    if (pattern.size() != size_)
    {
        throw std::invalid_argument("Input pattern size doesn't match network size");
    }

    std::vector<double> current_state = pattern;
    std::vector<double> next_state(size_);
    bool stable = false;
    int iterations = 0;

    while (!stable && iterations < max_iterations)
    {
        stable = true;

        for (int i = 0; i < size_; ++i)
        {
            double sum = 0.0;
            for (int j = 0; j < size_; ++j)
            {
                sum += weights_[i][j] * current_state[j];
            }
            next_state[i] = sign(sum);

            if (next_state[i] != current_state[i])
            {
                stable = false;
            }
        }

        current_state = next_state;
        iterations++;
    }

    return current_state;
}

double Hopfield::energy(const std::vector<double> &state)
{
    if (state.size() != size_)
    {
        throw std::invalid_argument("State size doesn't match network size");
    }

    double energy = 0.0;
    for (int i = 0; i < size_; ++i)
    {
        for (int j = 0; j < size_; ++j)
        {
            energy += weights_[i][j] * state[i] * state[j];
        }
    }
    return -0.5 * energy;
}

double Hopfield::sign(double x)
{
    return x >= 0 ? 1.0 : -1.0;
}

bool Hopfield::save(const std::string &filename) const
{

    std::ofstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    file << size_ << "\n";

    file << "Weights:\n";
    for (const auto &row : weights_)
    {
        for (double val : row)
        {
            file << val << " ";
        }
        file << "\n";
    }

    file << "Trained Patterns:\n";
    file << trained_patterns_.size() << "\n";
    for (const auto &pattern : trained_patterns_)
    {
        for (double val : pattern)
        {
            file << val << " ";
        }
        file << "\n";
    }

    return true;
}

bool Hopfield::load(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    int loaded_size;
    file.read(reinterpret_cast<char *>(&loaded_size), sizeof(loaded_size));

    if (loaded_size != size_)
    {

        return false;
    }

    for (auto &row : weights_)
    {
        file.read(reinterpret_cast<char *>(row.data()), row.size() * sizeof(double));
    }

    size_t num_patterns;
    file.read(reinterpret_cast<char *>(&num_patterns), sizeof(num_patterns));

    trained_patterns_.resize(num_patterns);
    for (auto &pattern : trained_patterns_)
    {
        pattern.resize(size_);
        file.read(reinterpret_cast<char *>(pattern.data()), pattern.size() * sizeof(double));
    }

    return true;
}

const std::vector<std::vector<double>> &Hopfield::getTrainedPatterns() const
{
    return trained_patterns_;
}