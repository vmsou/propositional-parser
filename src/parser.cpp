/* Analisador Sintático */

#include "parser.hpp"

// Constructor
Parser::Parser(Tokenizer* tokenizer): tokenizer{ tokenizer } {}

// Methods
bool Parser::is_terminal(std::deque<Token>& tokens, const std::string& kind, bool single) {
    if (tokens.empty()) return false;
    if (single && tokens.size() != 1) return false;

    if (tokens.front().kind == kind) { tokens.pop_front(); return true; }
    return false;
}