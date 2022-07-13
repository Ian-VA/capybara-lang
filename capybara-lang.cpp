#include <iostream>
#include <string>
#include "lexer.hpp"
#include "test.hpp"



std::string evaluate(std::string input)
{
    return input;
}

std::string readln(std::string input)
{
    return input;
}

std::string out(std::string input)
{
    return input;
}

int rep(std::string input)
{
    Parser parses;
    std::vector<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;

    parses.parse();

    for (auto i : parses.all_operationnodes){
        return i.evaluate();
    }

    return 0;
}



int main()
{
    std::string input;
    while (true) {
        std::cout << "capybara> ";
        if (!std::getline(std::cin, input)) {
            break;
        } else {
            std::cout << rep(input) << "\n";
        }
    }
}
