/* Analisador Sintático */

#include "tokenizer.hpp"

class Parser {
    // Attributes
    protected:
        Tokenizer* tokenizer;
        std::deque<Token> cache;

    // Constructor
    public:
        Parser(Tokenizer* tokenizer);

    // Methods
    public:
        virtual bool valid(const std::string& expr) = 0;

        static bool is_terminal(std::deque<Token>& tokens, const std::string& kind, bool single=true);
};