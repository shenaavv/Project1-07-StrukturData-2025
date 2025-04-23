#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <chrono> 

using namespace std;
using namespace chrono; // biar gak nulis chrono:: tiap waktu

// Struct untuk menyimpan data produk
struct Product {
    int id;
    string label;
    int attr1;
    int attr2;

    Product(int _id, string _label, int _attr1, int _attr2)
        : id(_id), label(_label), attr1(_attr1), attr2(_attr2) {}

    void display() const {
        cout << "ID: " << id
             << ", Name: " << label
             << ", Attribute 1: " << attr1
             << ", Attribute 2: " << attr2 << endl;
    }
};

// Fungsi untuk membaca CSV ke dalam queue
void loadProductsFromCSV(const string& filename, queue<Product>& productQueue) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

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

        productQueue.push(Product(id, label, attr1, attr2));
    }

    file.close();
}

// Fungsi untuk menampilkan produk dengan waktu eksekusi
void displayProductsWithTiming(queue<Product>& productQueue, int n) {
    auto start = high_resolution_clock::now();

    cout << "ID\tLabel\tAttribute 1\tAttribute 2" << endl;

    queue<Product> tempQueue = productQueue;

    for (int i = 0; i < n && !tempQueue.empty(); ++i) {
        Product current = tempQueue.front();
        tempQueue.pop();
        cout << current.id << "\t" << current.label << "\t" << current.attr1 << "\t" << current.attr2 << endl;
    }

    auto stop = high_resolution_clock::now();
    duration<double> execTime = stop - start;

    cout << "Waktu eksekusi: " << execTime.count() << " detik" << endl;
}

int main() {
    queue<Product> productQueue;

    cout << "Product Queue Processing Program" << endl;
    cout << "================================" << endl;

    auto loadStart = high_resolution_clock::now();
    loadProductsFromCSV("ind_1000_2_product.csv", productQueue);
    auto loadEnd = high_resolution_clock::now();
    duration<double> loadDuration = loadEnd - loadStart;

    cout << "CSV file loaded. Total products: " << productQueue.size() << endl;
    cout << "Loading time: " << loadDuration.count() << " detik" << endl;

    displayProductsWithTiming(productQueue, 10);

    return 0;
}