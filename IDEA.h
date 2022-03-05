#include "mathAlg.h"

static const uint8_t BLOCK_SIZE = 8;
static const uint8_t ROUNDS = 8;
static const uint8_t KEY_SIZE = 16;

enum MODE{CFB, CBC, OFB, ECB};
class IDEA {
private:
    vector<uint8_t> data;
    vector<uint8_t> key;
    vector<uint16_t> subKey;
    MODE mode;
    bool flag;
public:
    IDEA(string key, string data) {
        this->data = makeKey(data);
        this->key = makeKey(key);
        subKey = vector<uint16_t>(ROUNDS * 6 + 4);
        generateSubkeys();
    }
    IDEA(string key, vector<uint8_t> data) {
        this->data = data;
        this->key = makeKey(key);
        subKey = vector<uint16_t>(ROUNDS * 6 + 4);
        generateSubkeys();
    }
    IDEA(vector<uint8_t> key, vector<uint8_t> data) {
        this->data = data;
        this->key = key;
        subKey = vector<uint16_t>(ROUNDS * 6 + 4);
        generateSubkeys();
    }
    void setKey(string key) {
        this->key = makeKey(key);
        subKey = vector<uint16_t>(ROUNDS * 6 + 4);
        generateSubkeys();
    }
    void setData(string data) {
        this->data = makeKey(data);
    }
    void generateSubkeys();
    void invertSubkeys();
    void encrypt(MODE mode, bool flag);
    vector<uint8_t> getDATA() {return data;}
    vector<uint8_t> getKey() {return key;}
    vector<uint16_t> getSubKey() {return subKey;}

//    void crypt(vector<uint8_t>& data);
    void crypt(vector<uint8_t>& data, size_t i);

    void cfb();
    void ecb();
    void ofb();
    void cbc();
};
