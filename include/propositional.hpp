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
        bool is_formula(std::deque<Token>& tokens);
        bool is_constant(std::deque<Token>& tokens);
        bool is_proposition(std::deque<Token>& tokens);
        bool is_unary_formula(std::deque<Token>& tokens);
        bool is_binary_formula(std::deque<Token>& tokens);
        bool is_open_parenthesis(std::deque<Token>& tokens);
        bool is_close_parenthesis(std::deque<Token>& tokens);
        bool is_unary_operator(std::deque<Token>& tokens);
        bool is_binary_operator(std::deque<Token>& tokens);
};
