#ifndef MYLANG_LEXER_H
#define MYLANG_LEXER_H

#include <string>

namespace MyLang {

enum class TokenType {
    EOF_TOK,       // End of file
    IDENT,         // Identifier: [a-zA-Z][a-zA-Z0-9_]*
    NUMBER,        // Integer literal
    PLUS,          // '+'
    MINUS,         // '-'
    MUL,           // '*'
    DIV,           // '/'
    ASSIGN,        // '='
    LPAREN,        // '('
    RPAREN,        // ')'
    SEMICOLON,     // ';'
    PRINT_KW,      // 'print' keyword
    UNKNOWN
};

struct Token {
    TokenType Type;
    std::string Lexeme; // Raw text for IDENT or NUMBER
};

class Lexer {
    const std::string& Source;
    size_t Index = 0;
    size_t Length;

public:
    Lexer(const std::string& src) : Source(src), Length(src.size()) {}
    Token getNextToken();

private:
    char peek() const;
    char get();
    void skipWhitespace();
    Token lexIdentifierOrKeyword();
    Token lexNumber();
};

} // namespace MyLang

#endif // MYLANG_LEXER_H
