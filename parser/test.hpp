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
    BinaryOperationNode* binleft;
    LiteralNode *left;
    LiteralNode *right;
    Token *operations;

    BinaryOperationNode(BinaryOperationNode *bleft, LiteralNode *right, Token *operation)
    {
        this->binleft = bleft;
        this->right = right;
        this->operations = operation;
    }

    BinaryOperationNode(LiteralNode *left, LiteralNode *right, Token *operation)
    {
        this->left = left;
        this->right = right;
        this->operations = operation;
    }
    
};

struct Parser 
{
    std::deque<Token> all_tokens;
    std::deque<BinaryOperationNode> all_operationnodes;
    std::deque<LiteralNode> all_literalnodes;
    int index = 0;
    int binopindex = 0;

    Token get_token()
    {
        if (index > all_tokens.size()){
            return Token {type::ENDINPUT, "EOF"};
        }
        return all_tokens[index];
    }

    BinaryOperationNode get_last_binop()
    {
        return all_operationnodes[binopindex - 1];
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
                        LiteralNode literalnode1 = {get_last_token()};
                        LiteralNode literalnode2 {peek_token()};
                        Token token = get_token();
                        BinaryOperationNode newnode = {literalnode1, literalnode2, token};
                        all_operationnodes.push_back(newnode);

                        for (int i = 0; i < 3; i++){
                            all_tokens.pop_front();
                        }
                    } else {

                        all_operationnodes.push_back(BinaryOperationNode {
                            BinaryOperationNode {get_last_binop()},
                            LiteralNode {peek_token()},
                            get_token()
                        });

                        binopindex++;
                    }
                    
                case type::NUM:
                case type::IDENTIFIER:
                    if (peek_token().types != (type::PLUS || type::SUBTRACT || type::TIMES || type::DIVISION || type::ENDINPUT) ){
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
