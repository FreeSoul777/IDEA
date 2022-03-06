#define _CRT_SECURE_NO_WARNINGS
#include "IDEA.h"
using namespace std;

void transform(string& a) {
    string b = "";
    for(size_t i=2; i < a.size()-1; i+=2) {
        string s = "0x" + string(a.begin()+i, a.begin()+i+2);
        unsigned int x = stoul(s, nullptr, 16);
        b += (char) x;
    }
    a = b;
}

int main() {
    cout << "Hello! This is IDEA.\n";

//    vector<uint8_t> feedback = {0xbb, 0xbb, 0xcc, 0xcc, 0x44, 0x44, 0x22, 0x22};
    string input, output;
    I:cout << "\nEnter the path to the file to be read: ";
    cin >> input;
    ifstream fin(input, ios::binary);
    if (!fin) {
        cout << "Error! The file was not found. Exit? (0/1): ";
        unsigned short int c; cin >> c;
        if(c == 0) exit(1);
        else goto I;
    }
    O:cout << "\nEnter the path to the file to be write: ";
    cin >> output;
    ofstream fout(output, ios::binary);
    if (!fout) {
        cout << "Error! The file was not found. Exit? (0/1): ";
        unsigned short int c; cin >> c;
        if(c == 0) exit(1);
        else goto O;
    }

    cout << "\nPlease enter a 128 bit key: ";
    string key = "";
    do {
       getline(cin, key, '\n');
       if(key[0] == '0' && key[1] == 'x' && key.size() == 34){transform(key); break;}
       else if(key.size() == 16) break;
       else cout << "\nThe entered string exceeds the specified size. Please repeat the input: ";
   } while(true);

    cout << "\nPlease enter a 64 bit feedback: ";
    string feedback = "";
    do {
       getline(cin, feedback, '\n');
       if(feedback[0] == '0' && feedback[1] == 'x' && feedback.size() == 18){transform(feedback); break;}
       else if(feedback.size() == 8) break;
       else cout << "\nThe entered string exceeds the specified size. Please repeat the input: ";
   } while(true);

    MODE mode = CFB;
    int m = mode;
    cout << "\nPlease enter one of the modes (1=CFB, 2=CBC, 3=OFB, 4=ECB): ";
    M:cin >> m;
    if(m<1 || m>4) {
        cout << "Incorrect mode is entered. Exit? (0/1): ";
        unsigned short int c; cin >> c;
        if(c == 0) exit(1);
        else goto M;
    }

    bool flag ;
    cout << "\nEncrypt or Decrypt? (1/0): ";
    cin >> flag;

    IDEA idea(key);
    idea.setFeedBack(feedback);

    vector<uint8_t> data;

    while(1) {
        uint8_t c;
        fin.read((char*)&c, sizeof(c));
        if(fin.eof()) break;
        data.push_back(c);

        if(data.size() == BLOCK_SIZE) {
            idea.setData(data);
            idea.encrypt(mode, !flag);
            data = idea.getDATA();
            for(size_t i = 0; i < data.size(); ++i) {
                fout << data[i];
            }
            data.clear();
        }
    }

    if(!data.empty()){

        idea.setData(data);
        idea.encrypt(mode, !flag);
        data = idea.getDATA();
        for(size_t i = 0; i < data.size(); ++i) {
            fout.write((char*)&data[i], sizeof (uint8_t));
        }
        data.clear();

    }

    fin.close();
    fout.close();
    return 0;
}
