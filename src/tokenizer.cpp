/* Analisador Léxico */

#include "tokenizer.hpp"

/* Token */
// Constructors
Token::Token(std::string kind, std::string text, Position pos): kind{ kind }, text{ text }, pos{ pos } {}

/* Tokenizer */
// Constructors

// Methods
std::deque<Token> Tokenizer::tokenize(const std::string& text) {
    this->ss = std::stringstream(text);
    std::deque<Token> tokens;

    while (!this->is_empty()) {
        Token t = this->get();;
        tokens.push_back(t);
    }
    return tokens;
}

bool Tokenizer::is_empty() const { return this->ss.rdbuf()->in_avail() == 0; }

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t) {
    return os << '{' << t.line << ", " << t.column << "}";
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    return os << "Token(" << t.kind << ", '" << t.text << "', " << t.pos << ")";
}
