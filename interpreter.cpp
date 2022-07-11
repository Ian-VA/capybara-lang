#include "parser.hpp"
#include "lexer.hpp"


int main()
{
    std::string input = "5 + 3";
    Parser parse;
    parse.all_tokens = buid_all(input)

    while (parse.parse() != Token {type::ENDINPUT, "EOF"} || parser.parse() != ERROR {})
    {
        parse.parse();
    }

    build_all(input);
}
