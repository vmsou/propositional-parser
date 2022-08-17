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
    std::string text;
    char c;
    while (true) {
        if (!this->ss.get(c)) return Token{};
        switch (c) {
            case '\n':
                this->col = 0;
                ++this->line;
                continue;;
            case ' ': case '\t': continue;
            case 'T': case 'F': 
                text += c;
                return Token{ "Constante", text, {this->line, this->col} };
            case '(':
                text += c;
                return Token{ "AbreParen", text, {this->line, this->col} };
            case ')':
                text += c;
                return Token{ "FechaParen", text, {this->line, this->col} };
            case 'a' ... 'z': case '\\':
                text += c;
                if (text == "\\neg") return Token{ "OperadorUnario", text, { this->line, this->col} };
                else if (text == "\\lor" || text == "\\and" || text == "\\implies" || text == "\\iff") return Token{ "OperadorBinario", text, {this->line, this->col} };
                break;
        }
        ++this->col;
    }
    return Token{};
}

bool Tokenizer::is_empty() const { return this->ss.rdbuf()->in_avail() == 0; }

// Functions
std::ostream& operator<<(std::ostream& os, const Token& t) {
    return os << "Token(" << "kind='" << t.kind << "', text='" << t.text << "')";
}
