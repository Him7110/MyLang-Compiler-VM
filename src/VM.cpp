#include "VM.h"
#include <iostream>
#include <stdexcept>
#include <cstring> // memcpy

using namespace MyLang;

int VM::pop() {
    if (stack.empty())
        throw std::runtime_error("VM Error: pop from empty stack");
    int v = stack.back();
    stack.pop_back();
    return v;
}

void VM::run(const std::vector<uint8_t>& bc) {
    size_t ip = 0; // instruction pointer
    while (ip < bc.size()) {
        uint8_t op = bc[ip++];
        switch (op) {
        case 0x01: { // ICONST
            if (ip + 4 > bc.size())
                throw std::runtime_error("VM: truncated ICONST");
            int value;
            std::memcpy(&value, &bc[ip], 4);
            ip += 4;
            stack.push_back(value);
            break;
        }
        case 0x02: { // LOAD_VAR
            if (ip >= bc.size())
                throw std::runtime_error("VM: truncated LOAD_VAR length");
            uint8_t len = bc[ip++];
            if (ip + len > bc.size())
                throw std::runtime_error("VM: truncated LOAD_VAR name");
            std::string name((char*)&bc[ip], len);
            ip += len;
            int v = 0;
            if (vars.find(name) != vars.end())
                v = vars[name];
            stack.push_back(v);
            break;
        }
        case 0x03: { // STORE_VAR
            if (ip >= bc.size())
                throw std::runtime_error("VM: truncated STORE_VAR length");
            uint8_t len = bc[ip++];
            if (ip + len > bc.size())
                throw std::runtime_error("VM: truncated STORE_VAR name");
            std::string name((char*)&bc[ip], len);
            ip += len;
            int v = pop();
            vars[name] = v;
            break;
        }
        case 0x04: { // ADD
            int b = pop();
            int a = pop();
            stack.push_back(a + b);
            break;
        }
        case 0x05: { // SUB
            int b = pop();
            int a = pop();
            stack.push_back(a - b);
            break;
        }
        case 0x06: { // MUL
            int b = pop();
            int a = pop();
            stack.push_back(a * b);
            break;
        }
        case 0x07: { // DIV
            int b = pop();
            int a = pop();
            if (b == 0)
                throw std::runtime_error("VM: division by zero");
            stack.push_back(a / b);
            break;
        }
        case 0x08: { // PRINT
            int v = pop();
            std::cout << v << "\n";
            break;
        }
        case 0x09: { // HALT
            return;
        }
        default:
            throw std::runtime_error("VM: unknown opcode");
        }
    }
}
