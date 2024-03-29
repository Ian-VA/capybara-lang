#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <vector>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <array>

// copyright Ian A. 2022, all rights reserved

enum type
{
    ASSIGN,
    COMPARE,
    ARGSEPARATE,
    STRING,
    NUM,
    AND,
    PLUS,
    SUBTRACT,
    SUBTRACTEQUALS,
    PLUSEQUALS,
    DIVISION,
    TIMES,
    LESSTHAN,
    LESSTHANOREQUALTO,
    GREATERTHAN,
    GREATERTHANOREQUALTO,
    XOR,
    IF,
    END,
    BEGIN,
    MAIN,
    INTEGER,
    CHAR,
    BOOLEAN,
    ELSEIF,
    ELSE,
    TIMESEQUALS,
    EQUALSEQUALS,
    NOT,
    NOTEQUAL,
    FALSE,
    RETURN,
    TRUE,
    VAR,
    REFERENCE,
    IDENTIFIER,
    LPAREN,
    RPAREN,
    RSQBR,
    LSQBR,
    RCURLY,
    LCURLY,
    ENDINPUT,
    FUNC,
    CONST,
    DEREFERENCE,
    POINTER,
    USE
};

struct Token
{
    type types;
    std::string value;

    friend std::ostream& operator<<(std::ostream& os, const Token& tk);
};

class LEXER
{
    public:
        char ahead;
        std::string m_input;
        Token m_last_token;
        int m_index = 0;
        size_t m_size;

        char current_char(){
            if(m_index <= m_size){
                char r = m_input[m_index];
                return r;
            } 
            return 'e';
        }


        void advance(){
            m_index++;
        }

        char peek(){
            if (m_index + 1 >= m_size){
                return 0;
            }
            return m_input[m_index + 1];
        }
    
};

class ERROR : public LEXER
{
    std::string location;
    std::string type_of_error;
};


std::ostream& operator<<(std::ostream& os, const Token& tk)
{
    os << " token value: " << tk.value << " token type: " << tk.types << "\n";
    return os;
}


#endif

