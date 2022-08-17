/* Analisador Léxico */

#pragma once
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

using Rules_T = std::unordered_map<std::string, std::string>;

struct Position { int line, column; }

struct Token {
    // Attributes
    std::string kind;
    std::string text;
    Position pos;

    // Constructors
    Token(std::string kind="NONE", std::string text="", Position pos={0,0});
};

struct Tokenizer {
    // Attributes
    private:
        std::stringstream ss;
        Rules_T* rules;

    // Constructors
    public:
        Tokenizer(Rules_T* rules);

    // Methods
    public:
        std::deque<Token> tokenize(const std::string& text);
        Token get();
        bool is_empty() const;
};

// Functions
std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
