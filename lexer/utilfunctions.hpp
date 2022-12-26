#ifndef UTILFUNCTIONS_HPP
#define UTILFUNCTIONS_HPP
#include <vector>
#include "classes.hpp"
#include <iostream>
// copyright Ian A. 2022, all rights reserved

void print_tokens(std::vector<Token> all_tokens)
{
    std::cout << "Vector size: " << all_tokens.size() << '\n';
    for (auto i : all_tokens)
    {
       std::cout << i;
    }

}

bool isNumber(const std::string& s) // i should probably get rid of this
{
    for (char const &ch : s) {
        if (std::isdigit(ch) == 0)
            return false;
    }
    return true;
}


bool check_if_bool(Token token)
{
    if (token.types != type::FALSE && token.types != type::TRUE){
        return false;
    }
    return true;
}



#endif
