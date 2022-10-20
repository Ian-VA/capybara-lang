#include "parser.hpp"
#include "lexer.hpp"
#include <deque>
// todo : a lot
astnode parserclass::parseInteger(Token tok)
{
    return integerliteral {tok};
}

astnode parserclass::parseVariable()
{

}

astnode parserclass::parseOperation()
{

}


int main()
{
    parserclass parses;
    std::deque<Token> alltokens = build_all();
    parses.all_tokens = alltokens;
    parses.index = 0;

    for (int i = 0; i < alltokens.size(); i++){

    }
}
