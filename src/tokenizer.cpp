/* Analisador Léxico */

#include "tokenizer.hpp"

/* Token */
// Constructors
Token::Token(std::string kind, std::string text, Position pos): kind{ kind }, text{ text }, pos{ pos } {}

/* Tokenizer */
// Constructors
Tokenizer::Tokenizer(KeywordMap* keyword_map): keyword_map{ keyword_map } {
    for (const auto& [kind, tokens] : *keyword_map) {
        for (const std::string& token_text : tokens) {
            this->reverse_keyword[token_text] = kind;
        }
    }
}

// Methods
std::list<Token> Tokenizer::tokenize(const std::string& expr) {
    this->str(expr);
    std::list<Token> tokens;

    while (!this->empty()) {
        Token t = this->get();;
        if (t) tokens.push_back(t);
    }
    return tokens;
}

std::string Tokenizer::match_kind(const std::string& text) {
    if (this->reverse_keyword.find(text) == this->reverse_keyword.end()) return "NONE";
    return this->reverse_keyword.at(text);
}

void Tokenizer::str(const std::string& text) {
    this->line = 0;
    this->col = 0;
    this->ss = std::istringstream{ text };
}

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t) {
    return os << '(' << t.line << ", " << t.column << ")";
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    return os << "Token(" << t.kind << ", '" << t.text << "', " << t.pos << ")";
}

std::ostream& operator<<(std::ostream& os, const std::list<Token>& d) {
    const size_t size = d.size();
    size_t i{};
    os << '[';
    for (const Token& t : d) {
        os << t;
        if (i < (size - 1)) os << ", ";
        ++i;
    }
    return os << ']';
}
