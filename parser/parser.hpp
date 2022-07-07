#include "classes.hpp"


struct LiteralNode
{
    LiteralNode(Token token){
        if (!(token.types == type::NUM || token.types == type::IDENTIFIER)){
            return;
        }

        this->literalnodetype = token.types;
        this->value = token.value;
     }
    std::string value;
    type literalnodetype;

};

struct BinaryOperationNode
{
    LiteralNode left;
    LiteralNode right;
    Token operation;

    int evaluate(){
        switch (operation.types)
        {
            case type::PLUS:
                return stoi(left.value) + stoi(right.value);
            case type::TIMES:
                return stoi(left.value) * stoi(right.value);
            case type::SUBTRACT:
                return stoi(left.value) - stoi(right.value);
        }
    }
};

struct Parser 
{
    std::vector<Token> all_tokens;
    int index = 1;

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

    void advance(){
        index += 1;
    }

    LiteralNode parseliteral(Token token){
        return LiteralNode {token};
    }

    BinaryOperationNode parsebinary(Token token, LiteralNode left, LiteralNode right){
        return BinaryOperationNode {left, right, token};
    }
};

struct error
{
    std::string location;
    std::string type_of_error;
};
