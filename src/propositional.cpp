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
            case 'a' ... 'z': case 'A' ... 'Z': case '\\':
                text += c;
                break;
            case ' ': case '\t': break;
        }
        
        if (text == "T" || text == "F")  {
            match = true;
            t = Token{ "Constante", text, {this->line, this->col} };
        }
        else if (text == "(") {
            match = true;
            t = Token{ "AbreParen", text, {this->line, this->col} };
        }
        else if (text == ")") {
            match = true;
            t = Token{ "FechaParen", text, {this->line, this->col} };
        }
        else if (text == "\\neg") {
            match = true;
            t = Token{ "OperadorUnario", text, { this->line, this->col - text.size() + 1} };
        }
        else if (text == "\\lor" || text == "\\land" || text == "\\implies" || text == "\\iff") {
            match = true;
            t = Token{ "OperadorBinario", text, {this->line, this->col - text.size() + 1} };
        }
        ++this->col;
    }
    return t;
}