#include "CodeGen.h"
#include <cstring> // for memcpy

using namespace MyLang;

// Helper: write a 4‐byte little‐endian int into vector
static void writeInt32(std::vector<uint8_t>& out, int value) {
    uint8_t bytes[4];
    bytes[0] = (uint8_t)(value & 0xFF);
    bytes[1] = (uint8_t)((value >> 8) & 0xFF);
    bytes[2] = (uint8_t)((value >> 16) & 0xFF);
    bytes[3] = (uint8_t)((value >> 24) & 0xFF);
    out.insert(out.end(), bytes, bytes + 4);
}

void CodeGen::emitICONST(std::vector<uint8_t>& out, int value) {
    out.push_back(0x01); // ICONST opcode
    writeInt32(out, value);
}

void CodeGen::emitLOAD_VAR(std::vector<uint8_t>& out, const std::string& name) {
    out.push_back(0x02); // LOAD_VAR opcode
    uint8_t len = (uint8_t)name.size();
    out.push_back(len);
    out.insert(out.end(), name.begin(), name.end());
}

void CodeGen::emitSTORE_VAR(std::vector<uint8_t>& out, const std::string& name) {
    out.push_back(0x03); // STORE_VAR opcode
    uint8_t len = (uint8_t)name.size();
    out.push_back(len);
    out.insert(out.end(), name.begin(), name.end());
}
