/* Analisador Sintático */

#include "tokenizer.hpp"

class Parser {
    // Attributes
    protected:
        Tokenizer* tokenizer;

    // Constructor
    public:
        Parser(Tokenizer* tokenizer);

    // Methods
    public:
        virtual bool valid(const std::string& expr) = 0;
        static bool is_terminal(std::list<Token>& tokens, const std::string& kind, bool single=true);

    protected:
        static void push_token(std::list<Token>& tokens, std::list<Token>& cache);
        static void revert_tokens(std::list<Token>& tokens, std::list<Token>& cache);
};