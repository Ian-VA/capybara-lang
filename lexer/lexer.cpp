#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include "lexer.hpp"
#include "utilfunctions.hpp"
#include <regex>
#include <map>
#include <array>


LEXER unique_objects(std::string_view input)
{
    LEXER lexer;
    return lexer;
}

Token check_if_keyword(std::string input)
{
    std::map<std::string, type> mapStringtoType = {
        {"true", type::TRUE},
        {"false", type::FALSE},
        {"var", type::VAR},
        {"func", type::FUNC},
        {"const", type::CONST}
    };

    type Type = mapStringtoType[input];
    switch(Type)
    {
        case type::TRUE:
            return Token {type::TRUE, input};
        case type::FALSE:
            return Token {type::FALSE, input};
        case type::VAR:
            return Token {type::VAR, input};
        case type::FUNC:
            return Token {type::FUNC, input};
        default:
            return Token {type::IDENTIFIER, input};
    }
}


Token build_token(std::string_view input)
{
    LEXER lexer_obj;
    lexer_obj.m_input = input;
    bool ispace = isspace(lexer_obj.current_char()); 
    bool isdigit = std::isdigit(lexer_obj.current_char());

    if (lexer_obj.m_index >= lexer_obj.m_input.size()){
        return Token {type::ENDINPUT, "EOF"};
    } else if (ispace){
        lexer_obj.advance();
    } else if (isdigit){
        std::string digit;
        digit.push_back(lexer_obj.current_char());
        
        while (std::isdigit(lexer_obj.peek()))
        {
            lexer_obj.advance();
            digit.push_back(lexer_obj.current_char());
        }

        return Token {type::NUM, digit};

    } else if (isalpha(lexer_obj.current_char())){
        std::string all_input;
        all_input.push_back(lexer_obj.current_char());
        
        while (isalpha(lexer_obj.peek()))
        {
            lexer_obj.advance();
            all_input.push_back(lexer_obj.current_char());
        }
        return check_if_keyword(all_input);
    }

    switch(lexer_obj.current_char())
    {
        case ' ':
            lexer_obj.advance();
        case '+':
            lexer_obj.advance();
            return Token {type::PLUS, "+"};
    
        case '*':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::TIMESEQUALS, "*="};
                default:
                    return Token {type::TIMES, "*"};
            }
        
        case '=':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::EQUALSEQUALS, "=="};
                default:
                    return Token {type::ASSIGN, "="};
            }

        default:
            return Token {type::ENDINPUT, "EOF"};
    }

}

std::string build_all(std::string input)
{
    const int size = input.length();
    char new_input[size + 1];
    strcpy(new_input, input.c_str());
    std::vector<Token> all_tokens;

    char *ptr = strtok(new_input, " ");

    while(ptr != NULL)
    {
        all_tokens.push_back(build_token(ptr));
        ptr = strtok(NULL, " ");
    }

    for (auto i : all_tokens)
    {
        std::cout << i;
    }
}

int main()
{
    std::string input = "hello false foo var func12 34598 126";
    build_all(input);
    std::cout << (input);
}
