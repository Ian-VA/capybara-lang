#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include <regex>
#include <map>
#include <array>


enum type
{
    ASSIGN,
    COMPARE,
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
    CONST
};

union S
{
    uint32_t valueint;
    std::string value;
    
    ~S();
    S();
};

struct Token
{
    type types;
    std::string value;
    S valuecontainer;
    
    
    template <typename T>
    Token(type types, uint32_t value){
        this->types = types;
        this->value = valuecontainer;
    }
    
    Token (type types, std::string value){
        this->types = types;
        this->value = value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& tk);
};

class LEXER
{
    public:
        char ahead;
        std::string_view m_input;
        Token m_last_token;
        int m_index = 0;
        size_t m_size;

        char current_char(){
            if(m_index <= m_size){
                char r = m_input[m_index];
                return r;
            } 
            return ' ';
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


