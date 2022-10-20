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
        virtual astnodetype get_type();
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

        astnodetype get_type() {
            return astnodetype::integerliteral;
        }

        std::string get_value(){
            return value;
        }
};

class binaryoperation : public astnode
{
    private:
        std::string operation;
    public:

        binaryoperation(Token tok)
        {
            this->operation = tok.value;
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

        astnodetype get_type(){
            return astnodetype::variable;
        }
};

struct parserclass
{
    std::deque<Token> all_tokens;
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

    astnode parseInteger();
    astnode parseVariable();
    astnode parseOperation();



};

#endif 
