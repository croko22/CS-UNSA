#ifndef HOPFIELD_H
#define HOPFIELD_H

#include <vector>
#include <string>

class Hopfield
{
public:
    Hopfield(int size);
    void train(const std::vector<std::vector<double>> &patterns);
    std::vector<double> predict(const std::vector<double> &pattern, int max_iterations = 100);
    double energy(const std::vector<double> &state);

    bool save(const std::string &filename) const;
    bool load(const std::string &filename);

    const std::vector<std::vector<double>> &getTrainedPatterns() const;

private:
    int size_;
    std::vector<std::vector<double>> weights_;
    std::vector<std::vector<double>> trained_patterns_;

    double sign(double x);
};

#endif