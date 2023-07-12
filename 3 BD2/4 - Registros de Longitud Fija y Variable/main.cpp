#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <valarray>

constexpr int PAGE_SIZE = 4096;
constexpr int RECORD_FIX_LEN = 115;
constexpr int RECORDS_PER_PAGE = PAGE_SIZE * 0.5 / RECORD_FIX_LEN;
constexpr int RECORDS_TO_LOAD = 50; // Working with 50 | 500 records
constexpr int REQUIRED_PAGES = (RECORDS_TO_LOAD / RECORDS_PER_PAGE) + (RECORDS_TO_LOAD % RECORDS_PER_PAGE > 0 ? 1 : 0);
using namespace std;

void readCSVData(std::vector<std::vector<std::string>> &data)
{
    std::fstream file("titanic.csv", std::ios::in);
    std::string line, word;
    int i = 0;
    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream s(line);
        while (std::getline(s, word, ','))
            row.push_back(word);
        data.push_back(row);
        i++;
        //? Read only 10 rows
        if (i == RECORDS_TO_LOAD)
            break;
    }
    file.close();
    //? Delete first row | headers
    data.erase(data.begin());
}

void fixedLengthRecord(std::vector<std::vector<std::string>> &data)
{
    std::string temp{""};
    int numRecords = data.size();
    cout << "Pages needed: " << REQUIRED_PAGES << " Records per page: " << RECORDS_PER_PAGE << endl;
    int freeSpace = PAGE_SIZE - (numRecords - 1) * RECORD_FIX_LEN;
    std::fstream output("titanic.bin", std::ios::out | std::ios::binary);

    //* WRITE DATA
    int recordPerCurrentPage{0};
    // std::reverse(data.begin(), data.end()); //? Old method
    for (int i = 0; i < REQUIRED_PAGES; i++)
    {
        //? Slice vector copy of size RECORDS_PER_PAGE
        vector<vector<string>> slice(data.begin() + i * RECORDS_PER_PAGE, data.begin() + i * RECORDS_PER_PAGE + RECORDS_PER_PAGE);
        reverse(slice.begin(), slice.end());

        //* File header
        recordPerCurrentPage = i != REQUIRED_PAGES - 1 ? RECORDS_PER_PAGE : numRecords % RECORDS_PER_PAGE;
        freeSpace = PAGE_SIZE - recordPerCurrentPage * RECORD_FIX_LEN;
        output.seekp(i * PAGE_SIZE, std::ios::beg);
        output << recordPerCurrentPage << " " << RECORD_FIX_LEN << " " << freeSpace << " ";
        //? Write data Position 4096 | record size 115
        for (int j = 0; j < recordPerCurrentPage; j++)
            output << (PAGE_SIZE * (i + 1)) - j * RECORD_FIX_LEN << " " << RECORD_FIX_LEN << " ";

        output.seekp(freeSpace + PAGE_SIZE * i, std::ios::beg);
        //* Iterate over slice
        for (auto row : slice)
        {
            temp = "";
            for (auto col : row)
                temp += col + " ";
            //* Complete to 115 with spaces
            temp.resize(115, ' ');
            output << temp;
        }

        // ? Alt method
        // LIMIT: RECORDS_PER_PAGE
        // for (int j = 0; j < RECORDS_PER_PAGE - 1; j++)
        // {
        //     temp = "";
        //     // for (auto col : data[j + i * RECORDS_PER_PAGE])
        //       // for (auto col : data[j + (numRecords - RECORDS_PER_PAGE) - (i * RECORDS_PER_PAGE)])
        //         temp += col + " ";
        //     //* Complete to 115 with spaces
        //     temp.resize(115, ' ');
        //     output << temp;
        // }
    }
    output.close();
}

void variableLenghtRecord(std::vector<std::vector<std::string>> &data)
{
    // TODO: (Use in variable size) - Size of first row in bytes
    int size{0};
    for (int i = 0; i < data[2].size(); i++)
    {
        size += data[2][i].size();
    }
    cout << "Size of first row in bytes: " << size << endl;
}

int main()
{
    //  TODO: Read the titanic csv
    fstream file("titanic.csv", ios::in);
    vector<vector<string>> data;
    readCSVData(data);

    //? Print data
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < 12; j++)
            cout << data[i][j] << " ";
        cout << endl;
    }

    //* OUTPUT FILE
    fixedLengthRecord(data);

    //? Read binary file
    fstream input("titanic.bin", ios::in | ios::binary);
    input.seekg(0, ios::beg);
    int numRecords2, recordSize, freeSpace2;
    input >> numRecords2 >> recordSize >> freeSpace2;
    cout << "Number of records: " << numRecords2 << endl;
    cout << "Record size: " << recordSize << endl;
    cout << "Free space: " << freeSpace2 << endl;
    //? Read data Position 4096 | record size 115
    int pos, size2;
    for (int i = 0; i < numRecords2; i++)
    {
        input >> pos >> size2;
        cout << "Position: " << pos << " Size: " << size2 << endl;
    }
    //? Read data
    input.seekg(pos + 115, ios::beg);
    vector<char> read(size2, ' ');
    input.read(read.data(), size2);
    cout << "Read: " << read.data() << endl;
    input.close();

    return 0;
}