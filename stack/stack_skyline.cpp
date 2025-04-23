#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;

struct Baju {
    string label;
    int harga;
    int rating;

    void print() const {
        cout << label << " | Harga: " << harga << ", Rating: " << rating << endl;
    }
};

bool mendominasi(const Baju& a, const Baju& b) {
    return (a.harga <= b.harga && a.rating >= b.rating) &&
           (a.harga < b.harga || a.rating > b.rating);
}

stack<Baju> loadData(const string& filename) {
    stack<Baju> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Gagal membuka file: " << filename << endl;
        return data;
    }

    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string temp, label;
        int harga, rating;

        getline(ss, temp, ','); // id
        getline(ss, label, ',');
        getline(ss, temp, ','); harga = stoi(temp);
        getline(ss, temp, ','); rating = stoi(temp);

        data.push({label, harga, rating});
    }

    return data;
}

vector<Baju> skylineQuery(stack<Baju> dataStack) {
    vector<Baju> skyline;
    while (!dataStack.empty()) {
        Baju kandidat = dataStack.top();
        dataStack.pop();

        bool didominasi = false;
        for (const Baju& b : skyline) {
            if (mendominasi(b, kandidat)) {
                didominasi = true;
                break;
            }
        }

        if (!didominasi) {
            vector<Baju> temp;
            for (const Baju& b : skyline) {
                if (!mendominasi(kandidat, b)) {
                    temp.push_back(b);
                }
            }
            skyline = temp;
            skyline.push_back(kandidat);
        }
    }
    return skyline;
}

void printSkyline(const vector<Baju>& skyline) {
    cout << "Skyline Set:\n";
    for (const Baju& b : skyline) {
        b.print();
    }
    cout << "\nTotal Produk di Skyline: " << skyline.size() << endl;
}

int main() {
    stack<Baju> originalData = loadData("ind_1000_2_product.csv");

    if (originalData.empty()) {
        return 1; // data tidak berhasil dibaca
    }

    auto start = chrono::high_resolution_clock::now();

    stack<Baju> tempStack = originalData;
    vector<Baju> finalSkyline = skylineQuery(tempStack);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> durasi = end - start;

    printSkyline(finalSkyline);

    cout << fixed << setprecision(10);
    cout << "Waktu komputasi (detik): " << durasi.count() << endl;

    chrono::duration<double, std::milli> ms = end - start;
    cout << "Waktu komputasi (milidetik): " << fixed << setprecision(4) << ms.count() << " ms" << endl;

    return 0;
}
