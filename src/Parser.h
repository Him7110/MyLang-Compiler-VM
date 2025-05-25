#ifndef MYLANG_PARSER_H
#define MYLANG_PARSER_H

#include "Lexer.h"
#include "AST.h"
#include <memory>

namespace MyLang {

class Parser {
    Lexer Lex;
    Token CurTok;

public:
    Parser(const std::string& src) : Lex(src) {
        CurTok = Lex.getNextToken();
    }

    // Parse whole program into a BlockAST
    std::unique_ptr<BlockAST> parseProgram();

private:
    void getNextToken() { CurTok = Lex.getNextToken(); }
    bool isToken(TokenType t) const { return CurTok.Type == t; }

    // Parsing helpers
    std::unique_ptr<StmtAST> parseStatement();
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<ExprAST> parsePrimary();
    std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrec,
                                          std::unique_ptr<ExprAST> LHS);
    int getTokPrecedence();

    // Utility: consume a token, error if not matching
    bool expect(TokenType expected, const std::string& errMsg);
};

} // namespace MyLang

#endif // MYLANG_PARSER_H
