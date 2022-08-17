/* 
1. Formula=Constante|Proposicao|FormulaUnaria|FormulaBinaria.
2. Constante="T"|"F".
3. Proposicao=[a−z0−9]+
4. FormulaUnaria=AbreParen OperadorUnario Formula FechaParen
5. FormulaBinaria=AbreParen OperatorBinario Formula Formula FechaParen
6. AbreParen="("
7. FechaParen=")"
8. OperatorUnario="¬"
9. OperatorBinario="∨"|"∧"|"→"|"↔"

std::unordered_map<std::string, std::string> rules{
        {"Formula", "Constante|Proposicao|FormulaUnaria|FormulaBinaria"},
        {"Constante", "'T'|'F'"},
        {"Proposicao", "[a-z0-9]+"},
        {"FormulaUnaria", "AbreParen OperadorUnario Formula FechaParen"},
        {"FormulaBinaria", "AbreParen OperadorBinario Formula FechaParen"},
        {"AbreParen", "'('"},
        {"FechaParen", "')'"},
        {"OperadorUnario", R"(\neg)"},
        {"OperadorBinario", R"(\lor|\land|\implies|\iff)"},
};
*/

#include "main.hpp"

int main() {
    Tokenizer tokenizer{};

    std::deque<Token> tokens = tokenizer.tokenize("T \\lor F");

    while (!tokens.empty()) {
        std::cout << tokens.front() << '\n';
        tokens.pop_front();
    }
}