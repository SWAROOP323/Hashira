#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <sstream>

using namespace std;
using namespace Eigen;

long long convertToDecimal(const string& number, int base) {
    long long result = 0;
    for (char c : number) {
        int digit = 0;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F') digit = 10 + (c - 'A');
        result = result * base + digit;
    }
    return result;
}

int main() {
    ifstream inputFile("input.json");
    string jsonText((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    map<int, pair<int, string>> rootsData;
    int n = 0, k = 0;

    size_t pos = 0;
    while ((pos = jsonText.find("\"keys\"")) != string::npos) {
        size_t start = jsonText.find("{", pos);
        size_t end = jsonText.find("}", start);
        string keysStr = jsonText.substr(start + 1, end - start - 1);
        size_t nPos = keysStr.find("\"n\"");
        size_t kPos = keysStr.find("\"k\"");
        if (nPos != string::npos) {
            size_t colon = keysStr.find(":", nPos);
            size_t comma = keysStr.find(",", colon);
            n = stoi(keysStr.substr(colon + 1, comma - colon - 1));
        }
        if (kPos != string::npos) {
            size_t colon = keysStr.find(":", kPos);
            size_t comma = keysStr.find("\n", colon);
            k = stoi(keysStr.substr(colon + 1, comma - colon - 1));
        }
        break;
    }

    for (int i = 1; i <= n; i++) {
        string keyStr = "\"" + to_string(i) + "\"";
        size_t keyPos = jsonText.find(keyStr);
        if (keyPos == string::npos) continue;
        size_t basePos = jsonText.find("\"base\"", keyPos);
        size_t baseStart = jsonText.find("\"", basePos + 6) + 1;
        size_t baseEnd = jsonText.find("\"", baseStart);
        string baseStr = jsonText.substr(baseStart, baseEnd - baseStart);
        int base = stoi(baseStr);

        size_t valuePos = jsonText.find("\"value\"", keyPos);
        size_t valueStart = jsonText.find("\"", valuePos + 7) + 1;
        size_t valueEnd = jsonText.find("\"", valueStart);
        string valueStr = jsonText.substr(valueStart, valueEnd - valueStart);

        rootsData[i] = make_pair(base, valueStr);
    }

    vector<long long> rootsDecimal;
    for (int i = 1; i <= n; i++) {
        int base = rootsData[i].first;
        string val = rootsData[i].second;
        rootsDecimal.push_back(convertToDecimal(val, base));
    }

    int degree = k - 1;
    MatrixXd matrix(k, k);
    VectorXd vector(k);

    for (int i = 0; i < k; i++) {
        long long x = rootsDecimal[i];
        long long y = i + 1;
        long long val = 1;
        for (int j = 0; j < k; j++) {
            matrix(i, j) = val;
            val *= x;
        }
        vector(i) = y;
    }

    VectorXd coefficients = matrix.colPivHouseholderQr().solve(vector);

    for (int i = 0; i < k; i++) {
        cout << "Coefficient a" << i << " = " << coefficients(i) << endl;
    }

    return 0;
}
