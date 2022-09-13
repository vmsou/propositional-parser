/* Analisador Sintático */

#include "parser.hpp"

// Functions
RuleWrapper rule_text(const std::string& text) { return RuleWrapper(new RuleText(text)); }
RuleWrapper rule_token(const std::string& kind) { return RuleWrapper(new RuleToken(kind)); }
RuleWrapper rule_ref(const std::string& name) { return RuleWrapper(new RuleRef(name)); }

/* RuleText */
RuleText::RuleText(const std::string& text): text{ text } {};

bool RuleText::operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const {
    if (tokens.empty() || tokens.front().text != this->text) return false;
    Parser::push_token(tokens, buffer);
    return true;
}

std::string RuleText::stringify() const { return "\"" + this->text + "\""; }

/* RuleToken */
RuleToken::RuleToken(const std::string& kind): kind{ kind } {};

bool RuleToken::operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const {
    if (tokens.empty() || tokens.front().kind != this->kind) return false;
    Parser::push_token(tokens, buffer);
    return true;
}

std::string RuleToken::stringify() const { return ":" + this->kind + ":"; }

/* RuleRef */
RuleRef::RuleRef(const std::string& name): name{ name } {};

bool RuleRef::operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const {
    return parser.get_rule(name).valid(parser, tokens, buffer);
}

std::string RuleRef::stringify() const { return "<" + this->name + ">"; }

// Functions
std::ostream& operator<<(std::ostream& os, const RuleWrapper& rw) {
    using State = RuleWrapper::State;
    std::string op = "";
    if (rw.state == State::AND) op = " ";
    else if (rw.state == State::OR) op = "|";
    return os << op << rw.element->stringify();
}

std::istream& operator>>(std::istream& is, RuleWrapper& re) {
    char c;
    if (!(is >> c)) throw std::runtime_error("Regra não encontrada.");
    std::string name;
    if (c == '"') {
        while (is >> c && c != '"') name += c;
        if (c != '"') throw std::runtime_error("Fechamento de Texto '\"' não encontrado.");
        re = rule_text(name);
    } else if (c == ':') {
        while (is >> c && c != ':') name += c;
        if (c != ':') throw std::runtime_error("Fechamento de Token ':' não encontrado.");
        re = rule_token(name);
    } else {
        is.putback(c);
        if (!(is >> name)) throw std::runtime_error("Regra de referência não encontrada.");
        if (name.back() == '.' || name.back() == '|') {is.putback(name.back()); name.pop_back(); }
        re = rule_ref(name);
    }
    return is;
}

/* RuleWrapper:: */
// Constructors

// Operators
RuleWrapper RuleWrapper::operator()(int qty) { return RuleWrapper(this->state, qty, qty, this->element); }
RuleWrapper RuleWrapper::operator()(int min, int max) { return RuleWrapper(this->state, min, max, this->element); }

bool RuleWrapper::operator()(Parser& parser, std::list<Token>& tokens, std::list<Token>& buffer) const {
    int valid = 0;
    while (valid < this->max) {
        if (!this->element->operator()(parser, tokens, buffer)) { 
            if (valid >= this->min) break;
            return false; 
        }
        ++valid;
    }
    return valid >= this->min;
}

/* Rule */
// Constructors
Rule::Rule(const std::string& name): _name{ name } {}

// Methods
bool Rule::valid(Parser& parser, std::list<Token>& tokens, std::list<Token>& shared_buffer) { 
    using State = RuleWrapper::State;

    bool valid = false;
    for (const RuleWrapper& r : this->elements) {
        if (r.state == RuleWrapper::State::START) {
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
    std::list<Token> shared_buffer;
    return this->valid(parser, tokens, shared_buffer);
}

// Operators
Rule& Rule::operator<<(RuleWrapper& rule) {
    rule.state = RuleWrapper::State::START;
    this->elements.push_back(rule);
    return *this;
}

Rule& Rule::operator&(RuleWrapper& rule) {
    rule.state = RuleWrapper::State::AND;
    this->elements.push_back(rule);
    return *this;
}

Rule& Rule::operator|(RuleWrapper& rule) {
    rule.state = RuleWrapper::State::OR;
    this->elements.push_back(rule);
    return *this;
}

Rule& Rule::operator<<(std::string& text) { return Rule::operator<<(RuleWrapper(new RuleText(text))); }
Rule& Rule::operator&(std::string& text) { return Rule::operator&(RuleWrapper(new RuleText(text))); }
Rule& Rule::operator|(std::string& text) { return Rule::operator|(RuleWrapper(new RuleText(text))); }


// Functions
std::ostream& operator<<(std::ostream& os, const Rule& r) {
    os << '<' << r.name() << '>' << "::=";
    for (const RuleWrapper& rw : r.elements) os << rw; 
    return os;
}

std::istream& operator>>(std::istream& is, Rule& r) {
    std::string rule_name;
    is >> rule_name;
    if (!is) return is;

    char equal;
    is >> equal;
    if (!is) return is;
    if (equal != '=') throw std::runtime_error("Esperava '=' em vez de '" + std::string(1, equal) + "'.");

    r = Rule(rule_name);
    RuleWrapper re;
    if (!(is >> re)) throw std::runtime_error("Elemento de regra invalido.");;
    r << re;

    char c;
    while (is >> c && c != '.') {
        if (c == '|') {
            if (!(is >> re)) return is;
            r | re;
        } else {
            is.putback(c);
            if (!(is >> re)) return is;
            r & re;
        }
    }
    if (c != '.') throw std::runtime_error("Ponto final da regra em vez de '" + std::string(1, c) + "'.");
    return is;
}

/* Parser */
// Constructor
Parser::Parser(Tokenizer* tokenizer, Grammar* grammar): tokenizer{ tokenizer }, grammar{ grammar } {
    for (const Rule& rule : *grammar) { this->rules[rule.name()] = rule; }
}

// Methods
bool Parser::valid(const std::string& expr, const std::string& rule) {
    std::list<Token> tokens = this->tokenizer->tokenize(expr);
    if (tokens.empty()) return false;
    // std::cout << tokens << '\n';
    Rule& r = this->get_rule(rule);
    bool accepted = r.valid(*this, tokens) && tokens.empty();
    return accepted;
}

bool Parser::has_rule(const std::string& name) const {
    if (this->rules.find(name) != this->rules.end()) return true;
    return false;
}

Rule& Parser::get_rule(const std::string& name) {
    if (this->rules.find(name) == this->rules.end()) throw std::runtime_error{ "Regra: " + name + " não exsite." };
    return this->rules.at(name); 
}

void Parser::push_token(std::list<Token>& tokens, std::list<Token>& buffer) {
    buffer.push_front(tokens.front()); tokens.pop_front();
}

void Parser::revert_tokens(std::list<Token>& tokens, std::list<Token>& buffer) {
    for (const Token& t : buffer) tokens.push_front(t);
    buffer.clear();
}

// Functions
std::istream& operator>>(std::istream& is, Parser::Grammar& rules) {
    Rule r;
    while (is >> r) rules.push_back(r);
    return is;
}