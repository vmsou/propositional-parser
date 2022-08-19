#include "propositional.hpp"

PropositionalTokenizer::PropositionalTokenizer(TokenMap* token_map): Tokenizer{ token_map } {}

Token PropositionalTokenizer::get() {
    Token t;
    std::string text;
    char c = '\0';
    bool match = false;
    bool start_proposition, end_proposition = false;
    bool is_proposition = true;
    while (!match) {
        if (!this->ss.get(c)) {
            if (start_proposition & is_proposition) return Token{ "Proposicao", text, {this->line, this->col - text.size() } };
            return Token{};
        };

        switch (c) {
            case '\n':
                this->col = 0;
                ++this->line;
                continue;
            case 'a' ... 'z': case '0' ... '9':
                start_proposition = true;
                text += c;
                break;
            case 'A' ... 'Z': case '\\':
                is_proposition = false;
                text += c;
                break;
            case ' ': case '\t': case '\r': case '\f': case '\v':
                if (start_proposition & is_proposition) {
                    start_proposition = false; 
                    end_proposition = true;
                }
                break;
            default:
                text += c;
                break;
        }

        std::string kind = this->match_kind(text);

        if (kind != "NONE") {
            match = true;
            t = Token{ kind, text, { this->line, (text.size() > 1) ? this->col - text.size() + 1 : this->col } };
        }
        else if (end_proposition & is_proposition) {
            match = true;
            start_proposition = false;
            end_proposition = false;
            t = Token{ "Proposicao", text, {this->line, this->col - text.size()} };
        }
        ++this->col;
    }
    return t;
}