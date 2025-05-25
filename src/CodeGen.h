#ifndef MYLANG_CODEGEN_H
#define MYLANG_CODEGEN_H

#include <vector>
#include <string>
#include <cstdint>

namespace MyLang {

/// Helper functions to emit instructions into a byte‐vector.
class CodeGen {
public:
    /// Emit ICONST <4‐byte signed int>
    static void emitICONST(std::vector<uint8_t>& out, int value);

    /// Emit LOAD_VAR <1‐byte name_len> <bytes name>
    static void emitLOAD_VAR(std::vector<uint8_t>& out, const std::string& name);

    /// Emit STORE_VAR <1‐byte name_len> <bytes name>
    static void emitSTORE_VAR(std::vector<uint8_t>& out, const std::string& name);
};

} // namespace MyLang

#endif // MYLANG_CODEGEN_H
