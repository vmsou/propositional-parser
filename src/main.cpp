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
*/

#include "main.hpp"

int main() {
    KeywordMap keywords{
        {"Constante", {"T", "F"}},
        {"AbreParen", {"("}},
        {"FechaParen", {")"}},
        {"OperadorUnario", {"\\neg", "¬"}},
        {"OperadorBinario", {
            "\\lor", "\\land", "\\implies", "\\iff",
            "∨", "∧", "→", "↔",
            "||", "&&"
            }
        },
    };
    PropositionalTokenizer tokenizer{ &keywords };
    PropositionalParser parser{ & tokenizer };

    bool is_running = true;
    std::string expr;
    while (is_running) {
        std::cout << "Expression (empty to exit): ";
        std::getline(std::cin, expr);

        if (expr.empty()) { is_running = false; continue; }

        std::deque<Token> tokens = tokenizer.tokenize(expr);
        std::cout << tokens << '\n';

        std::cout << "Valid: " << std::boolalpha << parser.valid(expr) << '\n';
    }

    std::cout << "Successful exit." << std::endl;
}