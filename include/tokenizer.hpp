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

    // Constructors
    public:
        Tokenizer() = default;

    // Methods
    public:
        virtual Token get() = 0;
        std::deque<Token> tokenize(const std::string& text);
        bool is_empty() const;
};

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t);
std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
