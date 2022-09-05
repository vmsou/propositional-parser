/* Analisador Sintático */

#pragma once
#include <functional>
#include "tokenizer.hpp"

class Parser;

bool is_terminal(const std::string& kind, std::list<Token>& tokens, std::list<Token>& cache);

struct RuleElement {
    enum class State { NONE=0, START, AND, OR };
    using callback = std::function<bool(Parser&, std::list<Token>&, std::list<Token>&)>;

    State state = State::START;
    callback func;
    
    RuleElement(callback func): func{ func } {}

    inline bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& cache) const { 
        return this->func(parser, tokens, cache); 
    }
};

RuleElement RuleToken(const std::string& kind);
RuleElement RuleRef(const std::string& name);

class Rule {
    // Attributes
    private:
        std::string _name;
        std::vector<RuleElement> elements;

    // Constructors
    public:
        Rule() = default;
        Rule(const std::string& name);

    // Methods
    public:
        inline std::string name() const { return this->_name; }
        bool valid(Parser& parser, std::list<Token>& tokens);

    // Operators
    public:
        Rule& operator<<(RuleElement&& rule);
        Rule& operator&(RuleElement&& rule);
        Rule& operator|(RuleElement&& rule);
};

class Parser {
    // Alias
    public:
        using Grammar = std::vector<Rule>;
        using RulesMap = std::unordered_map<std::string, Rule>;

    // Attributes
    protected:
        Tokenizer* tokenizer;
        Grammar* grammar;
        RulesMap rules;

    // Constructor
    public:
        Parser(Tokenizer* tokenizer, Grammar* grammar);

    // Methods
    public:
        virtual bool valid(const std::string& expr, const std::string& rule) = 0;

        bool has_rule(const std::string& name) const;
        Rule& get_rule(const std::string& name);

    // Static Methods
    public:
        static void push_token(std::list<Token>& tokens, std::list<Token>& cache);
        static void revert_tokens(std::list<Token>& tokens, std::list<Token>& cache);
};
