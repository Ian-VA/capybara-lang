#ifndef PARSER_HPP
#define PARSER_HPP

#include "classes.hpp"
#include "utilfunctions.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

enum astnodetype
{
    operation,
    integer,
    stringliteral,
    variable,
    ifstatement,
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

        const std::string get_value() const {
            return value;
        }

        virtual astnodetype get_type() const {
            return astnodetype::null;
        }

        virtual Value* codegen();

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

        virtual Value* codegen();

        virtual astnodetype get_type() const override {
            return astnodetype::integer;
        }

        std::string get_value() const {
            return value;
        }
};

class binaryoperation : public astnode
{
    private:
        std::string operation;
    public:
        virtual Value* codegen();

        std::unique_ptr<astnode> left;
        std::unique_ptr<astnode> right;

        virtual astnodetype get_type() const override
        {
            return astnodetype::operation;
        }

        binaryoperation(const std::string operation, std::unique_ptr<astnode> right, std::unique_ptr<astnode> left) : operation(operation), right(std::move(right)), left(std::move(left)) {}

        char get_operation(){
            return operation[0]; // bandaid for switch statements.. will fix when it becomes a problem with operators like += 
        }
};

class variabledeclaration : public astnode
{
    private:
        std::string variabletype, value, identifier;
    public:
        virtual Value* codegen();

        variabledeclaration(const std::string variabletype, const std::string value, const std::string identifier) : variabletype(variabletype), value(value), identifier(identifier) {}

        std::string get_identifier() const
        {
            return identifier;
        }

        std::string get_value() const
        {
            return value;
        }

        std::string getvariabletype() const
        {
            return std::string("placeholder");
        }

        virtual astnodetype get_type() const override {
            return astnodetype::variable;
        }

};

class callvariable : public astnode
{
    private:
        std::string identifier;
    public:
        const std::string get_identifier() const {
            return identifier;
        }

        virtual Value* codegen();
        callvariable(const std::string &identifier) : identifier(identifier) {}
};

class callfunctionnode : public astnode 
{
    std::string callee;
    std::vector<std::unique_ptr<astnode>> args;

    public:
        virtual Value* codegen();
        callfunctionnode(const std::string &callee, std::vector<std::unique_ptr<astnode>> args) : callee(callee), args(std::move(args)) {}
};

class protonode 
{
    std::string name;
    std::vector<std::string> args;

    public:
        virtual Value* codegen();
        protonode(const std::string& name, std::vector<std::string> args) : name(name), args(args) {}
        const std::string &getName() const { return name; }
};

class funcdefinitionnode
{
    std::unique_ptr<protonode> proto;
    std::unique_ptr<astnode> body;

    public:
        Value* codegen();
        funcdefinitionnode(std::unique_ptr<protonode> proto, std::unique_ptr<astnode> body) : proto(std::move(proto)), body(std::move(body)) {}
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

    std::unique_ptr<astnode> parseStatement();
    std::unique_ptr<astnode> parseGroupedExpr();

    std::unique_ptr<astnode> parseInteger();
    std::unique_ptr<astnode> parseVariable();
    std::unique_ptr<astnode> parseOperation();
    std::unique_ptr<astnode> parseFactor();

    std::unique_ptr<astnode> parseComparison();
    std::unique_ptr<astnode> parseIdentifierCall();
    std::unique_ptr<protonode> parsePrototype();
    std::unique_ptr<funcdefinitionnode> parseDefinition();
    std::unique_ptr<funcdefinitionnode> parseTopLevelExpr();
    std::unique_ptr<astnode> primaryParserLoop();

    std::unique_ptr<astnode> parseOperationRHS(int exprprec, std::unique_ptr<astnode> l) { // l here being a parsed binop
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
                r = parseOperationRHS(tokprec+1, std::move(r));

                if (!r) {
                    return nullptr;
                }
            }

            l = std::make_unique<binaryoperation> (binop, std::move(r), std::move(l));

        }
    }

    std::unique_ptr<astnode> parseExpression() {
        if (get_token().value == "EOF") {
            return nullptr;
        }

        auto l = primaryParserLoop();
        if (!l) {
            return nullptr;
        } else {
            return parseOperationRHS(0, std::move(l));
        }
    }

};

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<variabledeclaration> variable)
{
    os << "REPRESENT VARIABLE" << "\n" << "TYPE: " << variable->getvariabletype() << "\n" << "IDENTIFIER: " << variable->get_identifier() << "\n" << "VALUE: " << variable->get_value() << "\n";
    return os;
}

#endif
