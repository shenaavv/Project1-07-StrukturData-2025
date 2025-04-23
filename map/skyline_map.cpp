#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <chrono> 

using namespace std;
using namespace chrono;

struct Product {
    int id;
    string label;
    int attr1;
    int attr2;

    Product() {}
    Product(int _id, const string& _label, int _attr1, int _attr2)
        : id(_id), label(_label), attr1(_attr1), attr2(_attr2) {}
};

void loadProductsFromCSV(const string& filename, map<int, Product>& productMap) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Gagal membuka file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // lewati header

    while (getline(file, line)) {
        stringstream ss(line);
        string item;

        int id, attr1, attr2;
        string label;

        getline(ss, item, ',');
        id = stoi(item);

        getline(ss, label, ',');

        getline(ss, item, ',');
        attr1 = stoi(item);

        getline(ss, item, ',');
        attr2 = stoi(item);

        productMap[id] = Product(id, label, attr1, attr2);
    }

    file.close();
}

void displayProductsWithTiming(const map<int, Product>& productMap, int n) {
    auto start = high_resolution_clock::now();

    cout << "ID\tLabel\tattribute 1\tattribute 2" << endl;

    int count = 0;
    for (const auto& pair : productMap) {
        if (count >= n) break;
        const Product& p = pair.second;
        cout << p.id << "\t" << p.label << "\t" << p.attr1 << "\t\t" << p.attr2 << endl;
        count++;
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    cout << "waktu eksekusi: " << duration.count() << " detik" << endl;
}

int main() {
    map<int, Product> productMap;

    cout << "product MAP processing program" << endl;
    cout << "==============================" << endl;

    auto loadStart = high_resolution_clock::now();
    loadProductsFromCSV("ind_1000_2_product.csv", productMap);
    auto loadEnd = high_resolution_clock::now();
    duration<double> loadDuration = loadEnd - loadStart;

    cout << "csv file loaded. total products: " << productMap.size() << endl;
    cout << "loading time: " << loadDuration.count() << " detik" << endl;

    displayProductsWithTiming(productMap, 10);

    return 0;
}