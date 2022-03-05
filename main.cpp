#define _CRT_SECURE_NO_WARNINGS
#include "IDEA.h"
using namespace std;


int main() {
    cout << "Hello! This is IDEA.\n";

    vector<uint8_t> data;

    string input, output;
    I:cout << "\nEnter the path to the file to be read: ";
    cin >> input;
    ifstream fin(input, ios::binary);
    if (!fin) {
        cout << "Error! The file was not found. Exit? (1/0): ";
        unsigned short int c; cin >> c;
        if(c == 1) exit(1);
        else goto I;
    }
    while(1) {
        uint8_t c;
        fin >> c;
        if(fin.eof()) break;
        data.push_back(c);
    }
    fin.close();

    O:cout << "\nEnter the path to the file to be write: ";
    cin >> output;
    ofstream fout(output, ios::binary);
    if (!fout) {
        cout << "Error! The file was not found. Exit? (1/0): ";
        unsigned short int c; cin >> c;
        if(c == 1) exit(1);
        else goto O;
    }

    cout << "\nPlease enter a 128 bit key: ";
    const int SIZE = 16;
    string key = "";
    do {
        getline(cin, key);
        if(key.size() != SIZE)
            cout << "\nThe entered string exceeds the specified size. Please repeat the input: ";
    } while(key.size() != SIZE);

    MODE mode = CFB;
    int m = mode;
    cout << "\nPlease enter one of the modes (1=CFB, 2=CBC, 3=OFB, 4=ECB): ";
    M:cin >> m;
    if(m<1 || m>4) {
        cout << "Incorrect mode is entered. Exit? (1/0): ";
        unsigned short int c; cin >> c;
        if(c == 1) exit(1);
        else goto M;
    }

    bool flag;
    cout << "\nEncrypt or Decrypt? (1/0): ";
    cin >> flag;


    IDEA idea(key, data);
    idea.encrypt(mode, !flag);

    data = idea.getDATA();

    for(size_t i = 0; i < data.size(); ++i) {
        fout << data[i];
    }
    fout.close();
    return 0;
}
