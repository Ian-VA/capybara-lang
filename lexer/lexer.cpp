#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include "header.hpp"

void lexer(std::string input)
{
    int str_position = 0;
    std::vector<Token> tokens = {};
    std::string_view in = static_cast<std::string_view>(input);

    while (str_position < in.length()){
        
        int startpos = str_position;
        char ahead = in[startpos] + 1;
        bool ispace = isspace(ahead);
        bool isdigit = std::isdigit(ahead);

        if (ispace){
            str_position += 1;
        } else if (isdigit){

        }

        switch (ahead)
        {
            case '+':
            {
                str_position += 1;
                Token token;
                token.types = type::PLUS;
                token.value = '+';
                tokens.push_back(token);
            }

            case '*':
            {
                str_position += 1;
                Token token;
                token.types = type::TIMES;
                token.value = '*';
                tokens.push_back(token);
            }

        }
    }



    Token endinput;
    endinput.types = type::ENDINPUT;
    tokens.push_back(endinput);

}

int main()
{
    
}
