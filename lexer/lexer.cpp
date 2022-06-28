#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include "lexer.hpp"
#include "utilfunctions.hpp"



LEXER unique_objects(std::string_view input)
{
    LEXER lexer;
    return lexer;
}

void print_tokens(Token token)
{
    std::cout << token << "\n";
}

Token return_one_token(std::string input)
{
    LEXER lexer_obj;
    int str_position = 0;
    lexer_obj.m_input = static_cast<std::string_view>(input);
    bool ispace = isspace(lexer_obj.current_char());
    bool isdigit = std::isdigit(lexer_obj.current_char());

    if(ispace){
        lexer_obj.advance();
    }

    switch(lexer_obj.current_char())
    {
        case '+':
        {
            lexer_obj.advance();
            return Token {type::PLUS, "+"};
            
        }
        case '*':
        {
            lexer_obj.advance();
            return Token {type::TIMES, "*"};
        }
        case '=':
        {
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::EQUALSEQUALS, "=="};
                default:
                    return Token {type::ASSIGN, "="};
            }
        }

    }

    return Token {type::ENDINPUT, "EOF"};

}

int main()
{
    std::string input = "==";
    print_tokens(return_one_token(input));
}

