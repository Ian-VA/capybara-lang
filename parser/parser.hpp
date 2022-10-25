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
    groupedexpr,
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
        std::unique_ptr<astnode> left;
        std::unique_ptr<astnode> right;
    public:

        virtual astnodetype get_type()
        {
            return astnodetype::variable;
        }

        binaryoperation(std::string operations, std::unique_ptr<astnode> rights, std::unique_ptr<astnode> lefts)
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

class callfunctionnode : public astnode 
{
    std::string callee;
    std::vector<std::unique_ptr<astnode>> args;

    public:
        void accept(Visitor& visitor);

        callfunctionnode(const std::string &callee, std::vector<std::unique_ptr<astnode>> args) : callee(callee), args(std::move(args)) {}
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
    std::unique_ptr<astnode> parseIfStatement();
    std::unique_ptr<astnode> parseWhileLoop();
    std::unique_ptr<astnode> parseSwitchStatement();
    std::unique_ptr<astnode> parseIdentifierCall();
    std::unique_ptr<astnode> primaryParserLoop();

    std::unique_ptr<astnode> parseOperationRHS(int exprprec, std::unique_ptr<astnode> l) { // l here being a parsed binop
        while (true)
        {
            int tokprec = get_precedence(get_token());

            if (tokprec < exprprec) {
                return l;
            }

            char binop = get_token();
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

            l = std::make_unique<binaryoperation> (binop, std::move(l), std::move(r));

        }
    }

    std::unique_ptr<astnode> parseExpression() {
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
