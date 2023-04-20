#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer/classes.hpp"
#include "lexer/utilfunctions.hpp"
#include <deque>

// copyright Ian A. 2023, all rights reserved


enum astnodetype
{
    operation,
    integer,
    stringliteral,
    variable,
    ifs,
    null,
    groupedexpr,
    comparison,
    functioncall,
    functiondeclaration,
    protonodes,
    use,
    variablecall,
};

class astnode
{
    public:

        virtual std::string get_value() const {
            return value;
        }

        const virtual astnodetype get_type(){
            return astnodetype::null;
        }

        virtual const std::string& codegen();

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

        const virtual astnodetype get_type() override {
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

        const virtual astnodetype get_type() override
        {
            return astnodetype::operation;
        }

        binaryoperation(const std::string operation, std::shared_ptr<astnode> right, std::shared_ptr<astnode> left) : operation(operation), right((right)), left((left)) {}

        char get_operation(){
            return operation[0]; // bandaid for switch statements.. will fix when it becomes a problem with operators like += 
        }
        
        const virtual std::string& codegen() {}

};

class boolean : public astnode
{
    std::string value;

    public:
        virtual std::string get_value() const override {
            return value;
        }

        boolean(const std::string& value) : value(value) {}

};

class keyword : public astnode
{
    std::string value;

    public:
        virtual std::string get_value() const override {
            return value;
        }

        keyword(const std::string& value) : value(value) {}
};

class variabledeclaration : public astnode
{
    private:
        std::string variabletype, value, identifier;
        std::vector<std::string> keywords;
    public:

        variabledeclaration(const std::string variabletype, const std::string value, const std::string identifier, const std::vector<std::string> keywords) : variabletype(variabletype), value(value), identifier(identifier), keywords(keywords) {}

        std::string get_identifier() const
        {
            return identifier;
        }

        std::vector<std::string> get_keywords() const {return keywords;}
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

        const virtual astnodetype get_type() override {
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

        const virtual astnodetype get_type() override {
            return astnodetype::variablecall;
        }
        const virtual std::string& codegen() {}
    
        callvariable(const std::string &identifier) : value(identifier) {}
};

class usenode : public astnode
{
    private:
        std::string directory;
    public:
        virtual std::string get_value() const override {
            return directory;
        }

        const virtual astnodetype get_type() override {
            return astnodetype::use;
        }

        const virtual std::string& codegen();
    
        usenode(const std::string &directory) : directory(directory) {}

};

class callfunctionnode : public astnode 
{
    std::string callee;
    std::vector<std::shared_ptr<astnode>> args;

    public:
        const std::string& codegen();

        virtual const astnodetype get_type() override {
            return astnodetype::functioncall;
        }

        virtual std::string get_value() const override {
            return callee;
        }

        callfunctionnode(const std::string &callee, std::vector<std::shared_ptr<astnode>> args) : callee(callee), args((args)) {}
        std::vector<std::shared_ptr<astnode>> getArgs() const {return (args);}
};

class groupedexpre: public astnode
{
    std::string value;
    
    public:
        groupedexpre(std::string value) : value(value) {}
};

class protonode : public astnode
{
    std::string name;
    std::vector<std::string> args;
    std::vector<std::string> argtypes;
    
    public:
        virtual std::string get_value() const override {
            return name + "proto";
        }

        const std::string& codegen();
        const std::vector<std::string> &getTypes() const {return argtypes;}
        protonode(const std::string& name, std::vector<std::string> args, std::vector<std::string> argtypes) : name(name), args(args), argtypes(argtypes) {}
        const std::string &getName() const { return name; }
        const std::vector<std::string> &getArgs() const {return args;}
        const virtual astnodetype get_type() {return astnodetype::protonodes;}
};

class funcdefinitionnode : public astnode
{
    std::vector<std::shared_ptr<astnode>> body;
    std::string returntype;
    std::vector<std::string> argtypes;

    public:
        virtual std::string get_value() const override {
            return "definition";
        }

        std::shared_ptr<protonode> proto;
        const std::vector<std::shared_ptr<astnode>> getBody() {return body;}
        const std::string& codegen();
        const virtual astnodetype get_type() override {return astnodetype::functiondeclaration;}
        std::string getReturnType() const {return returntype;}
        funcdefinitionnode(std::shared_ptr<protonode> proto, std::vector<std::shared_ptr<astnode>> body, std::string returntype) : proto((proto)), body((body)), returntype(returntype) {}
};

class ifstatement : public astnode
{
    std::shared_ptr<astnode> condition;
    std::vector<std::shared_ptr<astnode>> body;

    public:
        std::shared_ptr<astnode> get_condition() {
            return condition;
        }
        const virtual astnodetype get_type() override {return astnodetype::ifs;}
        const std::string& codegen();
        ifstatement(std::shared_ptr<astnode> condition, std::vector<std::shared_ptr<astnode>> body) : condition((condition)), body((body)) {}
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
        {"==", 10},
        {"!=", 10},
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

    std::shared_ptr<keyword> parseKeyword();
    std::shared_ptr<astnode> parseStatement();
    std::shared_ptr<astnode> parseGroupedExpr();
    std::shared_ptr<integerliteral> parseInteger();
    std::shared_ptr<variabledeclaration> parseVariable();
    std::shared_ptr<binaryoperation> parseOperation();
    std::shared_ptr<callvariable> parseCallVariable();
    std::shared_ptr<astnode> parseBoolean();
    std::shared_ptr<ifstatement> parseIfStatement();
    std::shared_ptr<callfunctionnode> parseIdentifierCall();
    std::shared_ptr<protonode> parsePrototype();
    std::shared_ptr<funcdefinitionnode> parseDefinition();
    std::shared_ptr<funcdefinitionnode> parseTopLevelExpr();
    std::shared_ptr<astnode> primaryParserLoop();
    std::shared_ptr<astnode> allOtherParse();
    std::shared_ptr<usenode> parseUse();
    std::deque<std::shared_ptr<astnode>> parseAll();
    
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
         
        if (l->get_value() == "return" || l->get_type() == type::VAR){
            return l;
        }

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
