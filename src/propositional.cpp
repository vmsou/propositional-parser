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

// Methods
bool PropositionalParser::valid(const std::string& expr, const std::string& rule) {
    std::list<Token> tokens = this->tokenizer->tokenize(expr);
    // std::cout << tokens << '\n';
    bool accepted = this->get_rule(rule).valid(*this, tokens) && tokens.empty();
    return accepted;
}

// Grammar
bool PropositionalParser::is_constant(std::list<Token>& tokens) { 
    std::list<Token> cache;
    return is_terminal("Constante", tokens, cache); 
}

bool PropositionalParser::is_proposition(std::list<Token>& tokens) {
    std::list<Token> cache;
    return is_terminal("Proposicao", tokens, cache);
}

bool PropositionalParser::is_formula(std::list<Token>& tokens) {
    /* Formula = Constante|Proposicao|FormulaUnaria|FormulaBinaria */
    if (tokens.empty()) return false;
    return is_constant(tokens) || is_proposition(tokens) || is_unary_formula(tokens) || is_binary_formula(tokens);
}

bool PropositionalParser::is_unary_formula(std::list<Token>& tokens) {
    /* FormulaUnaria = AbreParen OperadorUnario Formula FechaParen  */
    std::list<Token> cache;
    if (!is_terminal("AbreParen", tokens, cache)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }

    if (!is_terminal("OperadorUnario", tokens, cache)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    
    if (!is_formula(tokens)) {
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
    std::list<Token> cache;
    if (is_terminal("AbreParen", tokens, cache)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }

    if (is_terminal("OperadorBinario", tokens, cache)) { 
        Parser::revert_tokens(tokens, cache);
        return false;
    }

    if (!is_formula(tokens)) {
        Parser::revert_tokens(tokens, cache);
        return false;
    }
    if (!is_formula(tokens)) {
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
