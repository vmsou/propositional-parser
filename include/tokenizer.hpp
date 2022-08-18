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

struct Position { std::size_t line, column; };

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

struct Tokenizer {
    // Attributes
    private:
        std::size_t line, col = 0;
        std::stringstream ss;

    // Constructors
    public:
        Tokenizer() = default;

    // Methods
    public:
        std::deque<Token> tokenize(const std::string& text);
        Token get();
        bool is_empty() const;
};

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t);
std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
