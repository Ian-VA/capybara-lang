#ifndef UTILFUNCTIONS_HPP
#define UTILFUNCTIONS_HPP
#include <vector>
#include "lexer.hpp"
#include <iostream>


bool check_if_bool(Token token)
{
    if (token.types != type::FALSE && token.types != type::TRUE){
        return false;
    }
    return true;
}



#endif
