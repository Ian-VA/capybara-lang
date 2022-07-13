#include "test.hpp"
#include "lexer.hpp"
#include <string>
#include <vector>


int main()
{
    std::string input = "5 - 3";
    Parser parses;
    std::vector<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;

    parses.parse();

    for (auto i : parses.all_operationnodes){
        std::cout << i.evaluate() << "\n";
    }
}
