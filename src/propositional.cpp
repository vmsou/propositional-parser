#include "propositional.hpp"

Token PropositionalTokenizer::get() {
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
                continue;
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