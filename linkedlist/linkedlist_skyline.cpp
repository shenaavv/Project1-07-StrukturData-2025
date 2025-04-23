#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;

struct Baju {
    string nama;
    int harga;
    int rating;
};

struct Node {
    Baju data;
    Node* next;
    Node(Baju b) : data(b), next(nullptr) {}
};

class LinkedList {
    Node* head;
public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() { while(head) { Node* temp = head; head = head->next; delete temp; } }
    void add(Baju b) { Node* n = new Node(b); n->next = head; head = n; }
    Baju pop() { 
        Node* temp = head; 
        Baju b = temp->data; 
        head = head->next; 
        delete temp; 
        return b; 
    }
    bool empty() { return head == nullptr; }
};

bool dominasi(Baju a, Baju b) {
    return (a.harga <= b.harga && a.rating >= b.rating) && 
           (a.harga < b.harga || a.rating > b.rating);
}

vector<Baju> skyline(LinkedList& data) {
    vector<Baju> result;
    while(!data.empty()) {
        Baju curr = data.pop();
        bool skip = false;
        
        for(auto& b : result) 
            if(dominasi(b, curr)) { skip = true; break; }
        
        if(!skip) {
            vector<Baju> temp;
            for(auto& b : result)
                if(!dominasi(curr, b)) temp.push_back(b);
            result = temp;
            result.push_back(curr);
        }
    }
    return result;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    LinkedList data;
    ifstream file("ind_1000_2_product.csv");
    string line;
    getline(file, line); // Skip header
    
    while(getline(file, line)) {
        stringstream ss(line);
        string id, nama, h, r;
        getline(ss, id, ',');
        getline(ss, nama, ',');
        getline(ss, h, ',');
        getline(ss, r, ',');
        data.add({nama, stoi(h), stoi(r)});
    }
    
    vector<Baju> hasil = skyline(data);
    
    cout << "=== HASIL SKYLINE ===" << endl;
    for(auto& b : hasil)
        cout << b.nama << " (Rp" << b.harga << ", *" << b.rating << ")\n";
    cout << "\nTotal: " << hasil.size() << " produk\n";
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> waktu = end - start;
    cout << "\nWaktu komputasi: " << waktu.count() << " detik" << endl;
    
    return 0;
}
