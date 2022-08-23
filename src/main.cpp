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
    std::cout << "> Inicio: Parser Logica Proposicional" << '\n';
    std::cout << "Arquivos: 'data/test.txt', 'data/valido.txt', 'data/invalido.txt'\n";
    std::cout << "Pressione ENTER (nome do arquivo vazio) para finalizar programa.\n\n";

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
    PropositionalParser parser{ &tokenizer };
    TextData td{};
    
    bool is_running = true;
    while (is_running) {
        std::cout << "Arquivo: ";
        std::string filename;
        std::getline(std::cin, filename);
        
        if (filename.empty()) { is_running = false; continue; }
        
        try {
            td = TextData::load(filename);
        } catch (const std::runtime_error &err) {
            std::cout << err.what() << "\n\n";
            continue;
        }
    
        for (const std::string& text : td.texts) {
            bool result = parser.valid(text);
            std::string msg = result ? "pertence" : "nao pertence";
            std::cout << text << ": " << msg << '\n';
        }
        std::cout << '\n';
    }
    std::cout << "> Fim: Parser Logica Proposicional" << '\n';
}