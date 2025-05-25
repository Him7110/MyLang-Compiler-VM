#ifndef MYLANG_AST_H
#define MYLANG_AST_H

#include <string>
#include <vector>
#include <memory>

namespace MyLang {

// Forward‐declare ExprAST for statements
class ExprAST;

// Base class for all statement nodes
class StmtAST {
public:
    virtual ~StmtAST() = default;
    // Generate bytecode for this statement; code emitted into `out`
    virtual void codegen(std::vector<uint8_t>& out) = 0;
};

// Expression base class
class ExprAST {
public:
    virtual ~ExprAST() = default;
    // Generate bytecode for this expression; code emitted into `out`
    // Leaves the result value on the stack.
    virtual void codegen(std::vector<uint8_t>& out) = 0;
};

// Integer literal: e.g. "42"
class IntegerExprAST : public ExprAST {
    int Value;

public:
    IntegerExprAST(int Val) : Value(Val) {}
    void codegen(std::vector<uint8_t>& out) override;
};

// Variable reference: e.g. "x"
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string& N) : Name(N) {}
    void codegen(std::vector<uint8_t>& out) override;
    const std::string& getName() const { return Name; }
};

// Binary operator: e.g. "lhs + rhs" or "lhs * rhs"
class BinaryExprAST : public ExprAST {
    char Op;  // '+', '-', '*', '/'
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char op,
                  std::unique_ptr<ExprAST> lhs,
                  std::unique_ptr<ExprAST> rhs)
        : Op(op), LHS(std::move(lhs)), RHS(std::move(rhs)) {}

    void codegen(std::vector<uint8_t>& out) override;
};

// Assignment statement: "x = expr;"
class AssignStmtAST : public StmtAST {
    std::string VarName;
    std::unique_ptr<ExprAST> Expr;

public:
    AssignStmtAST(const std::string& name, std::unique_ptr<ExprAST> expr)
        : VarName(name), Expr(std::move(expr)) {}
    void codegen(std::vector<uint8_t>& out) override;
};

// Print statement: "print(expr);"
class PrintStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> Expr;

public:
    PrintStmtAST(std::unique_ptr<ExprAST> expr)
        : Expr(std::move(expr)) {}
    void codegen(std::vector<uint8_t>& out) override;
};

// A sequence of statements (the whole program)
class BlockAST {
    std::vector<std::unique_ptr<StmtAST>> Stmts;

public:
    void addStmt(std::unique_ptr<StmtAST> S) {
        Stmts.push_back(std::move(S));
    }
    void codegen(std::vector<uint8_t>& out) {
        for (auto& S : Stmts)
            S->codegen(out);
        // After all statements, emit HALT
        out.push_back(0x09); // HALT opcode
    }
};

} // namespace MyLang

#endif // MYLANG_AST_H
