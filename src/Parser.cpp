#include "Parser.h"
#include <iostream>
#include <unordered_map>

using namespace MyLang;

// Precedence table
static std::unordered_map<TokenType, int> BinopPrecedence = {
    { TokenType::PLUS, 10 },
    { TokenType::MINUS, 10 },
    { TokenType::MUL, 20 },
    { TokenType::DIV, 20 },
};

int Parser::getTokPrecedence() {
    auto it = BinopPrecedence.find(CurTok.Type);
    if (it != BinopPrecedence.end())
        return it->second;
    return -1;
}

bool Parser::expect(TokenType expected, const std::string& errMsg) {
    if (!isToken(expected)) {
        std::cerr << "Error: " << errMsg << " (got '"
                  << CurTok.Lexeme << "')\n";
        return false;
    }
    getNextToken();
    return true;
}

// parseProgram: many statements until EOF
std::unique_ptr<BlockAST> Parser::parseProgram() {
    auto block = std::make_unique<BlockAST>();
    while (!isToken(TokenType::EOF_TOK)) {
        auto stmt = parseStatement();
        if (!stmt) {
            std::cerr << "Stopping parse due to error.\n";
            return nullptr;
        }
        block->addStmt(std::move(stmt));
    }
    return block;
}

// parseStatement: either an assignment or print, followed by ';'
std::unique_ptr<StmtAST> Parser::parseStatement() {
    if (isToken(TokenType::IDENT)) {
        // Could be assignment: IDENT = expr;
        std::string varName = CurTok.Lexeme;
        getNextToken();
        if (!expect(TokenType::ASSIGN, "Expected '=' after identifier"))
            return nullptr;
        auto expr = parseExpression();
        if (!expr) return nullptr;
        if (!expect(TokenType::SEMICOLON, "Expected ';' after assignment"))
            return nullptr;
        return std::make_unique<AssignStmtAST>(varName, std::move(expr));
    } else if (isToken(TokenType::PRINT_KW)) {
        // print ( expr );
        getNextToken(); // consume 'print'
        if (!expect(TokenType::LPAREN, "Expected '(' after 'print'"))
            return nullptr;
        auto expr = parseExpression();
        if (!expr) return nullptr;
        if (!expect(TokenType::RPAREN, "Expected ')' after print expression"))
            return nullptr;
        if (!expect(TokenType::SEMICOLON, "Expected ';' after print()"))
            return nullptr;
        return std::make_unique<PrintStmtAST>(std::move(expr));
    } else {
        std::cerr << "Unknown start of statement: " << CurTok.Lexeme << "\n";
        return nullptr;
    }
}

// parseExpression: parse primary and right-hand-side of binary ops
std::unique_ptr<ExprAST> Parser::parseExpression() {
    auto lhs = parsePrimary();
    if (!lhs) return nullptr;
    return parseBinOpRHS(0, std::move(lhs));
}

// parsePrimary: IDENT (variable) or NUMBER or parenthesized expr
std::unique_ptr<ExprAST> Parser::parsePrimary() {
    if (isToken(TokenType::IDENT)) {
        auto node = std::make_unique<VariableExprAST>(CurTok.Lexeme);
        getNextToken();
        return node;
    } else if (isToken(TokenType::NUMBER)) {
        int val = std::stoi(CurTok.Lexeme);
        auto node = std::make_unique<IntegerExprAST>(val);
        getNextToken();
        return node;
    } else if (isToken(TokenType::LPAREN)) {
        getNextToken(); // consume '('
        auto expr = parseExpression();
        if (!expr) return nullptr;
        if (!expect(TokenType::RPAREN, "Expected ')' in expression")) return nullptr;
        return expr;
    } else {
        std::cerr << "Unknown token when parsing expression: " << CurTok.Lexeme << "\n";
        return nullptr;
    }
}

// parseBinOpRHS: handle binary ops by precedence climbing
std::unique_ptr<ExprAST> Parser::parseBinOpRHS(int exprPrec,
    std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int tokPrec = getTokPrecedence();
        if (tokPrec < exprPrec)
            return LHS;

        TokenType binop = CurTok.Type;
        char opChar = 0;
        switch (binop) {
        case TokenType::PLUS:  opChar = '+'; break;
        case TokenType::MINUS: opChar = '-'; break;
        case TokenType::MUL:   opChar = '*'; break;
        case TokenType::DIV:   opChar = '/'; break;
        default:
            opChar = '?'; 
            break;
        }

        getNextToken(); // consume operator
        auto RHS = parsePrimary();
        if (!RHS) return nullptr;

        int nextPrec = getTokPrecedence();
        if (tokPrec < nextPrec) {
            RHS = parseBinOpRHS(tokPrec + 1, std::move(RHS));
            if (!RHS) return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>(opChar, std::move(LHS), std::move(RHS));
    }
}
