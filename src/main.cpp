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
    bool is_running = true;
    std::string expr;
    PropositionalTokenizer tokenizer;

    while (is_running) {
        std::cout << "Expression (empty to exit): ";
        std::getline(std::cin, expr);

        if (expr.empty()) { is_running = false; continue; }

        std::deque<Token> tokens = tokenizer.tokenize(expr);
        while (!tokens.empty()) {
            std::cout << tokens.front() << '\n';
            tokens.pop_front();
        }
    }

    std::cout << "Successful exit." << std::endl;
}