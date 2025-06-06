#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <omp.h>
#include <climits>

using namespace std;

const int match_score = 1;
const int mismatch_score = -1;
const int gap_score = -2;

string extractShortID(const string &header)
{
    size_t pos = header.find(' ');
    if (pos != string::npos)
        return header.substr(0, pos);
    else
        return header;
}

map<string, string> readFasta(const string &filename)
{
    ifstream file(filename);
    map<string, string> sequences;
    string line, id;

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        if (line[0] == '>')
        {
            id = extractShortID(line.substr(1));
        }
        else
        {
            sequences[id] += line;
        }
    }

    return sequences;
}

int max3(int a, int b, int c, char &dir)
{
    int max_val = max(a, max(b, c));
    dir = (a == max_val) ? 'd' : ((b == max_val) ? 'u' : 'l');
    return max_val;
}

pair<string, string> needlemanWunsch(const string &s1, const string &s2, int &score)
{
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    vector<vector<char>> dir(n + 1, vector<char>(m + 1));

    for (int i = 0; i <= n; ++i)
        dp[i][0] = gap_score * i;
    for (int j = 0; j <= m; ++j)
        dp[0][j] = gap_score * j;

#pragma omp parallel for collapse(2)
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            int diag = dp[i - 1][j - 1] + ((s1[i - 1] == s2[j - 1]) ? match_score : mismatch_score);
            int up = dp[i - 1][j] + gap_score;
            int left = dp[i][j - 1] + gap_score;
            dp[i][j] = max3(diag, up, left, dir[i][j]);
        }
    }

    string as1, as2;
    int i = n, j = m;
    while (i > 0 || j > 0)
    {
        if (dir[i][j] == 'd')
        {
            as1.push_back(s1[i - 1]);
            as2.push_back(s2[j - 1]);
            --i;
            --j;
        }
        else if (dir[i][j] == 'u')
        {
            as1.push_back(s1[i - 1]);
            as2.push_back('-');
            --i;
        }
        else
        {
            as1.push_back('-');
            as2.push_back(s2[j - 1]);
            --j;
        }
    }

    reverse(as1.begin(), as1.end());
    reverse(as2.begin(), as2.end());

    score = dp[n][m];
    return {as1, as2};
}

vector<vector<int>> computeScoreMatrix(const vector<string> &seqs)
{
    int n = seqs.size();
    vector<vector<int>> scoreMatrix(n, vector<int>(n, 0));

#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            int score;
            needlemanWunsch(seqs[i], seqs[j], score);
            scoreMatrix[i][j] = score;
            scoreMatrix[j][i] = score;
        }
    }

    return scoreMatrix;
}

int selectStarSequence(const vector<vector<int>> &scoreMatrix)
{
    int maxSum = INT_MIN, starIndex = 0;
    for (size_t i = 0; i < scoreMatrix.size(); ++i)
    {
        int sum = 0;
        for (size_t j = 0; j < scoreMatrix.size(); ++j)
            sum += scoreMatrix[i][j];
        if (sum > maxSum)
        {
            maxSum = sum;
            starIndex = i;
        }
    }
    return starIndex;
}

vector<pair<string, string>> alignToStar(const vector<string> &seqs, int starIndex)
{
    int n = seqs.size();
    vector<pair<string, string>> alignments(n);

#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
        if (i == starIndex)
        {
            alignments[i] = {seqs[i], seqs[i]};
        }
        else
        {
            int dummy_score;
            alignments[i] = needlemanWunsch(seqs[starIndex], seqs[i], dummy_score);
        }
    }

    return alignments;
}

vector<string> buildMSA(const vector<pair<string, string>> &alignedSeqs)
{
    int n = alignedSeqs.size();
    vector<string> msa(n);

    for (int i = 0; i < n; ++i)
    {
        msa[i] = alignedSeqs[i].second;
    }

    return msa;
}

void saveOutput(const string &outputFile,
                const vector<vector<int>> &scoreMatrix,
                const vector<string> &ids,
                const vector<pair<string, string>> &alignments,
                const vector<string> &msa)
{

    ofstream out(outputFile);

    out << "=== Matriz de Scores ===\n";
    for (auto id : ids)
        out << "\t" << id;
    out << "\n";

    for (size_t i = 0; i < scoreMatrix.size(); ++i)
    {
        out << ids[i];
        for (size_t j = 0; j < scoreMatrix[i].size(); ++j)
            out << "\t" << scoreMatrix[i][j];
        out << "\n";
    }

    out << "\n=== Alineamientos contra la estrella ===\n";
    for (size_t i = 0; i < alignments.size(); ++i)
    {
        out << ">" << ids[i] << "\n";
        out << alignments[i].first << "\n";
        out << alignments[i].second << "\n\n";
    }

    out << "\n=== Alineamiento Múltiple ===\n";
    for (size_t i = 0; i < msa.size(); ++i)
    {
        out << ">" << ids[i] << "\n"
            << msa[i] << "\n";
    }

    out.close();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " <archivo.fasta>\n";
        return 1;
    }

    string filename = argv[1];
    auto fastaMap = readFasta(filename);

    vector<string> ids, seqs;
    for (auto &[id, seq] : fastaMap)
    {
        ids.push_back(id);
        seqs.push_back(seq);
    }

    cout << "[*] Calculando matriz de scores...\n";
    auto scoreMatrix = computeScoreMatrix(seqs);

    int starIndex = selectStarSequence(scoreMatrix);
    cout << "[*] Secuencia estrella: " << ids[starIndex] << endl;

    cout << "[*] Alineando contra la estrella...\n";
    auto alignments = alignToStar(seqs, starIndex);

    cout << "[*] Generando alineamiento múltiple...\n";
    auto msa = buildMSA(alignments);

    string outputFile = filename + ".out.txt";
    saveOutput(outputFile, scoreMatrix, ids, alignments, msa);
    cout << "[+] Resultado guardado en: " << outputFile << endl;

    return 0;
}