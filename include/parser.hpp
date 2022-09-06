/* Analisador Sintático */

#pragma once
#include <functional>
#include "tokenizer.hpp"

class Parser;

bool is_terminal(const std::string& kind, std::list<Token>& tokens, std::list<Token>& buffer);

struct RuleElement {
    enum class State { NONE=0, START, AND, OR };
    using callback = std::function<bool(Parser&, std::list<Token>&, std::list<Token>&)>;

    State state = State::START;
    std::string name;
    callback func;
    
    RuleElement(const std::string& name, callback func): name{ name }, func{ func } {}

    inline bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const { 
        return this->func(parser, tokens, buffer); 
    }

    friend std::ostream& operator<<(std::ostream& os, const RuleElement& re);
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
        bool valid(Parser& parser, std::list<Token>& tokens, std::list<Token>& shared_buffer);

    // Operators
    public:
        Rule& operator<<(RuleElement&& rule);
        Rule& operator&(RuleElement&& rule);
        Rule& operator|(RuleElement&& rule);

    // Functions
    public:
        friend std::ostream& operator<<(std::ostream& os, const Rule& r);
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
        static void push_token(std::list<Token>& tokens, std::list<Token>& buffer);
        static void revert_tokens(std::list<Token>& tokens, std::list<Token>& buffer);
};
