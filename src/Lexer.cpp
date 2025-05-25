#include "Lexer.h"
#include <cctype>

using namespace MyLang;

static bool isIdentStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c));
}
static bool isIdentChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

char Lexer::peek() const {
    if (Index >= Length) return '\0';
    return Source[Index];
}

char Lexer::get() {
    if (Index >= Length) return '\0';
    return Source[Index++];
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek())))
        get();
}

Token Lexer::lexIdentifierOrKeyword() {
    size_t start = Index;
    while (isIdentChar(peek()))
        get();
    std::string lex = Source.substr(start, Index - start);
    if (lex == "print")
        return Token{TokenType::PRINT_KW, lex};
    else
        return Token{TokenType::IDENT, lex};
}

Token Lexer::lexNumber() {
    size_t start = Index;
    while (std::isdigit(static_cast<unsigned char>(peek())))
        get();
    std::string lex = Source.substr(start, Index - start);
    return Token{TokenType::NUMBER, lex};
}

Token Lexer::getNextToken() {
    skipWhitespace();
    char c = peek();
    if (c == '\0')
        return Token{TokenType::EOF_TOK, ""};

    // Single‐character tokens
    switch (c) {
    case '+':
        get();
        return Token{TokenType::PLUS, "+"};
    case '-':
        get();
        return Token{TokenType::MINUS, "-"};
    case '*':
        get();
        return Token{TokenType::MUL, "*"};
    case '/':
        get();
        return Token{TokenType::DIV, "/"};
    case '=':
        get();
        return Token{TokenType::ASSIGN, "="};
    case '(':
        get();
        return Token{TokenType::LPAREN, "("};
    case ')':
        get();
        return Token{TokenType::RPAREN, ")"};
    case ';':
        get();
        return Token{TokenType::SEMICOLON, ";"};
    default:
        break;
    }

    if (isIdentStart(c))
        return lexIdentifierOrKeyword();

    if (std::isdigit(static_cast<unsigned char>(c)))
        return lexNumber();

    // Unknown character
    get();
    return Token{TokenType::UNKNOWN, std::string(1, c)};
}
