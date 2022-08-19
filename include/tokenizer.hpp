/* Analisador Léxico */

#pragma once
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

using TokenMap = std::unordered_map<std::string, std::vector<std::string>>;

struct Position { std::size_t line = 0, column = 0; };

struct Token {
    // Attributes
    std::string kind;
    std::string text;
    Position pos;

    // Constructors
    Token(std::string kind="NONE", std::string text="", Position pos={0,0});

    // Operators
    operator bool() const { return this->kind != "NONE"; }
};

class Tokenizer {
    // Attributes
    protected:
        std::size_t line = 0, col = 0;
        std::stringstream ss;
        TokenMap* token_map;
        std::unordered_map<std::string, std::string> reverse_map;

    // Constructors
    public:
        Tokenizer(TokenMap* token_map);

    // Methods
    public:
        virtual Token get() = 0;
        std::deque<Token> tokenize(const std::string& text);
        std::string match_kind(const std::string& text);
        bool is_empty() const;
    private:
        void set_text(const std::string& text);
};

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t);
std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
