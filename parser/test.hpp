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

    std::string represent()
    {
        std::string rep = value;
        return rep;
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
            case type::DIVISION:
                return stoi(left.value) / stoi(right.value);
        }
    }
    
    std::string represent()
    {
        std::string rep = left.value + operation.value + right.value;
        return rep;
    }
};

struct Parser 
{
    std::vector<Token> all_tokens;
    std::vector<BinaryOperationNode> all_operationnodes;
    std::vector<LiteralNode> all_literalnodes;
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

    void parse()
    {
        while (get_token().value != "EOF"){
            switch(get_token().types)
            {
                case type::PLUS:
                case type::SUBTRACT:
                case type::TIMES:
                case type::DIVISION:
                    if (get_last_token().types == type::NUM && peek_token().types == type::NUM){
                        all_operationnodes.push_back(BinaryOperationNode {
                            LiteralNode {get_last_token()},
                            LiteralNode {peek_token()},
                            get_token(),
                        });
                        advance();
                    } else {
                        std::cout << "Incorrect use of operand" << get_token().value;
                        break;
                    }
                case type::NUM:
                case type::IDENTIFIER:
                    if (peek_token().types == (type::PLUS || type::SUBTRACT || type::TIMES || type::DIVISION)){
                        all_literalnodes.push_back(LiteralNode {get_token()});
                        advance();
                    } else {
                        advance();
                    }
            }
        }
    }
};

struct error
{
    std::string location;
    std::string type_of_error;
};
