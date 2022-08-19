#include <functional>
#include <iostream>
#include <unordered_map>

#include "propositional.hpp"


bool is_formula(const std::string& text);
bool is_constant(const std::string& text);
bool is_proposition(const std::string& text);
bool is_binary_formula(const std::string& text);
bool is_unary_formula(const std::string& text);
bool is_open_parenthesis(const std::string& text);
bool is_close_parenthesis(const std::string& text);
bool is_binary_operator(const std::string& text);
bool is_unary_operator(const std::string& text);