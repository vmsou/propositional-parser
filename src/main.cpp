/* 
# Enunciado:
Para  obter  os  pontos  relativos  a  este  trabalho,  você  deverá  fazer  um  programa,  usando  a 
linguagem de programação que desejar, que seja capaz de validar expressões de lógica propisicional 
escritas em latex e definir se são expressões gramaticalmente corretas. Você validará apenas a forma 
da expressão (sintaxe).  
A entrada será fornecida por um arquivo de textos que será carregado em linha de comando, 
com a seguinte formatação:  
1. Na primeira linha deste arquivo existe um número inteiro que informa quantas expressões 
lógicas estão no arquivo.  
2. Cada uma das linhas seguintes contém uma expressão lógica que deve ser validada.  
A saída do seu programa será no terminal padrão do sistema e constituirá de uma linha de saída 
para cada expressão lógica de entrada contendo ou a palavra valida ou a palavra inválida e nada mais. 

## Gramática:  
1. Formula=Constante|Proposicao|FormulaUnaria|FormulaBinaria.
2. Constante="T"|"F".
3. Proposicao=[a−z0−9]+
4. FormulaUnaria=AbreParen OperadorUnario Formula FechaParen
5. FormulaBinaria=AbreParen OperatorBinario Formula Formula FechaParen
6. AbreParen="("
7. FechaParen=")"
8. OperadorUnario="¬"
9. OperadorBinario="∨"|"∧"|"→"|"↔"
 
Cada  expressão  lógica  avaliada  pode  ter  qualquer  combinação  das  operações  de  negação, 
conjunção, disjunção, implicação e bi-implicação sem limites na combiação de preposições e operações. 
Os valores lógicos True e False estão representados na gramática e, como tal, podem ser usados em 
qualquer expressão de entrada. 
Para  validar  seu  trabalho,  você  deve  incluir  no  repl.it,  no  mínimo  três  arquivos  contendo 
números  diferentes  de  expressões  proposicionais.  O  professor  irá  incluir  um  arquivo  de  testes  extra 
para validar seu trabalho. Para isso, caberá ao professor incluir o arquivo no seu repl.it e rodar o seu 
programa carregando o arquivo de testes. 
*/

#include "main.hpp"

Rule gen_constante() { return Rule("Constante") << rule_text("T") | rule_text("F"); }

int main() {
    std::cout << "> Inicio: Parser Logica Proposicional" << '\n';
    std::cout << "Arquivos: 'data/test.txt', 'data/valido.txt', 'data/invalido.txt', 'data/professor.txt'\n";
    std::cout << "Pressione ENTER (nome do arquivo vazio) para finalizar programa.\n\n";

    Tokenizer::KeywordMap keywords{
        {"Constante", {"T", "F"}},
        {"AbreParen", {"("}},
        {"FechaParen", {")"}},
        {"OperadorUnario", {"\\neg", "\\lnot", "¬"}},
        {"OperadorBinario", {
            "\\lor", "\\land", "\\implies", "\\iff",
            "\\rightarrow", "\\leftrightarrow"
            "∨", "∧", "→", "↔",
            "||", "&&",
            "\\vee", "\\wedge"
            }
        },
    };

    /* Parser::Grammar rules{
        Rule("Formula") << rule_ref("Constante") | rule_ref("Proposicao") | rule_ref("FormulaUnaria") | rule_ref("FormulaBinaria"),
        Rule("Constante") << "T" | "F",
        Rule("Proposicao") << rule_token("Proposicao"),
        Rule("FormulaUnaria") << rule_ref("AbreParen") & rule_ref("OperadorUnario") & rule_ref("Formula") & rule_ref("FechaParen"),
        Rule("FormulaBinaria") << rule_ref("AbreParen") & rule_ref("OperadorBinario") & rule_ref("Formula") & rule_ref("Formula") & rule_ref("FechaParen"),
        Rule("AbreParen") << "(",
        Rule("FechaParen") << ")",
        Rule("OperadorUnario") << rule_token("OperadorUnario"),
        Rule("OperadorBinario") << rule_token("OperadorBinario"),
    }; */

    std::string rules_text = R"!(
        Formula = Constante | Proposicao | FormulaUnaria | FormulaBinaria .
        Proposicao = :Proposicao: .
        Constante = "T" | "F" .
        FormulaUnaria = AbreParen OperadorUnario Formula FechaParen .
        FormulaBinaria = AbreParen OperadorBinario Formula Formula FechaParen .
        AbreParen = "(" .
        FechaParen = ")" .
        OperadorUnario = :OperadorUnario: .
        OperadorBinario = :OperadorBinario: .
    )!";
    
    std::istringstream iss{ rules_text };
    Parser::Grammar rules;
    iss >> rules;

    std::cout << "Regras:\n";
    for (const Rule& rule : rules) std::cout << rule << '\n';
    std::cout << '\n';

    PropositionalTokenizer tokenizer{ &keywords };
    Parser parser{ &tokenizer, &rules };
    TextData td{};

    bool is_running = true;
    while (is_running) {
        std::cout << ">> ";
        std::string command;
        std::getline(std::cin, command);
        
        if (command.empty()) { is_running = false; continue; }

        if (std::filesystem::exists(command)) {    // FILE
            try {
                td = TextData::load(command);
            } catch (const std::runtime_error& err) {
                std::cout << err.what() << "\n\n";
                continue;
            }
    
            for (const std::string& text : td.texts) {
                bool result = parser.valid(text, "Formula");
                std::cout << text << ": " << (result ? "valido" : "invalido") << '\n';
            }
        } else {                                    // EXPRESSION
            bool result = parser.valid(command, "Formula");
            std::cout << command << ": " << (result ? "valido" : "invalido") << '\n';
        }
        std::cout << '\n';
    }
    std::cout << "> Fim: Parser Logica Proposicional" << std::endl;
}