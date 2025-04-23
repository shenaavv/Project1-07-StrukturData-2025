#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <chrono>
#include <iomanip> // Untuk setprecision

using namespace std;
using namespace chrono;

// Struktur untuk menyimpan data produk
struct Produk {
    int id;
    string nama;
    int harga;    // Semakin kecil semakin baik
    int rating;   // Semakin besar semakin baik

    // Operator perbandingan agar std::map dapat mengurutkan
    bool operator<(const Produk& other) const {
        if (harga == other.harga)
            return rating > other.rating; // Lebih tinggi rating lebih baik
        return harga < other.harga;       // Lebih murah lebih baik
    }
};

// Fungsi membaca data dari file CSV
void bacaData(const string& namaFile, map<Produk, bool>& produkMap) {
    ifstream file(namaFile);
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        size_t pos = 0;
        string tokens[4];
        int idx = 0;

        while ((pos = line.find(',')) != string::npos) {
            tokens[idx++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        tokens[idx] = line;

        if (idx >= 3) {
            Produk p;
            p.id = stoi(tokens[0]);
            p.nama = tokens[1];
            p.harga = stoi(tokens[2]);
            p.rating = stoi(tokens[3]);
            produkMap[p] = true;
        }
    }
}

// Fungsi untuk mengecek apakah produk a mendominasi produk b
bool apakahDominasi(const Produk& a, const Produk& b) {
    return (a.harga <= b.harga && a.rating >= b.rating) &&
           (a.harga < b.harga || a.rating > b.rating);
}

// Fungsi skyline query
void cariSkyline(map<Produk, bool>& produkMap, map<Produk, bool>& hasilSkyline) {
    for (auto& it : produkMap) {
        Produk saatIni = it.first;
        bool didominasi = false;

        for (auto& s : hasilSkyline) {
            if (apakahDominasi(s.first, saatIni)) {
                didominasi = true;
                break;
            }
        }

        if (!didominasi) {
            for (auto it2 = hasilSkyline.begin(); it2 != hasilSkyline.end();) {
                if (apakahDominasi(saatIni, it2->first)) {
                    it2 = hasilSkyline.erase(it2);
                } else {
                    ++it2;
                }
            }
            hasilSkyline[saatIni] = true;
        }
    }
}

int main() {
    map<Produk, bool> produkMap;
    map<Produk, bool> hasilSkyline;
    string namaFile = "ind_1000_2_product.csv";

    bacaData(namaFile, produkMap);

    auto mulai = high_resolution_clock::now();
    cariSkyline(produkMap, hasilSkyline);
    auto selesai = high_resolution_clock::now();

    double waktuEksekusi = duration_cast<duration<double>>(selesai - mulai).count();

    // Tampilkan hasil
    cout << "=============================================\n";
    cout << " HASIL SKYLINE QUERY (1000 PRODUK)\n";
    cout << " Menggunakan Struktur Data Map\n";
    cout << "=============================================\n\n";

    int jumlahSkyline = hasilSkyline.size();
    cout << "Produk skyline ditemukan: " << jumlahSkyline << endl;
    cout << fixed << setprecision(8);
    cout << "Waktu eksekusi: " << waktuEksekusi << " detik\n\n";

    // Tampilkan 10 hasil pertama
    cout << "10 Produk Skyline Pertama:\n";
    cout << "ID\tLabel\t\tHarga\tRating\n";
    cout << "----------------------------------------\n";

    int contoh = min(10, jumlahSkyline);
    int i = 0;
    for (auto& p : hasilSkyline) {
        if (i++ >= contoh) break;
        cout << p.first.id << "\t" << p.first.nama 
             << "\t\t" << p.first.harga << "\t" << p.first.rating << endl;
    }

    // Opsi simpan hasil ke file
    char simpan;
    cout << "\nSimpan hasil lengkap ke file? (y/n): ";
    cin >> simpan;

    if (simpan == 'y' || simpan == 'Y') {
        ofstream outFile("hasil_skyline_map.txt");
        outFile << "Daftar Produk Skyline (Total: " << jumlahSkyline << ")\n";
        outFile << "ID\tLabel\t\tHarga\tRating\n";
        outFile << "----------------------------------------\n";

        for (auto& p : hasilSkyline) {
            outFile << p.first.id << "\t" << p.first.nama 
                    << "\t\t" << p.first.harga << "\t" << p.first.rating << "\n";
        }

        cout << "Hasil telah disimpan ke file 'hasil_skyline_map.txt'\n";
    }

    return 0;
}