#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <omp.h>

namespace fs = std::filesystem;

std::string toLower(const std::string &s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

std::string removeSpecialChars(const std::string &s)
{
    std::string result;
    for (char c : s)
    {
        if (std::isalnum(static_cast<unsigned char>(c)))
            result += c;
    }
    return result;
}

std::vector<std::string> tokenize(const std::string &text)
{
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word)
    {
        std::string cleaned = removeSpecialChars(word);
        if (!cleaned.empty())
            tokens.push_back(toLower(cleaned));
    }
    return tokens;
}

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc != 2)
    {
        std::cerr << "Uso: " << argv[0] << " <carpeta>" << std::endl;
        return 1;
    }

    std::string folderPath = argv[1];
    std::vector<std::string> filePaths;
    for (const auto &entry : fs::directory_iterator(folderPath))
    {
        if (entry.path().extension() == ".txt")
            filePaths.push_back(entry.path().string());
    }

    int numFiles = filePaths.size();
    std::cout << "Archivos encontrados: " << numFiles << "\n";
    for (int i = 0; i < numFiles; ++i)
        std::cout << i << " - " << filePaths[i] << "\n";

    std::vector<std::map<std::string, std::vector<int>>> localIndices(omp_get_max_threads());

#pragma omp parallel
    {
        int threadId = omp_get_thread_num();
#pragma omp for
        for (int i = 0; i < numFiles; ++i)
        {
            std::ifstream file(filePaths[i]);
            std::string line;
            std::unordered_set<std::string> seenWords;
            while (std::getline(file, line))
            {
                std::vector<std::string> words = tokenize(line);
                for (const std::string &word : words)
                {
                    if (seenWords.insert(word).second)
                    {
                        localIndices[threadId][word].push_back(i + 1);
                    }
                }
            }
        }
    }

    std::map<std::string, std::vector<int>> invertedIndex;
    for (const auto &localMap : localIndices)
    {
        for (const auto &[word, docs] : localMap)
        {
            invertedIndex[word].insert(invertedIndex[word].end(), docs.begin(), docs.end());
        }
    }

    for (auto &[word, docs] : invertedIndex)
    {
        std::sort(docs.begin(), docs.end());
        docs.erase(std::unique(docs.begin(), docs.end()), docs.end());
    }

    std::ofstream outFile("output.txt");
    for (const auto &[word, docs] : invertedIndex)
    {
        outFile << word << " → { ";
        for (size_t i = 0; i < docs.size(); ++i)
        {
            outFile << docs[i];
            if (i != docs.size() - 1)
                outFile << ", ";
        }
        outFile << " }\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nÍndice guardado en output.txt\n";
    std::cout << "Tiempo total: " << elapsed.count() << " segundos\n";

    return 0;
}
