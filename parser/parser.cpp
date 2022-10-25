#include "parser.hpp"
#include "lexer.hpp"
#include <deque>

std::unique_ptr<astnode> parserclass::parseInteger()
{
    if (get_token().types != type::NUM){
        std::cout << "Expected integer, got " << get_token().value << "\n";
        return NULL;
    }

    auto result = std::make_unique<integerliteral>(get_token());
    eat();
    return std::move(result);
}


std::unique_ptr<astnode> parserclass::parseVariable()
{
  if (get_token().types == type::VAR){
      eat(); // eat var
      
      if (get_token().types != type::IDENTIFIER){
            std::cout << "Expected identifier after VAR, got " << get_token().value << "\n";
            return nullptr;
      } else {
            std::string identifier = get_token().value;
            eat(); eat();
            std::string value = get_token().value;
            eat();
            return std::make_unique<variabledeclaration>(std::string("placeholder"), value, identifier);
      }
  } else if (get_token().types == type::IDENTIFIER) { // we then assume this was called by parseidentifiercall
      std::string identifier = get_token().value;
      eat(); eat();
      std::string value = get_token().value;
      eat();

      return std::make_unique<variabledeclaration>(std::string("placeholder"), value, identifier);
  }

}

std::unique_ptr<astnode> parserclass::parseOperation()
{
    if (get_token().types == type::NUM){
        std::unique_ptr<astnode> right = std::move(parseInteger());
        
        if (get_token().types != (type::PLUS || type::SUBTRACT || type::TIMES || type::DIVISION)){
            std::cout << "Expected operation, got " << get_token().value << "\n";
            return nullptr;
        } else {
            std::string operation = get_token().value;
            eat();
            
            if (get_token().types != type::NUM){
                std::cout << "Expected integer after: " << operation << " got " << get_token().value << " instead" << "\n";
                return nullptr;
            } else {
                std::unique_ptr<astnode> left = std::move(parseInteger());
                
                return std::make_unique<binaryoperation>(operation, std::move(right), std::move(left));
            }
          
        }
    } 
}


std::unique_ptr<astnode> parserclass::parseGroupedExpr()
{
    eat();
    auto V = parseExpression();

    if (!V || get_token().value != ")") {
        return nullptr;
    }

    eat();
    return V;
}

std::unique_ptr<astnode> parserclass::parseIdentifierCall()
{
    std::string identifiername = get_token().value;
    eat();

    if (get_token().value != "(") {
        return std::move(parseVariable());
    } else {
        eat(); // eat (
        std::vector<std::unique_ptr<astnode>> args;

        while (get_token().value != ")") {
            if (auto arg = parseExpression()){
                args.push_back(std::move(arg)); // push back arg

                if (get_token().value != ",") {
                    std::cout << "Expected ') or ',' token in argument list" << "\n";
                    return nullptr;
                } else {
                    eat(); // eat ,
                }
            } else {
                return nullptr;
            }
        }

        eat(); // eat ')'
        return std::make_unique<callfunctionnode>(identifiername, std::move(args));
    }
}


std::unique_ptr<astnode> parserclass::primaryParserLoop()
{
    switch (get_token().types)
    {
        case type::IDENTIFIER:
            return std::move(parseIdentifierCall());
        case type::NUM:
            return std::move(parseInteger());
        case type::LPAREN:
            return std::move(parseGroupedExpr());
    }
}


std::unique_ptr<astnode> parserclass::parseStatement()
{
    switch (get_token().types)
    {

    }
}



int main()
{
    parserclass parses;
    std::string input = "function (arg1)";
    std::deque<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;
    std::vector<std::unique_ptr<astnode>> ast;
    std::unique_ptr<astnode> call = std::move(parses.parseIdentifierCall());


    for (int i = 0; i < alltokens.size(); i++){
        switch (alltokens[i].types)
        {
            
        }
    }
}
