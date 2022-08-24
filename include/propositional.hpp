#include "parser.hpp"
#include "tokenizer.hpp"


class PropositionalTokenizer : public Tokenizer {
    // Constructors
    public:
        PropositionalTokenizer(KeywordMap* keyword_map);

    // Methods
    public:
        Token get() override;
};


class PropositionalParser : public Parser {
    // Constructors
    public:
        PropositionalParser(Tokenizer* tokenizer);

    // Methods
    public:
        bool valid(const std::string& expr) override;

    // Grammar
    public:
        bool is_constant(std::list<Token>& tokens, bool single=true);
        bool is_proposition(std::list<Token>& tokens, bool single=true);
        bool is_open_parenthesis(std::list<Token>& tokens, bool single=true);
        bool is_close_parenthesis(std::list<Token>& tokens, bool single=true);
        bool is_unary_operator(std::list<Token>& tokens, bool single=true);
        bool is_binary_operator(std::list<Token>& tokens, bool single=true);

        bool is_formula(std::list<Token>& tokens, bool single=true);
        bool is_unary_formula(std::list<Token>& tokens);
        bool is_binary_formula(std::list<Token>& tokens);
};
