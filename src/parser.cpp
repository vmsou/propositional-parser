/* Analisador Sintático */

#include "parser.hpp"

// Functions
bool is_terminal(const std::string& kind, std::list<Token>& tokens, std::list<Token>& buffer) {
    if (tokens.empty() || tokens.front().kind != kind) return false;
    Parser::push_token(tokens, buffer);
    return true;
}

std::ostream& operator<<(std::ostream& os, const RuleElement& re) {
    using State = RuleElement::State;
    std::string op = "";
    if (re.state == State::AND) op = " ";
    else if (re.state == State::OR) op = "|";
    return os << op << re.name;
}

RuleElement RuleToken(const std::string& kind) {
    return RuleElement{ "Token(" + kind + ")",
        [&](Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) {
            return is_terminal(kind, tokens, buffer);
        }
    };
}

RuleElement RuleRef(const std::string& name) {
    return RuleElement{ "<" + name + ">",
        [&](Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) {
            return parser.get_rule(name).valid(parser, tokens, buffer);
        }
    };
}

/* Rule */
// Constructors
Rule::Rule(const std::string& name): _name{ name } {}

// Methods
bool Rule::valid(Parser& parser, std::list<Token>& tokens, std::list<Token>& shared_buffer) { 
    using State = RuleElement::State;

    bool valid = false;
    for (const RuleElement& r : this->elements) {
        if (r.state == RuleElement::State::START) {
            valid = r(parser, tokens, shared_buffer);
        }    
        else if (r.state == State::OR) {
            std::list<Token> temp_buffer;
            if (valid || r(parser, tokens, temp_buffer)) return true;
            else {
                valid = false;
                Parser::revert_tokens(tokens, temp_buffer);
            }
        } 
        else if (r.state == State::AND) {
            if (!r(parser, tokens, shared_buffer)) { 
                Parser::revert_tokens(tokens, shared_buffer);
                return false;
            }
        }
    }
    return valid;
}

bool Rule::valid(Parser& parser, std::list<Token>& tokens) { 
    using State = RuleElement::State;

    std::list<Token> shared_buffer;
    bool valid = false;
    for (const RuleElement& r : this->elements) {
        if (r.state == RuleElement::State::START) {
            valid = r(parser, tokens, shared_buffer);
        }    
        else if (r.state == State::OR) {
            std::list<Token> temp_buffer;
            if (valid || r(parser, tokens, temp_buffer)) return true;
            else {
                valid = false;
                Parser::revert_tokens(tokens, temp_buffer);
            }
        } 
        else if (r.state == State::AND) {
            if (!r(parser, tokens, shared_buffer)) { 
                Parser::revert_tokens(tokens, shared_buffer);
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

// Functions
std::ostream& operator<<(std::ostream& os, const Rule& r) {
    os << '<' << r.name() << '>' << "::=";
    for (const RuleElement& re : r.elements) os << re; 
    return os;
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

void Parser::push_token(std::list<Token>& tokens, std::list<Token>& buffer) {
    buffer.push_front(tokens.front()); tokens.pop_front();
}

void Parser::revert_tokens(std::list<Token>& tokens, std::list<Token>& buffer) {
    for (const Token& t : buffer) tokens.push_front(t);
    buffer.clear();
}
