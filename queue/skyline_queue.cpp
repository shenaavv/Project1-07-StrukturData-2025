#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <chrono> 

using namespace std;
using namespace chrono; 

// Struktur untuk menyimpan data produk
struct Produk {
    int id;
    string nama;
    int harga;    // attr_1 (semakin kecil semakin baik)
    int rating;   // attr_2 (semakin besar semakin baik)
};

// Fungsi untuk membaca data dari file CSV
void bacaData(const string& namaFile, queue<Produk>& antrian) {
    ifstream file(namaFile);
    string line;
    
    // Lewati header
    getline(file, line);
    
    // Membaca data dari file dan menambahkannya ke dalam queue
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
            antrian.push(p);
        }
    }
}

// Fungsi untuk mengecek apakah produk a mendominasi produk b
bool apakahDominasi(const Produk& a, const Produk& b) {
    return (a.harga <= b.harga && a.rating >= b.rating) && 
           (a.harga < b.harga || a.rating > b.rating);
}

// Fungsi utama skyline query menggunakan queue
void cariSkyline(queue<Produk>& antrian, queue<Produk>& hasilSkyline) {
    while (!antrian.empty()) {
        Produk saatIni = antrian.front();
        antrian.pop();
        bool didominasi = false;
        
        // Cek dominasi dengan produk skyline yang sudah ada
        queue<Produk> tempSkyline = hasilSkyline;  // Salin hasil skyline ke sementara
        while (!tempSkyline.empty()) {
            if (apakahDominasi(tempSkyline.front(), saatIni)) {
                didominasi = true;
                break;
            }
            tempSkyline.pop();
        }
        
        if (!didominasi) {
            // Hapus produk skyline yang didominasi oleh produk saat ini
            queue<Produk> skylineBaru;
            while (!hasilSkyline.empty()) {
                if (!apakahDominasi(saatIni, hasilSkyline.front())) {
                    skylineBaru.push(hasilSkyline.front());
                }
                hasilSkyline.pop();
            }
            skylineBaru.push(saatIni);
            hasilSkyline = skylineBaru;
        }
    }
}

int main() {
    queue<Produk> antrian;
    queue<Produk> hasilSkyline;
    
    string namaFile = "ind_1000_2_product.csv";
    
    // Baca data dari file
    bacaData(namaFile, antrian);
    
    // Mulai pengukuran waktu menggunakan chrono
    auto mulai = high_resolution_clock::now();
    
    // Cari produk skyline
    cariSkyline(antrian, hasilSkyline);
    
    // Hitung waktu eksekusi
    auto selesai = high_resolution_clock::now();
    auto waktuEksekusi = duration_cast<duration<double>>(selesai - mulai).count();
    
    // Tampilkan hasil
    cout << "=============================================\n";
    cout << " HASIL SKYLINE QUERY (1000 PRODUK)\n";
    cout << " Menggunakan Struktur Data Queue\n";
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
    for (int i = 0; i < contoh; i++) {
        Produk p = hasilSkyline.front();
        hasilSkyline.pop();
        cout << p.id << "\t" << p.nama 
             << "\t" << p.harga << "\t" << p.rating << endl;
    }
    
    // Opsi: Simpan hasil ke file
    char simpan;
    cout << "\nSimpan hasil lengkap ke file? (y/n): ";
    cin >> simpan;
    
    if (simpan == 'y' || simpan == 'Y') {
        ofstream outFile("hasil_skyline_queue.txt");
        outFile << "Daftar Produk Skyline (Total: " << jumlahSkyline << ")\n";
        outFile << "ID\tLabel\t\tAtrribut 1\tAtrribut 2\n";
        outFile << "----------------------------------------\n";
        
        queue<Produk> tempHasilSkyline = hasilSkyline;
        while (!tempHasilSkyline.empty()) {
            Produk p = tempHasilSkyline.front();
            tempHasilSkyline.pop();
            outFile << p.id << "\t" << p.nama << "\t" 
                    << p.harga << "\t" << p.rating << "\n";
        }
        
        cout << "Hasil telah disimpan ke file 'hasil_skyline_queue.txt'\n";
    }
    
    return 0;
}