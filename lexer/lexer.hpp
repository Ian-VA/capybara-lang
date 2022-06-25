enum type
{
    NUM,
    PLUS,
    TIMES,
    FALSE,
    TRUE,
    LPAREN,
    RPAREN,
    FUNCTION,
    ENDINPUT
};


struct Token
{
    type types;
    char value;
};
