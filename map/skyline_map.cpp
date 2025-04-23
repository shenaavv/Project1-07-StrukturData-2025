#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <chrono>

using namespace std;
using namespace chrono;

// Struktur untuk menyimpan data produk
struct Produk {
    int id;
    string nama;
    int harga;    // attr_1 (semakin kecil semakin baik)
    int rating;   // attr_2 (semakin besar semakin baik)
    
    // Operator untuk perbandingan berdasarkan harga dan rating
    bool operator<(const Produk& other) const {
        if (harga == other.harga)
            return rating > other.rating; // Rating yang lebih tinggi lebih diutamakan
        return harga < other.harga; // Harga yang lebih rendah lebih diutamakan
    }
};

// Fungsi untuk membaca data dari file CSV
void bacaData(const string& namaFile, map<Produk, bool>& produkMap) {
    ifstream file(namaFile);
    string line;
    
    // Lewati header
    getline(file, line);
    
    // Membaca data dari file dan menambahkannya ke dalam map
    while (getline(file, line)) {
        size_t pos = 0;
        string tokens[4];  // Menyimpan 4 kolom per produk
        int idx = 0;
        
        // Pisahkan baris berdasarkan koma
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
            produkMap[p] = true; // Masukkan produk ke dalam map
        }
    }
}

// Fungsi untuk mengecek apakah produk a mendominasi produk b
bool apakahDominasi(const Produk& a, const Produk& b) {
    return (a.harga <= b.harga && a.rating >= b.rating) && 
           (a.harga < b.harga || a.rating > b.rating);
}

// Fungsi utama skyline query menggunakan map
void cariSkyline(map<Produk, bool>& produkMap, map<Produk, bool>& hasilSkyline) {
    for (auto it = produkMap.begin(); it != produkMap.end(); ++it) {
        Produk saatIni = it->first;
        bool didominasi = false;
        
        // Cek dominasi dengan produk skyline yang sudah ada
        for (auto& s : hasilSkyline) {
            if (apakahDominasi(s.first, saatIni)) {
                didominasi = true;
                break;
            }
        }
        
        if (!didominasi) {
            // Hapus produk skyline yang didominasi oleh produk saat ini
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
    map<Produk, bool> produkMap;      // Menyimpan semua produk dari file
    map<Produk, bool> hasilSkyline;   // Menyimpan produk skyline
    
    string namaFile = "ind_1000_2_product.csv";
    
    // Baca data dari file
    bacaData(namaFile, produkMap);
    
    // Mulai pengukuran waktu menggunakan chrono
    auto mulai = high_resolution_clock::now();
    
    // Cari produk skyline
    cariSkyline(produkMap, hasilSkyline);
    
    // Hitung waktu eksekusi
    auto selesai = high_resolution_clock::now();
    auto waktuEksekusi = duration_cast<duration<double>>(selesai - mulai).count();
    
    // Tampilkan hasil
    cout << "=============================================\n";
    cout << " HASIL SKYLINE QUERY (1000 PRODUK)\n";
    cout << " Menggunakan Struktur Data Map\n";
    cout << "=============================================\n\n";
    
    // Hitung jumlah produk di hasil skyline
    int jumlahSkyline = hasilSkyline.size();
    cout << "Produk skyline ditemukan: " << jumlahSkyline << endl;
    cout << "Waktu eksekusi: " << waktuEksekusi << " detik\n\n";
    
    // Tampilkan 10 produk skyline pertama sebagai contoh
    cout << "10 Produk Skyline Pertama:\n";
    cout << "ID\tLabel\t\tAtrribut 1\tAtrribut 2\n";
    cout << "----------------------------------------\n";
    
    int contoh = min(10, jumlahSkyline);
    int i = 0;
    for (auto& p : hasilSkyline) {
        if (i++ >= contoh) break;
        cout << p.first.id << "\t" << p.first.nama 
             << "\t" << p.first.harga << "\t" << p.first.rating << endl;
    }
    
    // Opsi: Simpan hasil ke file
    char simpan;
    cout << "\nSimpan hasil lengkap ke file? (y/n): ";
    cin >> simpan;
    
    if (simpan == 'y' || simpan == 'Y') {
        ofstream outFile("hasil_skyline_map.txt");
        outFile << "Daftar Produk Skyline (Total: " << jumlahSkyline << ")\n";
        outFile << "ID\tLabel\t\tAtrribut 1\tAtrribut 2\n";
        outFile << "----------------------------------------\n";
        
        for (auto& p : hasilSkyline) {
            outFile << p.first.id << "\t" << p.first.nama << "\t" 
                    << p.first.harga << "\t" << p.first.rating << "\n";
        }
        
        cout << "Hasil telah disimpan ke file 'hasil_skyline_map.txt'\n";
    }
    
    return 0;
}