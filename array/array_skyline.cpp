#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct Product {
    int id;
    string label;
    double attr_1;  // Misal: harga
    double attr_2;  // Misal: rating
};

// Fungsi untuk memeriksa dominasi
bool isDominating(const Product &a, const Product &b) {
    return (a.attr_1 <= b.attr_1 && a.attr_2 <= b.attr_2) && 
           (a.attr_1 < b.attr_1 || a.attr_2 < b.attr_2);
}

// Fungsi untuk membaca data dari CSV
void readCSV(const string &filename, Product data[], int &size) {
    ifstream file(filename);
    string line;
    size = 0;

    if (!file.is_open()) {
        cerr << "Error: Tidak dapat membuka file " << filename << endl;
        exit(1);
    }

    // Lewati header
    getline(file, line);

    while (getline(file, line) && size < 1000) {
        stringstream ss(line);
        string token;
        
        // Baca ID
        getline(ss, token, ',');
        data[size].id = stoi(token);

        // Baca Label
        getline(ss, data[size].label, ',');

        // Baca Attr_1
        getline(ss, token, ',');
        data[size].attr_1 = stod(token);

        // Baca Attr_2
        getline(ss, token, ',');
        data[size].attr_2 = stod(token);

        size++;
    }

    file.close();
}


vector<Product> skylineQueryTop9(Product data[], int size) {
    vector<Product> skyline;
    
    for (int i = 0; i < size; i++) {
        bool dominated = false;
        
        
        for (int j = 0; j < skyline.size(); j++) {
            if (isDominating(skyline[j], data[i])) {
                dominated = true;
                break;
            }
        }
        
        if (!dominated) {
            
            vector<Product> newSkyline;
            for (const Product &p : skyline) {
                if (!isDominating(data[i], p)) {
                    newSkyline.push_back(p);
                }
            }
            newSkyline.push_back(data[i]);
            skyline = newSkyline;
            
            
            if (skyline.size() > 9) {
                
                sort(skyline.begin(), skyline.end(), 
                    [](const Product &a, const Product &b) {
                        return a.attr_1 < b.attr_1;
                    });
                
                skyline.pop_back();
            }
        }
    }

    
    sort(skyline.begin(), skyline.end(), 
        [](const Product &a, const Product &b) {
            return a.attr_1 < b.attr_1;
        });

    return skyline;
}

int main() {
    const int MAX_SIZE = 1000;
    Product data[MAX_SIZE];
    int dataSize = 0;
    
    
    readCSV("ind_1000_2_product.csv", data, dataSize);
    
    
    auto start = high_resolution_clock::now();
    vector<Product> result = skylineQueryTop9(data, dataSize);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    
    cout << "=== HASIL SKYLINE QUERY (TOP 9) ===" << endl;
    cout << "Total data diproses: " << dataSize << endl;
    cout << "ID\tLabel\t\tAttr_1\tAttr_2" << endl;
    cout << "----------------------------------------" << endl;
    
    for (const Product &p : result) {
        cout << p.id << "\t" << p.label << "\t" << p.attr_1 << "\t" << p.attr_2 << endl;
    }

    cout << "\n=== STATISTIK ===" << endl;
    cout << "Jumlah produk dalam skyline: " << result.size() << endl;
    cout << "Waktu eksekusi: " << duration.count() << " mikrodetik" << endl;

    return 0;
}