#include "tokenizer.hpp"

class PropositionalTokenizer : public Tokenizer {
    // Constructor
    public:
        PropositionalTokenizer(KeywordMap* keyword_map);

    // Methods
    public:
        Token get() override;
};