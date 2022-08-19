#include "tokenizer.hpp"

class PropositionalTokenizer : public Tokenizer {
    // Constructor
    public:
        PropositionalTokenizer(TokenMap* token_map);

    // Methods
    public:
        Token get() override;
};