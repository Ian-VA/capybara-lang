#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <vector>
#include <iostream>
#include <string>
<<<<<<< HEAD
=======
#include <string_view>
>>>>>>> 8c4ac030fef114cdc84582cee1738729e21975fe
#include <regex>
#include <map>
#include <array>

// copyright Ian A. 2022, all rights reserved

enum type
{
    ASSIGN,
    COMPARE,
    ARGSEPARATE,
<<<<<<< HEAD
    STRING,
=======
>>>>>>> 8c4ac030fef114cdc84582cee1738729e21975fe
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
<<<<<<< HEAD
    END,
    BEGIN,
    MAIN,
    INTEGER,
    CHAR,
    BOOLEAN,
=======
>>>>>>> 8c4ac030fef114cdc84582cee1738729e21975fe
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
<<<<<<< HEAD
    CONST,
    DEREFERENCE,
    POINTER
=======
    CONST
>>>>>>> 8c4ac030fef114cdc84582cee1738729e21975fe
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
<<<<<<< HEAD
        std::string m_input;
=======
        std::string_view m_input;
>>>>>>> 8c4ac030fef114cdc84582cee1738729e21975fe
        Token m_last_token;
        int m_index = 0;
        size_t m_size;

        char current_char(){
            if(m_index <= m_size){
                char r = m_input[m_index];
                return r;
            } 
<<<<<<< HEAD
            return 'e';
=======
            return ' ';
>>>>>>> 8c4ac030fef114cdc84582cee1738729e21975fe
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

