/* Analisador Sintático */

#pragma once
#include <functional>
#include "tokenizer.hpp"

class Parser;

struct RuleElement {
    enum class State { NONE=0, START, AND, OR };
    using callback = std::function<bool(Parser&, std::list<Token>&, std::list<Token>&)>;

    State state = State::START;
    std::string name;
    callback func;
    int min=1, max=1;
    
    inline RuleElement(const std::string& name, callback func, int qty=1): name{ name }, func{ func }, min{ qty }, max{ qty } {}
    inline RuleElement(const std::string& name, callback func, int min, int max): name{ name }, func{ func }, min{ min }, max{ max } {}

    inline bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const { 
        int valid = 0;
        
        while (valid < this->max) {
            if (!this->func(parser, tokens, buffer)) { 
                if (valid >= this->min) break;
                return false; 
            }
            ++valid;
        }
        return valid >= this->min;
    }

    RuleElement operator()(int qty) { return RuleElement{ this->name, this->func, qty }; }
    RuleElement operator()(int min, int max) { return RuleElement{ this->name, this->func, min, max }; }

    friend std::ostream& operator<<(std::ostream& os, const RuleElement& re);
};

RuleElement RuleText(const std::string& name);
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

        Rule& operator<<(std::string&& text);
        Rule& operator&(std::string&& text);
        Rule& operator|(std::string&& text);

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
        bool valid(const std::string& expr, const std::string& rule);

        bool has_rule(const std::string& name) const;
        Rule& get_rule(const std::string& name);

    // Static Methods
    public:
        static void push_token(std::list<Token>& tokens, std::list<Token>& buffer);
        static void revert_tokens(std::list<Token>& tokens, std::list<Token>& buffer);
};
