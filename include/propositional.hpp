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
        using Parser::Parser;

    // Methods
    public:
        bool valid(const std::string& expr, const std::string& rule) override;

    // Grammar
    public:
        static bool is_constant(std::list<Token>& tokens);
        static bool is_proposition(std::list<Token>& tokens);
       
        static bool is_formula(std::list<Token>& tokens);
        static bool is_unary_formula(std::list<Token>& tokens);
        static bool is_binary_formula(std::list<Token>& tokens);
};
