#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include "classes.hpp"
#include "utilfunctions.hpp"
#include <deque>
#include <regex>
#include <map>
#include <array>


Token unique_objects(char current_char, std::string_view input, int index)
{
    LEXER lexer;
    lexer.m_input = input;
    lexer.m_index = index;
    
    switch (lexer.current_char())
    {
        case '&':
            lexer.advance();
            switch (lexer.current_char())
            {
                case '&':
                    return Token {type::AND, "&&"};
                default:
                    return Token {type::REFERENCE, "&"};
            }
    }
    return Token {type::ENDINPUT, "EOF"};
}

Token check_if_keyword(std::string input)
{
    std::map<std::string, type> mapStringtoType = {
        {"true", type::TRUE},
        {"false", type::FALSE},
        {"var", type::VAR},
        {"func", type::FUNC},
        {"const", type::CONST},
        {"if", type::IF},
        {"elseif", type::ELSEIF},
        {"else", type::ELSE},
        {"return", type::RETURN}
    };

    type Type = mapStringtoType[input];
    switch(Type)
    {
        case type::TRUE:
        case type::FALSE:
        case type::VAR:
        case type::FUNC:
        case type::CONST:
        case type::IF:
        case type::ELSEIF:
        case type::ELSE:
            return Token {Type, input};
        default:
            return Token {type::IDENTIFIER, input};
    }
}

Token build_token(std::string_view input)
{
    LEXER lexer_obj;
    lexer_obj.m_input = input;
    bool isdigit = std::isdigit(lexer_obj.current_char());

    if (lexer_obj.m_index >= lexer_obj.m_input.size()){
        return Token {type::ENDINPUT, "EOF"};
    } else if (lexer_obj.current_char() == ' '){
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
        
        while (isalpha(lexer_obj.peek()) || std::isdigit(lexer_obj.peek()))
        {
            lexer_obj.advance();
            all_input.push_back(lexer_obj.current_char());
        }
        return check_if_keyword(all_input);
    }

    switch(lexer_obj.current_char())
    {
        case '+':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::PLUSEQUALS, "+="};
                default:
                    return Token {type::PLUS, "+"};
            }
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
                case '+':
                    return Token {type::PLUSEQUALS, "=+"};
                case '-':
                    return Token {type::SUBTRACTEQUALS, "=-"};
                default:
                    return Token {type::ASSIGN, "="};
            }
        case '-':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::SUBTRACTEQUALS, "-="};
                default:
                    return Token {type::SUBTRACT, "-"};
            }
        case '<':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::LESSTHANOREQUALTO, "<="};
                default:
                    return Token {type::LESSTHAN, "<"};
            }
        case '>':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::GREATERTHANOREQUALTO, ">="};
                default:
                    return Token {type::GREATERTHAN, ">"};
            }
        case '^':
            lexer_obj.advance();
            return Token {type::XOR, "^"};
        case '!':
            lexer_obj.advance();
            switch(lexer_obj.current_char())
            {
                case '=':
                    return Token {type::NOTEQUAL, "!="};
                default:
                    return Token {type::NOT, "!"};
            }
        case '/':
            lexer_obj.advance();
            return Token {type::DIVISION, "/"};
        case '(':
            lexer_obj.advance();
            return Token {type::LPAREN, "("};
        case ')':
            lexer_obj.advance();
            return Token {type::RPAREN, ")"};
        case '{':
            lexer_obj.advance();
            return Token {type::LCURLY, "{"};
        case '}':
            lexer_obj.advance();
            return Token {type::RCURLY, "}"};
        case '[':
            lexer_obj.advance();
            return Token {type::LSQBR, "["};
        case ']':
            lexer_obj.advance();
            return Token {type::RSQBR, "]"};
        case ',':
            lexer_obj.advance();
            return Token {type::ARGSEPARATE, ","};
        default:
            return unique_objects(lexer_obj.current_char(), lexer_obj.m_input, lexer_obj.m_index);
    }

}

std::string removeSpaces(std::string &str)
{
    int n = str.length();
    int i = 0, j = -1;
    bool spaceFound = false;
 
    while (++j < n && str[j] == ' ');
 
    while (j < n)
    {
        if (str[j] != ' ')
        {
            if ((str[j] == '.' || str[j] == ',' ||
                 str[j] == '?') && i - 1 >= 0 &&
                 str[i - 1] == ' '){
                    str[i - 1] = str[j++];
                 } else {
                    str[i++] = str[j++];
                 }

            spaceFound = false;
        }
        else if (str[j++] == ' ')
        {
            if (!spaceFound)
            {
                str[i++] = ' ';
                spaceFound = true;
            }
        }
    }
    if (i <= 1){
        str.erase(str.begin() + i, str.end());
    } else {
        str.erase(str.begin() + i - 1, str.end());
    } 
    return str;
}

std::deque<Token> build_all(std::string input)
{
    LEXER lexer_object;
    lexer_object.m_index = 0;
    input.push_back(' ');
    input = removeSpaces(input);
    lexer_object.m_input = input;
    std::deque<Token> all_tokens;

    while (true)
    {
        Token token = build_token(input);
        size_t position = input.find(token.value);

        all_tokens.push_back(token);

        if (position != std::string::npos){
            input.erase(position, token.value.length());
        }

        
        input.erase(remove(input.begin(), input.end(), ' '), input.end());


        if(token.types == type::ENDINPUT){
            break;
        }

        lexer_object.advance();

    }


    return all_tokens;

}



#endif
