#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

struct Produk
{
    int id;
    string nama;
    int harga;
    int rating;
};

void bacaData(const string &namaFile, Produk produkArray[], int &size)
{
    ifstream file(namaFile);
    string line;
    getline(file, line);

    while (getline(file, line))
    {
        size_t pos = 0;
        string tokens[4];
        int idx = 0;

        while ((pos = line.find(',')) != string::npos)
        {
            tokens[idx++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        tokens[idx] = line;

        if (idx >= 3)
        {
            Produk p;
            p.id = stoi(tokens[0]);
            p.nama = tokens[1];
            p.harga = stoi(tokens[2]);
            p.rating = stoi(tokens[3]);
            produkArray[size++] = p;
        }
    }
}

bool apakahDominasi(const Produk &a, const Produk &b)
{
    return (a.harga <= b.harga && a.rating >= b.rating) &&
           (a.harga < b.harga || a.rating > b.rating);
}

void cariSkyline(Produk produkArray[], int size, Produk hasilSkyline[], int &hasilSize)
{
    for (int i = 0; i < size; ++i)
    {
        Produk saatIni = produkArray[i];
        bool didominasi = false;

        for (int j = 0; j < hasilSize; ++j)
        {
            if (apakahDominasi(hasilSkyline[j], saatIni))
            {
                didominasi = true;
                break;
            }
        }

        if (!didominasi)
        {
            for (int j = 0; j < hasilSize;)
            {
                if (apakahDominasi(saatIni, hasilSkyline[j]))
                {
                    for (int k = j; k < hasilSize - 1; ++k)
                    {
                        hasilSkyline[k] = hasilSkyline[k + 1];
                    }
                    --hasilSize;
                }
                else
                {
                    ++j;
                }
            }
            hasilSkyline[hasilSize++] = saatIni;
        }
    }
}

int main()
{
    const int maxSize = 1000;
    Produk produkArray[maxSize];
    Produk hasilSkyline[maxSize];
    int size = 0;
    int hasilSize = 0;
    string namaFile = "ind_1000_2_product.csv";

    bacaData(namaFile, produkArray, size);

    auto mulai = high_resolution_clock::now();
    cariSkyline(produkArray, size, hasilSkyline, hasilSize);
    auto selesai = high_resolution_clock::now();

    double waktuEksekusi = duration_cast<duration<double>>(selesai - mulai).count();

    cout << "=============================================\n";
    cout << " HASIL SKYLINE QUERY (1000 PRODUK)\n";
    cout << " Menggunakan Struktur Data Array\n";
    cout << "=============================================\n\n";

    int jumlahSkyline = hasilSize;
    cout << "Produk skyline ditemukan: " << jumlahSkyline << endl;
    cout << fixed << setprecision(8);
    cout << "Waktu eksekusi: " << waktuEksekusi << " detik\n\n";

    cout << "10 Produk Skyline Pertama:\n";
    cout << "ID\tLabel\t\tHarga\tRating\n";
    cout << "----------------------------------------\n";

    int contoh = min(10, jumlahSkyline);
    for (int i = 0; i < contoh; ++i)
    {
        cout << hasilSkyline[i].id << "\t" << hasilSkyline[i].nama
             << "\t\t" << hasilSkyline[i].harga << "\t" << hasilSkyline[i].rating << endl;
    }

    char simpan;
    cout << "\nSimpan hasil lengkap ke file? (y/n): ";
    cin >> simpan;

    if (simpan == 'y' || simpan == 'Y')
    {
        ofstream outFile("hasil_skyline_array.txt");
        outFile << "Daftar Produk Skyline (Total: " << jumlahSkyline << ")\n";
        outFile << "ID\tLabel\t\tHarga\tRating\n";
        outFile << "----------------------------------------\n";

        for (int i = 0; i < jumlahSkyline; ++i)
        {
            outFile << hasilSkyline[i].id << "\t" << hasilSkyline[i].nama
                    << "\t\t" << hasilSkyline[i].harga << "\t" << hasilSkyline[i].rating << "\n";
        }

        cout << "Hasil telah disimpan ke file 'hasil_skyline_array.txt'\n";
    }

    return 0;
}
