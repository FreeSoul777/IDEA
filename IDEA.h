#include "mathAlg.h"
#ifndef IDEA_H
#define IDEA_H
#define BLOCK_SIZE 8
#define ROUNDS 8
#define KEY_SIZE 16

enum MODE{CFB, CBC, OFB, ECB};
class IDEA {
private:
    vector<uint8_t> data;
    vector<uint8_t> key;
    vector<uint16_t> subKey;
    vector<uint8_t> feedback;
    MODE mode;
    bool flag;
public:
    IDEA() {
        this->key.clear();
        this->data.clear();
        this->subKey.clear();
        this->feedback.clear();
        this->mode = ECB;
        this->flag = true;
    }
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

    void setFeedBack(vector<uint8_t> feedback) {
        this->feedback = feedback;
    }
    void setFeedBack(string feedback) {
        this->feedback = makeKey(feedback);
    }
    void setKey(string key) {
        this->key = makeKey(key);
        subKey = vector<uint16_t>(ROUNDS * 6 + 4);
        generateSubkeys();
    }
    void setData(vector<uint8_t> data) {
        this->data = data;
    }
    void setData(string data) {
        this->data = makeKey(data);
    }
    void setMode(MODE mode) {this->mode = mode;}
    void setFlag(bool flag) {this->flag = flag;}

    vector<uint8_t> getDATA() {return data;}
    vector<uint8_t> getKey() {return key;}
    vector<uint8_t> getFeedback() {return feedback;}
    vector<uint16_t> getSubKey() {return subKey;}
    MODE getMode() {return mode;}
    bool getFlag() {return flag;}

    void crypt(vector<uint8_t>& data, size_t i);
    void generateSubkeys();
    void invertSubkeys();
    void encrypt();
    void cfb();
    void ecb();
    void ofb();
    void cbc();
};

#endif
