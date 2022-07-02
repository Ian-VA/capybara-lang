#ifndef LEXER_HPP
#define LEXER_HPP
#include <string_view>
#include <string>

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
    TIMESEQUALS,
    EQUALSEQUALS,
    NOT,
    NOTEQUAL,
    FALSE,
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

struct Token
{
    type types;
    std::string value;

    friend std::ostream& operator<<(std::ostream& os, const Token& tk);
};

class LEXER : public Token
{
    public:
        char ahead;
        std::vector<Token> res;
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

