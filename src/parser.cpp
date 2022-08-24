/* Analisador Sintático */

#include "parser.hpp"

// Constructor
Parser::Parser(Tokenizer* tokenizer): tokenizer{ tokenizer } {}

// Methods
bool Parser::is_terminal(std::list<Token>& tokens, const std::string& kind, bool single) {
    if (tokens.empty()) return false;
    if (single && tokens.size() != 1) return false;

    if (tokens.front().kind != kind) return false;
    tokens.pop_front();
    return true;
}

void Parser::push_token(std::list<Token>& tokens, std::list<Token>& cache) {
    cache.push_front(tokens.front()); tokens.pop_front();
}

void Parser::revert_tokens(std::list<Token>& tokens, std::list<Token>& cache) {
    for (const Token& t : cache) tokens.push_front(t);
    cache.clear();
}