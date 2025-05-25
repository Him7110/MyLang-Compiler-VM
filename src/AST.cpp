#include "AST.h"
#include "CodeGen.h"  // We’ll use helper functions to emit instructions

using namespace MyLang;

// IntegerExprAST: emit ICONST <4‐byte value>
void IntegerExprAST::codegen(std::vector<uint8_t>& out) {
    CodeGen::emitICONST(out, Value);
}

// VariableExprAST: emit LOAD_VAR <name>
void VariableExprAST::codegen(std::vector<uint8_t>& out) {
    CodeGen::emitLOAD_VAR(out, Name);
}

// BinaryExprAST: generate LHS, generate RHS, then emit the correct opcode
void BinaryExprAST::codegen(std::vector<uint8_t>& out) {
    LHS->codegen(out);
    RHS->codegen(out);
    switch (Op) {
    case '+':
        out.push_back(0x04);  // ADD
        break;
    case '-':
        out.push_back(0x05);  // SUB
        break;
    case '*':
        out.push_back(0x06);  // MUL
        break;
    case '/':
        out.push_back(0x07);  // DIV
        break;
    default:
        // Unexpected operator; do nothing
        break;
    }
}

// AssignStmtAST: generate code for Expr, then STORE_VAR <name>
void AssignStmtAST::codegen(std::vector<uint8_t>& out) {
    Expr->codegen(out);
    CodeGen::emitSTORE_VAR(out, VarName);
}

// PrintStmtAST: generate code for Expr, then PRINT
void PrintStmtAST::codegen(std::vector<uint8_t>& out) {
    Expr->codegen(out);
    out.push_back(0x08);  // PRINT
}
