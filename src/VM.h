#ifndef MYLANG_VM_H
#define MYLANG_VM_H

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace MyLang {

/// A tiny stack‐based VM that interprets our custom bytecode.
class VM {
    std::vector<int> stack;  // The operand stack
    std::unordered_map<std::string, int> vars; // Variable store

public:
    VM() = default;

    /// Execute the given bytecode until HALT.
    void run(const std::vector<uint8_t>& bytecode);

private:
    /// Pop one integer from stack (error if empty)
    int pop();
};

} // namespace MyLang

#endif // MYLANG_VM_H
