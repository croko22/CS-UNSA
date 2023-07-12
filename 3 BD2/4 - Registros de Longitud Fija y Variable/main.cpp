#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

constexpr int PAGE_SIZE = 4096;
constexpr int RECORD_FIX_LEN = 115;
constexpr int RECORDS_PER_PAGE = PAGE_SIZE * 0.5 / RECORD_FIX_LEN;
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
        if (i == 15)
            break;
    }
    file.close();
    //? Delete first row
    data.erase(data.begin());
}

void fixedLengthRecord(std::vector<std::vector<std::string>> &data)
{
    //* Calculate cuantity of records per page
    cout << "Records per page: " << RECORDS_PER_PAGE << endl;
    //? File header
    std::string temp{""};
    int numRecords = data.size();
    int freeSpace = PAGE_SIZE - (numRecords - 1) * RECORD_FIX_LEN;
    std::fstream output("titanic.bin", std::ios::out | std::ios::binary);
    output.seekp(0, std::ios::beg);
    output << numRecords << " " << RECORD_FIX_LEN << " " << freeSpace << " ";

    //? Write data Position 4096 | record size 115
    for (int i = 0; i < data.size(); i++)
        output << 4096 - i * RECORD_FIX_LEN << " " << RECORD_FIX_LEN << " ";

    // Reverse vector and start writing at freeSpace
    std::reverse(data.begin(), data.end());
    output.seekp(freeSpace, std::ios::beg);
    for (auto row : data)
    {
        temp = "";
        for (auto col : row)
            temp += col + " ";
        //* Complete to 115 with spaces
        temp.resize(115, ' ');
        output << temp;
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