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
    std::deque<Token> tokens = this->tokenizer->tokenize(expr);
    bool accepted = this->is_formula(tokens);
    return accepted;
}

// Grammar
bool PropositionalParser::is_constant(std::deque<Token>& tokens, bool single) { 
    return Parser::is_terminal(tokens, "Constante", single); 
}

bool PropositionalParser::is_proposition(std::deque<Token>& tokens, bool single) {
    return Parser::is_terminal(tokens, "Proposicao", single);
}

bool PropositionalParser::is_open_parenthesis(std::deque<Token>& tokens, bool single) {
    return Parser::is_terminal(tokens, "AbreParen", single);
}

bool PropositionalParser::is_close_parenthesis(std::deque<Token>& tokens, bool single) {
    return Parser::is_terminal(tokens, "FechaParen", single);
}

bool PropositionalParser::is_unary_operator(std::deque<Token>& tokens, bool single) {
    return Parser::is_terminal(tokens, "OperadorUnario", single);
}

bool PropositionalParser::is_binary_operator(std::deque<Token>& tokens, bool single) {
    return Parser::is_terminal(tokens, "OperadorBinario", single);
}

bool PropositionalParser::is_formula(std::deque<Token>& tokens, bool single) {
    /* Formula = Constante|Proposicao|FormulaUnaria|FormulaBinaria */
    return is_constant(tokens, single) || is_proposition(tokens, single) || is_binary_formula(tokens) || is_unary_formula(tokens);
}

bool PropositionalParser::is_unary_formula(std::deque<Token>& tokens) {
    /* FormulaUnaria = AbreParen OperadorUnario Formula FechaParen  */
    if (tokens.empty() || tokens.front().kind != "AbreParen") return false;
    const Token& abre_paren = tokens.front(); tokens.pop_front();

    if (tokens.empty() || tokens.front().kind != "OperadorUnario") { 
        tokens.push_front(abre_paren);
        return false;
    }
    const Token& op_unario = tokens.front(); tokens.pop_front();
    
    if (!is_formula(tokens, false)) {
        tokens.push_front(op_unario);
        tokens.push_front(abre_paren);
        return false;
    }
    if (tokens.empty() || tokens.front().kind != "FechaParen") {
        tokens.push_front(op_unario);
        tokens.push_front(abre_paren);
        return false;
    };
    tokens.pop_front();  // FechaParen
    return true;
}

bool PropositionalParser::is_binary_formula(std::deque<Token>& tokens) {
    /* FormulaBinaria=AbreParen OperadorBinario Formula Formula FechaParen  */
    if (tokens.empty() || tokens.front().kind != "AbreParen") return false;
    const Token& abre_paren = tokens.front(); tokens.pop_front();

    if (tokens.empty() || tokens.front().kind != "OperadorBinario") { 
        tokens.push_front(abre_paren);
        return false;
    }
    const Token& op_binario = tokens.front(); tokens.pop_front();
    
    if (!is_formula(tokens, false)) {
        tokens.push_front(op_binario);
        tokens.push_front(abre_paren);
        return false;
    }
    if (!is_formula(tokens, false)) {
        tokens.push_front(op_binario);
        tokens.push_front(abre_paren);
        return false;
    }
    if (tokens.empty() || tokens.front().kind != "FechaParen") {
        tokens.push_front(op_binario);
        tokens.push_front(abre_paren);
        return false;
    };
    tokens.pop_front();  // FechaParen
    return true;
}
