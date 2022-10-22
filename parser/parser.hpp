#ifndef PARSER_HPP
#define PARSER_HPP

#include "classes.hpp"
#include "utilfunctions.hpp"

enum astnodetype
{
    operation,
    integer,
    stringliteral,
    variable,
    ifstatement,
    boolean,
    null,
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

    std::unique_ptr<integerliteral> parseInteger();
    std::unique_ptr<astnode> parseVariable();
    std::unique_ptr<astnode> parseOperation();
    std::unique_ptr<astnode> parseIfStatement();
    std::unique_ptr<astnode> parseWhileLoop();
    std::unique_ptr<astnode> parseSwitchStatement();
    


};

#endif
