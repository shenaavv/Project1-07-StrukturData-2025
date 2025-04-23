#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

struct Baju
{
    int harga;
    int rating;
};

bool mendominasi(Baju a, Baju b)
{
    return (a.harga <= b.harga && a.rating <= b.rating) &&
           (a.harga < b.harga || a.rating < b.rating);
}

vector<Baju> skylineQueryHashTable(const vector<Baju> &data)
{
    unordered_map<int, Baju> skylineTable;

    for (const auto &baju : data)
    {
        bool isDominated = false;

        for (auto it = skylineTable.begin(); it != skylineTable.end();)
        {
            const Baju &b = it->second;

            if (b.harga <= baju.harga && b.rating >= baju.rating &&
                (b.harga < baju.harga || b.rating > baju.rating))
            {
                isDominated = true;
                break;
            }
            else if (baju.harga <= b.harga && baju.rating >= b.rating &&
                     (baju.harga < b.harga || baju.rating > b.rating))
            {
                it = skylineTable.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (!isDominated)
        {
            skylineTable[baju.harga] = baju;
        }
    }

    vector<Baju> skyline;
    for (auto &pair : skylineTable)
    {
        skyline.push_back(pair.second);
    }

    return skyline;
}

vector<Baju> bacaDataCSV(const string &namaFile)
{
    vector<Baju> data;
    ifstream file(namaFile);
    string line;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string id, name, attr1, attr2;

        if (!getline(ss, id, ','))
            continue;
        if (!getline(ss, name, ','))
            continue;
        if (!getline(ss, attr1, ','))
            continue;
        if (!getline(ss, attr2, ','))
            continue;

        try
        {
            int harga = stoi(attr1);
            int rating = stoi(attr2);
            data.push_back({harga, rating});
        }
        catch (const invalid_argument &e)
        {
            continue;
        }
    }

    return data;
}

int main()
{
    string namaFile = "ind_1000_2_product.csv";

    vector<Baju> data = bacaDataCSV(namaFile);

    auto start = chrono::high_resolution_clock::now();

    auto skyline = skylineQueryHashTable(data);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;

    cout << "Hasil Skyline Query:\n";
    for (auto &baju : skyline)
    {
        cout << "Harga: " << baju.harga << ", Rating: " << baju.rating << "\n";
    }
    cout << "\nJumlah Skyline yang didapat: " << skyline.size() << endl;
    cout << "Jumlah data yang dibaca: " << data.size() << endl;
    cout << "Jumlah data yang tidak terduplikasi: " << skyline.size() << endl;
    cout << "Jumlah data yang terduplikasi: " << data.size() - skyline.size() << endl;
    cout << "Waktu eksekusi: " << duration.count() << " detik" << endl;

    return 0;
}
