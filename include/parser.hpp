/* Analisador Sintático */

#pragma once
#include <functional>
#include <ios>
#include <memory>
#include "tokenizer.hpp"

class Parser;

struct RuleElement {
    virtual ~RuleElement() {}
    virtual bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const = 0;

    virtual std::unique_ptr<RuleElement> clone() const = 0;
    virtual std::string stringify() const = 0;
};

struct RuleText : public RuleElement {
    std::string text;

    RuleText(const std::string& text);

    bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const override;
    std::unique_ptr<RuleElement> clone() const override { return std::make_unique<RuleText>(this->text); }
    std::string stringify() const override;
};

struct RuleToken : public RuleElement {
    std::string kind;

    RuleToken(const std::string& kind);

    bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const override;
    std::unique_ptr<RuleElement> clone() const override { return std::make_unique<RuleToken>(this->kind); }
    std::string stringify() const override;
};

struct RuleRef : public RuleElement {
    std::string name;

    RuleRef(const std::string& name);

    bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const override;
    std::unique_ptr<RuleElement> clone() const override { return std::make_unique<RuleRef>(this->name); }
    std::string stringify() const override;
};

struct RuleWrapper {
    enum class State { NONE=0, START, AND, OR };

    // Attributes
    State state = State::START;
    int min, max;
    std::unique_ptr<RuleElement> element;

    // Constructors
    RuleWrapper(): min{ 1 }, max{ 1 }, element{ nullptr } {}
    RuleWrapper(const RuleWrapper& rw): state{ rw.state }, min{ rw.min }, max{ rw.max }, element{ rw.element->clone() } {}
    RuleWrapper(RuleWrapper&& rw): state{ rw.state }, min{ rw.min }, max{ rw.max }, element{ rw.element.release() } {}

    RuleWrapper(State state, int min, int max, std::unique_ptr<RuleElement>& element): state{ state }, min{ min }, max{ max }, element{ element->clone() } {}
    RuleWrapper(RuleElement* element): min{ 1 }, max{ 1 }, element{ element } {}

    // Operators
    RuleWrapper& operator=(const RuleWrapper& rw) {
        if (this != &rw) {
            this->state = rw.state;
            this->min = rw.min; this->max = rw.max;
            this->element = rw.element->clone();
        }
        return *this;
    };
    RuleWrapper& operator=(RuleWrapper&& rw) {
        if (this != &rw) {
            this->state = rw.state;
            this->min = rw.min; this->max = rw.max;
            this->element.reset(rw.element.release());
        }
        return *this;
    }

    RuleWrapper operator()(int qty);
    RuleWrapper operator()(int min, int max);

    bool operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const;

    friend std::ostream& operator<<(std::ostream& os, const RuleWrapper& rw);
};

RuleWrapper rule_text(const std::string& text);
RuleWrapper rule_token(const std::string& kind);
RuleWrapper rule_ref(const std::string& name);


class Rule {
    // Attributes
    public:
        std::string _name;
        std::vector<RuleWrapper> elements;

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
        // Reference
        Rule& operator<<(RuleWrapper& rule);
        Rule& operator&(RuleWrapper& rule);
        Rule& operator|(RuleWrapper& rule);

        Rule& operator<<(std::string& text);
        Rule& operator&(std::string& text);
        Rule& operator|(std::string& text);

        // R-value
        Rule& operator<<(RuleWrapper&& rule) { return this->operator<<(rule); }
        Rule& operator&(RuleWrapper&& rule) { return this->operator&(rule); }
        Rule& operator|(RuleWrapper&& rule) { return this->operator|(rule); }

        Rule& operator<<(std::string&& text) { return this->operator<<(text); }
        Rule& operator&(std::string&& text) { return this->operator&(text); }
        Rule& operator|(std::string&& text) { return this->operator|(text); }

    // Functions
    public:
        friend std::ostream& operator<<(std::ostream& os, const Rule& r);
        friend std::istream& operator>>(std::istream& is, Rule& r);
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


// Functions
std::istream& operator>>(std::istream& is, Parser::Grammar& rules);