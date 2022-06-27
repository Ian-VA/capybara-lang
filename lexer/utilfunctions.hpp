#ifndef UTILFUNCTIONS_HPP
#define UTILFUNCTIONS_HPP
#include <vector>
#include "lexer.hpp"
#include <iostream>

void print_tokens(std::vector<Token> all_tokens)
{
    std::cout << "Vector size: " << all_tokens.size() << '\n';
    for (auto i : all_tokens)
    {
       std::cout << i.types;
    }

}


bool check_if_bool(Token token)
{
    if (token.types != type::FALSE && token.types != type::TRUE){
        return false;
    }
    return true;
}



#endif
