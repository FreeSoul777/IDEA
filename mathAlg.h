#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fstream>
#include <locale>
#include <vector>
#include <stdexcept>
#include <string_view>
#include <string>
#include <iomanip>
using namespace std;

uint16_t mult(uint16_t x, uint16_t y);

uint16_t addReversKey(uint16_t x);

uint16_t add(uint16_t x, uint16_t y);

uint16_t multReversKey(uint16_t x);

vector<uint8_t> makeKey(string charKey);

uint16_t glue2Bytes(uint8_t a1, uint8_t a2);

