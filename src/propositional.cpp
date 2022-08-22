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
bool PropositionalParser::is_formula(std::deque<Token>& tokens) {
    /* Formula = Constante|Proposicao|FormulaUnaria|FormulaBinaria */
    return is_constant(tokens) || is_proposition(tokens) || is_unary_formula(tokens) || is_binary_formula(tokens);
}

bool PropositionalParser::is_constant(std::deque<Token>& tokens) {
    /* Constante="T"|"F" */
    if (tokens.empty()) return false;

    const Token& token = tokens.front(); tokens.pop_front();
    if (token.kind == "Constante") return true;
    tokens.push_front(token);
    return false;
}

bool PropositionalParser::is_proposition(std::deque<Token>& tokens) {
    /* Proposicao=[a−z0−9]+ */
    if (tokens.empty()) return false;

    const Token& token = tokens.front(); tokens.pop_front();
    if (token.kind == "Proposicao") return true;
    tokens.push_front(token);
    return false;
}

bool PropositionalParser::is_unary_formula(std::deque<Token>& tokens) {
    /* FormulaUnaria = AbreParen OperadorUnario Formula FechaParen  */
    return is_open_parenthesis(tokens) && is_unary_operator(tokens) && is_formula(tokens) && is_close_parenthesis(tokens);
}

bool PropositionalParser::is_binary_formula(std::deque<Token>& tokens) {
    /* FormulaBinaria=AbreParen OperatorBinario Formula Formula FechaParen  */
    return is_open_parenthesis(tokens) && is_binary_operator(tokens) && is_formula(tokens) && is_formula(tokens) && is_close_parenthesis(tokens);
}

bool PropositionalParser::is_open_parenthesis(std::deque<Token>& tokens) {
    /* AbreParen="(" */
    if (tokens.empty()) return false;

    const Token& token = tokens.front(); tokens.pop_front();
    if (token.kind == "AbreParen") return true;
    tokens.push_front(token);
    return false;
}

bool PropositionalParser::is_close_parenthesis(std::deque<Token>& tokens) {
    /* FechaParen=")" */
    if (tokens.empty()) return false;

    const Token& token = tokens.front(); tokens.pop_front();
    if (token.kind == "FechaParen") return true;
    tokens.push_front(token);
    return false;
}

bool PropositionalParser::is_unary_operator(std::deque<Token>& tokens) {
    /* OperatorUnario="¬" */
    if (tokens.empty()) return false;

    const Token& token = tokens.front(); tokens.pop_front();
    if (token.kind == "OperadorUnario") return true;
    tokens.push_front(token);
    return false;
}

bool PropositionalParser::is_binary_operator(std::deque<Token>& tokens) {
    /* OperatorBinario= "∨"|"∧"|"→"|"↔" */
    if (tokens.empty()) return false;

    const Token& token = tokens.front(); tokens.pop_front();
    if (token.kind == "OperadorBinario") return true;
    tokens.push_front(token);
    return false;
}
