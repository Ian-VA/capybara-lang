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

struct LEXER
{
    int l_paren_num = 0;
    int r_paren_num = 0;
    int rsq_br_num = 0;
    int lsq_br_num = 0;
    int rcurly = 0;
    int lcurly = 0;
    std::string l_error;
    char ahead;
    std::string l_location;
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

