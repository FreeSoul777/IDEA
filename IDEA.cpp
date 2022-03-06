#include "IDEA.h"

void IDEA::generateSubkeys() {
    if (key.size() != KEY_SIZE) {
        throw std::runtime_error("Error: Key must be 128 bits in length.");
    }

    uint16_t b1, b2;
    for (size_t i = 0; i < key.size() / 2; i++) {
        subKey[i] = glue2Bytes(key[2 * i], key[2 * i + 1]);
    }

    for (size_t i = key.size() / 2; i < subKey.size(); i++) {
        b1 = subKey[(i + 1) % 8 != 0 ? i - 7 : i - 15] << 9;
        b2 = subKey[(i + 2) % 8 < 2 ? i - 14 : i - 6] >> 7;
        subKey[i] = (b1 | b2) & 0xFFFF;
    }
}

void IDEA::invertSubkeys() {
    vector<uint16_t> reversSubkey(subKey.size());
    uint8_t p = 0;
    uint8_t i = ROUNDS * 6;
    reversSubkey[i]     = multReversKey(subKey[p++]);
    reversSubkey[i + 1] = addReversKey(subKey[p++]);
    reversSubkey[i + 2] = addReversKey(subKey[p++]);
    reversSubkey[i + 3] = multReversKey(subKey[p++]);

    for (uint8_t r = ROUNDS - 1; r > 0; r--) {
        i = r * 6;
        reversSubkey[i + 4] = subKey[p++];
        reversSubkey[i + 5] = subKey[p++];
        reversSubkey[i]     = multReversKey(subKey[p++]);
        reversSubkey[i + 2] = addReversKey(subKey[p++]);
        reversSubkey[i + 1] = addReversKey(subKey[p++]);
        reversSubkey[i + 3] = multReversKey(subKey[p++]);
    }

    reversSubkey[4] = subKey[p++];
    reversSubkey[5] = subKey[p++];
    reversSubkey[0] = multReversKey(subKey[p++]);
    reversSubkey[1] = addReversKey(subKey[p++]);
    reversSubkey[2] = addReversKey(subKey[p++]);
    reversSubkey[3] = multReversKey(subKey[p]);

    subKey = reversSubkey;
}

void IDEA::encrypt(MODE mode, bool flag) {
    cout << endl << "\ndata.size: " << data.size();
    if(data.size() % BLOCK_SIZE != 0) {
        data.insert(data.end(), (BLOCK_SIZE - (data.size() % BLOCK_SIZE)), '0');
        cout << " add, data.size = " << data.size();
    }
    cout << endl;
    if (data.size() % BLOCK_SIZE != 0){
        throw std::runtime_error("\ndata.size() % 8 != 0\n");
    }
    this->flag = flag;
    this->mode = mode;
    switch (this->mode) {
    case CFB: cfb(); break;
    case CBC: cbc(); break;
    case OFB: ofb(); break;
    case ECB: ecb(); break;
    default: cout << "The mode is incorrectly selected.";break;
    }
}

void IDEA::crypt(vector<uint8_t>& data, size_t i) {
    uint16_t d1 = glue2Bytes(data[i + 0], data[i + 1]);
    uint16_t d2 = glue2Bytes(data[i + 2], data[i + 3]);
    uint16_t d3 = glue2Bytes(data[i + 4], data[i + 5]);
    uint16_t d4 = glue2Bytes(data[i + 6], data[i + 7]);
    size_t j = 0;
    for (uint8_t round = 0; round < ROUNDS; round++) {
        uint16_t a = mult(d1, subKey[j++]);
        uint16_t b = add(d2, subKey[j++]);
        uint16_t c = add(d3, subKey[j++]);
        uint16_t d = mult(d4, subKey[j++]);
        uint16_t e = a ^ c;
        uint16_t f = b ^ d;

        uint16_t g = mult(e, subKey[j++]);
        uint16_t s = add(f, g);
        uint16_t h = mult(s, subKey[j++]);
        uint16_t k = add(g, h);

        d1 = a ^ h;
        d2 = c ^ h;
        d3 = b ^ k;
        d4 = d ^ k;
    }
    uint16_t res1 = mult(d1, subKey[j++]);
    uint16_t res2 = add(d3, subKey[j++]);
    uint16_t res3 = add(d2, subKey[j++]);
    uint16_t res4 = mult(d4, subKey[j]);

    data[i + 0] = static_cast<uint8_t>(res1 >> 8);
    data[i + 1] = static_cast<uint8_t>(res1);
    data[i + 2] = static_cast<uint8_t>(res2 >> 8);
    data[i + 3] = static_cast<uint8_t>(res2);
    data[i + 4] = static_cast<uint8_t>(res3 >> 8);
    data[i + 5] = static_cast<uint8_t>(res3);
    data[i + 6] = static_cast<uint8_t>(res4 >> 8);
    data[i + 7] = static_cast<uint8_t>(res4);
}

void IDEA::cfb() {
    size_t i = 0;vector<uint8_t> clone(8);
    while(i < data.size()) {
        if(flag) {
            clone.clear();
            clone = vector<uint8_t>(data.begin()+i, data.begin()+i+8);
        }
        crypt(feedback, i);
        for(size_t j = 0; j < BLOCK_SIZE; j++) {
              data[i + j] ^= feedback[j];
              feedback[j] = data[i + j];
        }
        if(flag) {
            feedback.clear();
            feedback = clone;
        }
        i += 8;
    }

    cout << endl << " CFB:\n ";
    for(auto c: data) cout << c << " ";
}

void IDEA::ecb() {
    size_t i = 0;
    if(flag) invertSubkeys();
    while(i < data.size()) {
        crypt(data, i);
        i += 8;
    }
    cout << endl << " ECB:\n ";
    for(auto c: data) cout << c << " ";
}

void IDEA::cbc() {
    size_t i = 0;
    if(!flag){
        while(i < data.size()) {
            for(size_t j = 0; j < BLOCK_SIZE; j++) {
                feedback[j] ^= data[i + j];
            }
            crypt(feedback, i);
            for(size_t j = 0; j < BLOCK_SIZE; j++) {
                data[i + j] = feedback[j];
            }
            i += 8;
        }
    }
    else {
        invertSubkeys();
        vector<uint8_t> clone1, clone2;
        while(i < data.size()) {
            clone1.clear(); clone2.clear();
            clone1 = vector<uint8_t>(data.begin()+i, data.begin()+i+8);
            clone2 = clone1;
            crypt(clone1, i);
            for(size_t j = 0; j < BLOCK_SIZE; j++) {
                feedback[j] ^= clone1[j];
                data[i + j] = feedback[j];
            }
            feedback.clear();
            feedback = clone2;
            i += 8;
        }
    }

    cout << endl << " CBC:\n ";
    for(auto c: data) cout << c << " ";
}

void IDEA::ofb() {
    size_t i = 0;
    while(i < data.size()) {
        crypt(feedback, i);
        for(size_t j = 0; j < BLOCK_SIZE; j++) {
              data[i + j] ^= feedback[j];
        }
        i += 8;
    }
    cout << endl << " OFB:\n ";
    for(auto c: data) cout << c << " ";
}



