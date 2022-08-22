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
};