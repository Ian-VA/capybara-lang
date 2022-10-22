#ifndef PARSER_HPP
#define PARSER_HPP

#include "classes.hpp"
#include "utilfunctions.hpp"

class Visitor;

enum astnodetype
{
    operation,
    integer,
    stringliteral,
    variable,
    ifstatement,
    boolean,
    null,
    comparison,
    functioncall,
    functiondeclaration,
};

class astnode
{
    public:
        virtual astnodetype get_type(){
            return astnodetype::null;
        }

    private:
        std::string value;

};

class integerliteral : public astnode
{
    private:
        std::string value;

    public:

        integerliteral(Token tok)
        {
            this->value = tok.value;
        }

        virtual astnodetype get_type() {
            return astnodetype::integer;
        }

        void accept(Visitor& visitor);

        std::string get_value(){
            return value;
        }
};

class binaryoperation : public astnode
{
    private:
        std::string operation;
        std::unique_ptr<integerliteral> left;
        std::unique_ptr<integerliteral> right;
    public:

        virtual astnodetype get_type()
        {
            return astnodetype::variable;
        }

        binaryoperation(std::string operations, std::unique_ptr<integerliteral> rights, std::unique_ptr<integerliteral> lefts)
        {
            this->operation = operations;
            this->left = std::move(lefts);
            this->right = std::move(rights);
        }

        void accept(Visitor& visitor);


        std::string get_operation(){
            return operation;
        }
};

class variabledeclaration : public astnode
{
    private:
        std::string variabletype, value, identifier;
    public:

        variabledeclaration(std::string variabletypes, std::string values, std::string identifiers)
        {
            this->identifier = identifiers;
            this->variabletype = variabletypes;
            this->value = values;
        }


        void accept(Visitor& visitor);


        std::string get_identifier()
        {
            return identifier;
        }

        std::string get_value()
        {
            return value;
        }


        std::string getvariabletype()
        {
            return std::string("placeholder");
        }

        virtual astnodetype get_type(){
            return astnodetype::variable;
        }

};

struct parserclass
{
    std::deque<Token> all_tokens;
    int curr_line = 1;
    int index = 0;

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
    std::unique_ptr<astnode> parseExpression();
    std::unique_ptr<astnode> parseGroupedExpr();

    std::unique_ptr<integerliteral> parseInteger();
    std::unique_ptr<variabledeclaration> parseVariable();
    std::unique_ptr<astnode> parseOperation();
    std::unique_ptr<astnode> parseFactor();

    std::unique_ptr<astnode> parseComparison();
    std::unique_ptr<astnode> parseIfStatement();
    std::unique_ptr<astnode> parseWhileLoop();
    std::unique_ptr<astnode> parseSwitchStatement();
    


};

std::ostream& operator<<(std::ostream& os, const std::unique_ptr<variabledeclaration> variable)
{
    os << "REPRESENT VARIABLE" << "\n" << "TYPE: " << variable->getvariabletype() << "\n" << "IDENTIFIER: " << variable->get_identifier() << "\n" << "VALUE: " << variable->get_value() << "\n";
    return os;
}

#endif
