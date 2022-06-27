#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include "lexer.hpp"
#include "utilfunctions.hpp"

void lexer(std::string input)
{
    LEXER lexer;
    int str_position = 0;
    std::vector<Token> tokens;
    std::string_view in = static_cast<std::string_view>(input);


    while (str_position < input.length()){
        char currentpos = in[str_position];
        bool ispace = isspace(currentpos);
        bool isdigit = std::isdigit(currentpos);

        if (currentpos == '+'){
            tokens.push_back(Token{
                    type::PLUS,
                    '+'
                });
            str_position += 1;
        } else if (currentpos == '*'){
            tokens.push_back(Token{
                    type::TIMES,
                    '*'
                });
            str_position += 1;
        }

        if (ispace){
            str_position += 1;
        } else if (isdigit){
            str_position += 1;
        }

    }

    Token endinput;
    endinput.types = type::ENDINPUT;
    endinput.value = 'E';
    tokens.push_back(endinput);

    print_tokens(tokens);

}

int main()
{
    std::string input = "**+*+";
    lexer(input);
}


