#include "mathAlg.h"

uint16_t mult(uint16_t x, uint16_t y) {
    uint32_t m = static_cast <uint32_t>(x * y) % 0x10001;
    if (m != 0) return m  & 0xFFFF;
    else if (x != 0 || y != 0) return (1 - x - y) & 0xFFFF;
    return 1;
}

uint16_t addReversKey(uint16_t x) {
    return (0x10000 - x) & 0xFFFF;
}

uint16_t add(uint16_t x, uint16_t y) {
    return (x + y) & 0xFFFF;
}

uint16_t multReversKey(uint16_t x) {
    uint64_t n = 65535;
    uint64_t a = static_cast<uint64_t>(x);
    uint64_t r = 1;
    while(n) {
        if(n&1) r = (r*a)%65537;
        n >>= 1;
        a = (a*a)%65537;
    }
    return static_cast<uint16_t>(r) & 0xffff;
}

vector<uint8_t> makeKey(string charKey) {
    vector<uint8_t> key(charKey.size());
    for(uint8_t i = 0; i < charKey.size(); ++i) key[i] = static_cast<uint8_t> (charKey[i]);
    return key;
}

uint16_t glue2Bytes(uint8_t a1, uint8_t a2) {
    uint16_t b1 = static_cast<uint16_t> (a1);
    uint16_t b2 = static_cast<uint16_t> (a2);
    b1 = (b1 & 0xFF) << 8;  // xxxxxxxx00000000
    b2 = b2 & 0xFF;         // 00000000xxxxxxxx
    return (b1 | b2);       // xxxxxxxxxxxxxxxx
}
