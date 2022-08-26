#include "propositional.hpp"

/* PropositionalTokenizer:: */

// Constructors
PropositionalTokenizer::PropositionalTokenizer(KeywordMap* keyword_map): Tokenizer{ keyword_map } {}


// Methods
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
                if (start_proposition & is_proposition) {
                    start_proposition = false;
                    end_proposition = true;
                    this->ss.putback(c);
                    --this->col;
                } else {
                    text += c;
                }
                break;
        }

        std::string kind = this->match_kind(text);

        if (kind != "NONE") {
            match = true;
            size_t col = (text.size() > 1) ? this->col - text.size() + 1 : this->col;
            t = Token{ kind, text, { this->line, col} };
        }
        else if (end_proposition & is_proposition) {
            match = true;
            start_proposition = false;
            end_proposition = false;
            size_t col = (text.size() > 1) ? this->col - text.size() + 1 : this->col;
            t = Token{ "Proposicao", text, {this->line, col} };
        }
        ++this->col;
    }
    return t;
}

/* PropositionalParser:: */

// Constructors
PropositionalParser::PropositionalParser(Tokenizer* tokenizer): Parser{ tokenizer } {}

// Methods
bool PropositionalParser::valid(const std::string& expr) {
    std::list<Token> tokens = this->tokenizer->tokenize(expr);
    // std::cout << tokens << '\n';
    bool accepted = this->is_formula(tokens);
    return accepted;
}

// Grammar
bool PropositionalParser::is_constant(std::list<Token>& tokens, bool single) { 
    return Parser::is_terminal(tokens, "Constante", single); 
}

bool PropositionalParser::is_proposition(std::list<Token>& tokens, bool single) {
    return Parser::is_terminal(tokens, "Proposicao", single);
}

bool PropositionalParser::is_formula(std::list<Token>& tokens, bool single) {
    /* Formula = Constante|Proposicao|FormulaUnaria|FormulaBinaria */
    if (tokens.empty()) return false;
    return is_constant(tokens, single) || is_proposition(tokens, single) || is_unary_formula(tokens) || is_binary_formula(tokens);
}

bool PropositionalParser::is_unary_formula(std::list<Token>& tokens) {
    /* FormulaUnaria = AbreParen OperadorUnario Formula FechaParen  */
    if (tokens.empty() || tokens.front().kind != "AbreParen") return false;
    std::list<Token> cache;
    Parser::push_token(tokens, cache);

    if (tokens.empty() || tokens.front().kind != "OperadorUnario") { 
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    Parser::push_token(tokens, cache);
    
    if (!is_formula(tokens, false)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    if (tokens.empty() || tokens.front().kind != "FechaParen") {
        Parser::revert_tokens(tokens, cache);
        return false;
    };
    Parser::push_token(tokens, cache);
    return true;
}

bool PropositionalParser::is_binary_formula(std::list<Token>& tokens) {
    /* FormulaBinaria=AbreParen OperadorBinario Formula Formula FechaParen  */
    if (tokens.empty() || tokens.front().kind != "AbreParen") return false;
    std::list<Token> cache;
    Parser::push_token(tokens, cache);

    if (tokens.empty() || tokens.front().kind != "OperadorBinario") { 
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    Parser::push_token(tokens, cache);
    
    if (!is_formula(tokens, false)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    if (!is_formula(tokens, false)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    if (tokens.empty() || tokens.front().kind != "FechaParen") {
        Parser::revert_tokens(tokens, cache);
        return false;
    };
    Parser::push_token(tokens, cache);
    cache.clear();
    return true;
}
