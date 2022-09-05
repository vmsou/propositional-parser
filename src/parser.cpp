/* Analisador Sintático */

#include "parser.hpp"

// Functions
bool is_terminal(const std::string& kind, std::list<Token>& tokens, std::list<Token>& cache) {
    if (tokens.empty() || tokens.front().kind != kind) return false;
    Parser::push_token(tokens, cache);
    return true;
}

RuleElement RuleToken(const std::string& kind) {
    return RuleElement{
        [&](Parser& parser, std::list<Token>& tokens, std::list<Token>& cache) {
            return is_terminal(kind, tokens, cache);
        }
    };
}

RuleElement RuleRef(const std::string& name) {
    return RuleElement{
        [&](Parser& parser, std::list<Token>& tokens, std::list<Token>& cache) {
            return parser.get_rule(name).valid(parser, tokens);
        }
    };
}

/* Rule */
// Constructors
Rule::Rule(const std::string& name): _name{ name } {}

// Methods
bool Rule::valid(Parser& parser, std::list<Token>& tokens) { 
    using State = RuleElement::State;

    std::list<Token> shared_cache;
    bool valid = false;
    for (const RuleElement& r : this->elements) {
        if (r.state == RuleElement::State::START) {
            valid = r(parser, tokens, shared_cache);
        }    
        else if (r.state == State::OR) {
            std::list<Token> temp_cache;
            if (valid || r(parser, tokens, temp_cache)) return true;
            else {
                valid = false;
                Parser::revert_tokens(tokens, temp_cache);
            }
        } 
        else if (r.state == State::AND) {
            if (!r(parser, tokens, shared_cache)) { 
                Parser::revert_tokens(tokens, shared_cache);
                return false;
            }
        }
    }
    return valid;
}

// Operators
Rule& Rule::operator<<(RuleElement&& rule) {
    rule.state = RuleElement::State::START;
    this->elements.push_back(rule);
    return *this;
}

Rule& Rule::operator&(RuleElement&& rule) {
    rule.state = RuleElement::State::AND;
    this->elements.push_back(rule);
    return *this;
}

Rule& Rule::operator|(RuleElement&& rule) {
    rule.state = RuleElement::State::OR;
    this->elements.push_back(rule);
    return *this;
}


/* Parser */
// Constructor
Parser::Parser(Tokenizer* tokenizer, Grammar* grammar): tokenizer{ tokenizer }, grammar{ grammar } {
    for (const Rule& rule : *grammar) { this->rules[rule.name()] = rule; }
}

// Methods
bool Parser::has_rule(const std::string& name) const {
    if (this->rules.find(name) != this->rules.end()) return true;
    return false;
}

Rule& Parser::get_rule(const std::string& name) { return this->rules.at(name); }

void Parser::push_token(std::list<Token>& tokens, std::list<Token>& cache) {
    cache.push_front(tokens.front()); tokens.pop_front();
}

void Parser::revert_tokens(std::list<Token>& tokens, std::list<Token>& cache) {
    for (const Token& t : cache) tokens.push_front(t);
    cache.clear();
}
