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