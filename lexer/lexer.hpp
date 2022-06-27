#ifndef LEXER_HPP
#define LEXER_HPP

enum type
{
    NUM,
    PLUS,
    TIMES,
    FALSE,
    TRUE,
    IDENTIFIER,
    LPAREN,
    RPAREN,
    RSQBR,
    LSQBR,
    RCURLY,
    LCURLY,
    FUNCTION,
    ENDINPUT
};

class LEXER
{
    int lparennum = 0;
    int rparennum = 0;
    int rsqbrnum = 0;
    int lsqbrnum = 0;
    int rcurly = 0;
    int lcurly = 0;
    char ahead;
};

class ERROR : public LEXER
{
    std::string location;
    std::string type_of_error;
};

struct Token
{
    type types;
    char value;

    friend std::ostream& operator<<(std::ostream& os, const Token& tk);
};

std::ostream& operator<<(std::ostream& os, const Token& tk)
{
    os << " TOKEN VAL: " << tk.value << " TOKEN TYPE: " << tk.types << "\n";
    return os;
}


#endif

