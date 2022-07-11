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

    Token get_last_token()
    {
        return all_tokens[index - 1];
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

    template <class T>
    T parse()
    {
        switch(get_token().types)
        {
            case type::PLUS:
            case type::SUBTRACT:
            case type::TIMES:
            case type::DIVISION:
                if (get_last_token().types == type::NUM && peek_token().types == type::NUM){
                    return BinaryOperationNode {
                        get_token(),
                        LiteralNode {get_last_token().types, get_last_token().value},
                        LiteralNode {peek_token().types, peek_token().value}
                    };
                    advance();
                } else {
                    return ERROR {" ", "Incorrect use of operand" + get_last_token().value};
                    break;
                }
            case type::NUM:
            case type::IDENTIFIER:
                if (peek_token().types != (type::PLUS || type::SUBTRACT || type::TIMES || type::DIVISION)){
                    return LiteralNode {get_token().types, get_token().value};
                    advance();
                } else {
                    advance();
                }
            case type::ENDINPUT:
                return Token {type::ENDINPUT, "EOF"};
        }
    }
};

struct error
{
    std::string location;
    std::string type_of_error;
};

