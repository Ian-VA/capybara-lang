#ifndef PARSER_HPP
#define PARSER_HPP

#include "classes.hpp"
#include "utilfunctions.hpp"
#include <deque>


enum astnodetype
{
    operation,
    integer,
    stringliteral,
    variable,
    ifs,
    boolean,
    null,
    groupedexpr,
    comparison,
    functioncall,
    functiondeclaration,
};

class astnode
{
    public:

        virtual std::string get_value() const {
            return value;
        }

        virtual astnodetype get_type() const {
            return astnodetype::null;
        }

        virtual const std::string& codegen();
        bool isReturnExpression = false;

    private:
        std::string value;

};

class integerliteral : public astnode
{
    private:
        std::string value;

    public:
        integerliteral(Token tok) : value(tok.value) {}
        integerliteral(std::string val) : value(val) {}

        virtual astnodetype get_type() const override {
            return astnodetype::integer;
        }

        virtual std::string get_value() const override{
            return value;
        }

        const virtual std::string& codegen();

};

class binaryoperation : public astnode
{
    private:
        std::string operation;
    public:
        std::shared_ptr<astnode> left;
        std::shared_ptr<astnode> right;

        virtual std::string get_value() const override {
            return left->get_value() + operation + right->get_value();
        }

        virtual astnodetype get_type() const override
        {
            return astnodetype::operation;
        }

        binaryoperation(const std::string operation, std::shared_ptr<astnode> right, std::shared_ptr<astnode> left) : operation(operation), right((right)), left((left)) {}

        char get_operation(){
            return operation[0]; // bandaid for switch statements.. will fix when it becomes a problem with operators like += 
        }
        
        const virtual std::string& codegen();

};

class variabledeclaration : public astnode
{
    private:
        std::string variabletype, value, identifier;
    public:

        variabledeclaration(const std::string variabletype, const std::string value, const std::string identifier) : variabletype(variabletype), value(value), identifier(identifier) {}

        std::string get_identifier() const
        {
            return identifier;
        }

        virtual std::string get_value() const 
        {
            return value;
        }

        std::string getvariabletype() const
        {
            if (isNumber(this->get_value())){
                return std::string("int");

            } else if (std::isalpha(this->get_value()[0])) {
                if (this->get_value().size() <= 1) {
                    return std::string("char");
                }
                return std::string("string");

            }
        }

        virtual astnodetype get_type() const override {
            return astnodetype::variable;
        }

        const virtual std::string& codegen();

};

class callvariable : public astnode
{
    private:
        std::string value;
    public:
        virtual std::string get_value() const override {
            return value;
        }
        const virtual std::string& codegen();
    
        callvariable(const std::string &identifier) : value(identifier) {}
};

class callfunctionnode : public astnode 
{
    std::string callee;
    std::vector<std::shared_ptr<astnode>> args;

    public:
        const std::string& codegen();
        callfunctionnode(const std::string &callee, std::vector<std::shared_ptr<astnode>> args) : callee(callee), args((args)) {}
        const std::vector<std::shared_ptr<astnode>> &getArgs() const {return (args);}
};

class protonode 
{
    std::string name;
    std::vector<std::string> args;

    public:
        const std::string& codegen();
        protonode(const std::string& name, std::vector<std::string> args) : name(name), args(args) {}
        const std::string &getName() const { return name; }
        const std::vector<std::string> &getArgs() const {return args;}
};

class funcdefinitionnode
{
    std::shared_ptr<protonode> proto;
    std::shared_ptr<astnode> body;
    std::string returntype;

    public:
        const std::string& getBody() {return body->get_value();}
        const std::string& codegen();
        const std::string& getReturnType() const {return returntype;}
        funcdefinitionnode(std::shared_ptr<protonode> proto, std::shared_ptr<astnode> body) : proto((proto)), body((body)) {}
};

class ifstatement
{
    std::shared_ptr<astnode> condition;
    std::shared_ptr<astnode> body;

    public:
        std::shared_ptr<astnode> get_condition() {
            return (condition);
        }

        ifstatement(std::shared_ptr<astnode> condition, std::shared_ptr<astnode> body) : condition((condition)), body((body)) {}
};

class error
{
    public:
        error(int m_line, std::string error, std::string note){
            std::cout << "Error encountered at line " << m_line << ": " << error << "\n";
            if (!note.empty()){
                std::cout << "Note: " << note << "\n";
            }
            std::cout << "Aborting.." << "\n";
            exit(1);
        }
};

struct parserclass
{
    std::deque<Token> all_tokens;
    int curr_line = 1;
    bool encountered_main = false;
    int index = 0;

    std::map<std::string, int> precedence = {
        {"<", 10},
        {">", 10},
        {"+", 20},
        {"-", 20},
        {"*", 40},
        {"/", 40}
    };

    int get_precedence(Token tok)
    {
        int tokprec = precedence[tok.value];

        if (tokprec <= 0) return -1;
        return tokprec;
    }

    Token get_token()
    {
        if (index > all_tokens.size()){
            return Token {type::ENDINPUT, "EOF"};
        }
        return all_tokens[index];
    }

    Token peek_token()
    {
        if(index + 1 > all_tokens.size()){
            return Token {type::ENDINPUT, "EOF"};
        }
        return all_tokens[index + 1];
    }

    void eat(){
        all_tokens.pop_front();
    }

    std::shared_ptr<astnode> parseStatement();
    std::shared_ptr<astnode> parseGroupedExpr();

    std::shared_ptr<integerliteral> parseInteger();
    std::shared_ptr<variabledeclaration> parseVariable();
    std::shared_ptr<binaryoperation> parseOperation();
    std::shared_ptr<callvariable> parseCallVariable();
    std::shared_ptr<astnode> parseComparison();
    std::shared_ptr<ifstatement> parseIfStatement();
    std::shared_ptr<callfunctionnode> parseIdentifierCall();
    std::shared_ptr<protonode> parsePrototype();
    std::shared_ptr<funcdefinitionnode> parseDefinition();
    std::shared_ptr<funcdefinitionnode> parseTopLevelExpr();
    std::shared_ptr<astnode> primaryParserLoop();
    
    std::shared_ptr<astnode> parseOperationRHS(int exprprec, std::shared_ptr<astnode> l) { // l here being a parsed identifier or number (anything else is returned automatically)
        while (true)
        {
            int tokprec = get_precedence(get_token());

            if (tokprec < exprprec) {
                return l;
            }

            std::string binop = get_token().value;
            eat();

            auto r = primaryParserLoop();

            if (!r) {
                return nullptr;
            }

            int nextprec = get_precedence(get_token());

            if (tokprec < nextprec) {
                r = parseOperationRHS(tokprec+1, (r));

                if (!r) {
                    return nullptr;
                }
            }

            l = std::make_shared<binaryoperation> (binop, (r), (l));

        }
    }

    std::shared_ptr<astnode> parseExpression() {
        if (get_token().value == "EOF") {
            return nullptr;
        }

        auto l = primaryParserLoop();
        if (!l) {
            return nullptr;
        } else {
            return parseOperationRHS(0, (l));
        }
    }

};

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<variabledeclaration> variable)
{
    os << "REPRESENT VARIABLE" << "\n" << "TYPE: " << variable->getvariabletype() << "\n" << "IDENTIFIER: " << variable->get_identifier() << "\n" << "VALUE: " << variable->get_value() << "\n";
    return os;
}

#endif
