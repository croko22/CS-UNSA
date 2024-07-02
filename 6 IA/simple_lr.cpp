#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

//* Extraer datos de X y Y de un archivo CSV a partir de las columnas xRow y yRow
tuple<vector<int>, vector<int>> extractXYData(string filename, int xRow, int yRow)
{
    ifstream file(filename);
    vector<int> X, Y;
    string line;
    bool headerSkipped = false;

    while (getline(file, line))
    {
        // Ignorar la primera línea
        if (!headerSkipped)
        {
            headerSkipped = true;
            continue;
        }

        stringstream ss(line);
        string value;
        vector<string> tokens;

        // Separar la línea por comas
        while (getline(ss, value, ','))
            tokens.push_back(value);

        // Columnas de x e y
        try
        {
            int x = stoi(tokens[xRow]);
            int y = stoi(tokens[yRow]);
            // Ignorar valores atípicos
            if (x <= 3500 && y <= 80000)
            {
                X.push_back(x);
                Y.push_back(y);
            }
        }
        // Ignorar líneas con valores no numéricos
        catch (const std::invalid_argument &e)
        {
            continue;
        }
    }

    file.close();
    return make_tuple(X, Y);
}

//* Calcular valores de b0 y b1
tuple<double, double> linearRegression(vector<int> X, vector<int> Y)
{
    int n = X.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    for (int i = 0; i < n; i++)
    {
        sumX += X[i];
        sumY += Y[i];
        sumXY += X[i] * Y[i];
        sumX2 += X[i] * X[i];
        sumY2 += Y[i] * Y[i];
    }

    double b1 = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double b0 = (sumY - b1 * sumX) / n;

    return make_tuple(b0, b1);
}

int main()
{
    auto XY = extractXYData("articulos_ml.csv", 2, 7);
    vector<int> wordCounts{get<0>(XY)}, sharesCounts{get<1>(XY)};

    //* Calculate the number of records
    auto b0b1 = linearRegression(wordCounts, sharesCounts);
    double b0{get<0>(b0b1)}, b1{get<1>(b0b1)};

    cout << "b0 = " << b0 << endl;
    cout << "b1 = " << b1 << endl;
    cout << "Formula: Shares = " << b0 << " + " << b1 << " * Word Count" << endl;

    //* Predicción de Shares para Word Count 2000
    double wordCount = 2000;
    double predictedShares = b0 + b1 * wordCount;
    cout << "Prediccion # Shares para Word Count 2000: " << predictedShares << endl;

    return 0;
}
