#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int main()
{
    // H. Estudio
    vector<double> X = {3, 6, 8, 2, 1, 6};
    // NOTA
    vector<double> Y = {8, 10, 15, 8, 5, 12};

    // Calculo de b1 y b0
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
    double correlation = (n * sumXY - sumX * sumY) / sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

    cout << "b0 = " << b0 << endl;
    cout << "b1 = " << b1 << endl;
    cout << "Correlacion (r) = " << correlation << endl;

    return 0;
}
