/* Analisador Léxico */

#pragma once
#include <functional>
#include <list>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

using KeywordMap = std::unordered_map<std::string, std::vector<std::string>>;

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
        std::istringstream ss;
        KeywordMap* keyword_map;
        std::unordered_map<std::string, std::string> reverse_keyword;

    // Constructors
    public:
        Tokenizer(KeywordMap* keyword_map);

    // Methods
    public:
        virtual Token get() = 0;
        std::list<Token> tokenize(const std::string& expr);
        std::string match_kind(const std::string& text);
        bool empty() const { return this->ss.rdbuf()->in_avail() == 0; }
    private:
        void str(const std::string& text);
};

// Functions
std::ostream& operator<<(std::ostream& os, const Position& t);
std::ostream& operator<<(std::ostream& os, const Token& t);
std::ostream& operator<<(std::ostream& os, const std::list<Token>& d);
#endif
