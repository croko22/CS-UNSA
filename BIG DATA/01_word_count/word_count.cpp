#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>
#include <filesystem>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

void process_chunk(const string &input_file, size_t start, size_t end, const string &output_file)
{
    ifstream file(input_file, ios::binary);
    if (!file)
        exit(1);

    file.seekg(start);
    vector<char> buffer(end - start);
    file.read(buffer.data(), buffer.size());

    int num_threads = omp_get_max_threads();
    vector<unordered_map<string, int>> local_maps(num_threads);

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        size_t chunk_start = (buffer.size() * tid) / num_threads;
        size_t chunk_end = (buffer.size() * (tid + 1)) / num_threads;
        string clean;

        for (size_t i = chunk_start; i < chunk_end; ++i)
        {
            char c = buffer[i];
            if (isalpha(c))
                clean += tolower(c);
            else
            {
                if (!clean.empty())
                {
                    local_maps[tid][clean]++;
                    clean.clear();
                }
            }
        }
        if (!clean.empty())
            local_maps[tid][clean]++;
    }

    unordered_map<string, int> wordCounts;
    for (const auto &lm : local_maps)
        for (const auto &[w, count] : lm)
            wordCounts[w] += count;

    ofstream out(output_file);
    for (const auto &[w, count] : wordCounts)
        out << w << ' ' << count << '\n';
}

void merge_partials(const vector<string> &partial_files, const string &final_output)
{
    unordered_map<string, int> finalCounts;
    for (const string &file_name : partial_files)
    {
        ifstream file(file_name);
        string word;
        int count;
        while (file >> word >> count)
            finalCounts[word] += count;
    }

    ofstream out(final_output);
    for (const auto &[w, count] : finalCounts)
        out << w << ' ' << count << '\n';
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <inputfile>\n";
        return 1;
    }

    string input_file = argv[1];
    const size_t chunk_size = 2ull * 1024 * 1024 * 1024; // 2GB
    ifstream file(input_file, ios::binary | ios::ate);
    if (!file)
    {
        cerr << "Error opening input file\n";
        return 1;
    }

    size_t file_size = file.tellg();
    vector<string> partial_files;
    size_t start = 0, idx = 0;

    auto t_start = high_resolution_clock::now();

    while (start < file_size)
    {
        size_t end = min(start + chunk_size, file_size);
        string partial_name = "partial_" + to_string(idx++) + ".txt";
        process_chunk(input_file, start, end, partial_name);
        partial_files.push_back(partial_name);
        start = end;
    }

    merge_partials(partial_files, "final_wordcount.txt");

    for (const string &f : partial_files)
        filesystem::remove(f);

    auto t_end = high_resolution_clock::now();
    double elapsed = duration<double>(t_end - t_start).count();

    cout << "Done. Elapsed time: " << elapsed << " seconds.\n";
}
