#include "classes.hpp"


class LiteralNode
{
    public:
        LiteralNode(Token token){
            if (!(token.types == type::NUM || token.types == type::IDENTIFIER)){
                return;
            }

            this->literalnodetype = token.types;
            this->value = token.value;
        }
    private:
        std::string value;
        type literalnodetype;

};

class BinaryOperationNode
{
    LiteralNode left;
    LiteralNode right;
    Token operation;
};

class Parser 
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

    void advance(){
        index += 1;
    }

    LiteralNode parseliteral(Token token){
        return LiteralNode {token};
    }
};
