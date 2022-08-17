/* Analisador Léxico */

#include "tokenizer.hpp"

/* Token */
// Constructors
Token::Token(std::string kind, std::string text): kind{ kind }, text{ text } {}

/* Tokenizer */
// Constructors
Tokenizer::Tokenizer(Rules_T* rules): rules{ rules } {}

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

Token Tokenizer::get() {
    return Token{};
}

bool Tokenizer::is_empty() const { return this->ss.rdbuf()->in_avail() == 0; }

// Functions
std::ostream& operator<<(std::ostream& os, const Token& t) {
    return os << "Token(" << "kind='" << t.kind << "', text='" << t.text << "')";
}
