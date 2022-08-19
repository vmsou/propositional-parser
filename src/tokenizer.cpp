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

Token Tokenizer::get() {
    Token t;
    std::string text;
    char c = '\0';
    bool match = false;
    while (!match) {
        if (!this->ss.get(c)) return Token{};
        switch (c) {
            case '\n':
                this->col = 0;
                ++this->line;
                continue;;
            case ' ': case '\t': break;
            case 'T': case 'F': 
                match = true;
                text += c;
                t = Token{ "Constante", text, {this->line, this->col} };
                break;
            case '(':
                match = true;
                text += c;
                t = Token{ "AbreParen", text, {this->line, this->col} };
                break;
            case ')':
                match = true;
                text += c;
                t = Token{ "FechaParen", text, {this->line, this->col} };
                break;
            case 'a' ... 'z': case '\\':
                text += c;
                if (text == "\\neg") {
                    match = true;
                    t = Token{ "OperadorUnario", text, { this->line, this->col - text.size() + 1} };
                    break;
                }
                else if (text == "\\lor" || text == "\\land" || text == "\\implies" || text == "\\iff") { 
                    match = true;
                    t = Token{ "OperadorBinario", text, {this->line, this->col - text.size() + 1} };
                    break;
                }
                break;
        }
        ++this->col;
    }
    return t;
}

bool Tokenizer::is_empty() const { return this->ss.rdbuf()->in_avail() == 0; }

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t) {
    return os << '{' << t.line << ", " << t.column << "}";
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    return os << "Token(" << t.kind << ", '" << t.text << "', " << t.pos << ")";
}
