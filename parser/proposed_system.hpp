#ifndef PARSERFUNCTIONS_HPP
#define PARSERFUNCTIONS_HPP

#include "classes.hpp"

struct Parser 
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

    int parseSum();
    int parseProduct();

    int parseFactor(Token token)
    {
        int data = std::stoi(token.value);
        std::cout << data << "\n";
        if (token.types == type::NUM){
            eat();
            return data;
        } else {
            std::cout << "Expected digit, got " << token.value << " instead" << "\n";
            return 0;
        }
    }



};



#endif
