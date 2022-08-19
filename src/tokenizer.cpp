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
std::deque<Token> Tokenizer::tokenize(const std::string& text) {
    this->set_text(text);
    std::deque<Token> tokens;

    while (!this->is_empty()) {
        Token t = this->get();;
        if (t) tokens.push_back(t);
    }
    return tokens;
}

bool Tokenizer::is_empty() const { return this->ss.rdbuf()->in_avail() == 0; }

std::string Tokenizer::match_kind(const std::string& text) {
    for (const auto& [token_text, kind] : this->reverse_keyword) {
        if (text == token_text) return kind;
    }
    return "NONE";
}

void Tokenizer::set_text(const std::string& text) {
    this->line = 0;
    this->col = 0;
    this->ss = std::stringstream{ text };
}

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t) {
    return os << '{' << t.line << ", " << t.column << "}";
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    return os << "Token(" << t.kind << ", '" << t.text << "', " << t.pos << ")";
}

std::ostream& operator<<(std::ostream& os, const std::deque<Token>& d) {
    const size_t size = d.size();
    size_t i{};
    os << '{';
    for (const Token& t : d) {
        os << t;
        if (i < (size - 1)) os << ", ";
    }
    return os << '}';
}
