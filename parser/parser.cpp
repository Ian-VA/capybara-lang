#include "parser.hpp"
#include "lexer.hpp"
#include <deque>

std::unique_ptr<astnode> parserclass::parseInteger()
{
    if (get_token().types != type::NUM){
        error {curr_line, "Expected integer, found unknown token", ""};
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
        error {curr_line, "Expected identifier, found unknown token", ""};
      } else {
            std::string identifier = get_token().value;
            eat(); eat();
            std::string value = get_token().value;

            if (value == "EOF") {
                error {curr_line, "Expected value, found EOF", ""};
            }
            eat();
            return std::make_unique<variabledeclaration>(std::string("placeholder"), value, identifier);
      }
  } else if (get_token().types == type::IDENTIFIER) { // we then assume this was called by parseidentifiercall, and is a call to a variable
      std::string identifier = get_token().value;
      eat(); eat();

      return std::make_unique<callvariable>(identifier);
  }

}

std::unique_ptr<astnode> parserclass::parseOperation()
{
    if (get_token().types == type::NUM){
        std::unique_ptr<astnode> right = std::move(parseInteger()); // eat first num
        
        if (get_token().types != type::PLUS && get_token().types != type::SUBTRACT && get_token().types != type::TIMES && get_token().types != type::DIVISION){
            error {curr_line, "Expected operation, found unknown token", ""};
        } else {
            std::string operation = get_token().value;
            eat(); // eat operation
            
            if (get_token().types != type::NUM){
                error {curr_line, "Expected integer after operation, found unknown token", ""};
                return nullptr;
            } else {
                std::unique_ptr<astnode> left = std::move(parseInteger()); // eat second num
                
                return std::make_unique<binaryoperation>(operation, std::move(right), std::move(left));
            }
          
        }
    } 
}


std::unique_ptr<astnode> parserclass::parseGroupedExpr()
{
    eat(); // (
    auto V = parseExpression();

    if (!V || get_token().value != ")") {
        return nullptr;
    }

    eat();
    return V;
}

std::unique_ptr<astnode> parserclass::parseIdentifierCall()
{
    if (peek_token().value != "(") {
        return std::move(parseVariable());
    } 

    std::string identifiername = get_token().value;
    eat(); eat();
    
    std::vector<std::unique_ptr<astnode>> args;

    while (get_token().value != ")") {
        if (auto arg = parseExpression()){
            args.push_back(std::move(arg)); // push back arg

            if (get_token().value != ",") {
                error {curr_line, "Expected ',' or ')' in function argument list", ""};
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

std::unique_ptr<protonode> parserclass::parsePrototype()
{
    if (get_token().types != type::IDENTIFIER) {
        error {curr_line, "Expected identifier in function prototype", ""};
    } else {
        std::string name = get_token().value;
        eat();
        if (get_token().value != "("){
            error {curr_line, "Expected '(' in function prototype", ""};
        } else {
            eat();
            std::vector<std::string> args;
            while (get_token().types == type::IDENTIFIER)
            {
                args.push_back(get_token().value);
                eat();
                if (get_token().value != "," && peek_token().types == type::IDENTIFIER && get_token().value != ")") {
                    error {curr_line, "Expected ',' seperating function arguments", ""};
                } else if (get_token().value == ","){
                    eat();
                }
            }
            if (get_token().value != ")"){
                error {curr_line, "Expected ')' in function prototype", ""};
            } else {
                eat();
                return std::make_unique<protonode>(name, args);
            }
        }
    }
}

std::unique_ptr<funcdefinitionnode> parserclass::parseDefinition()
{
    eat();
    auto proto = parsePrototype();

    if (auto e = parseExpression()) {
        return std::make_unique<funcdefinitionnode>(std::move(proto), std::move(e));
    } else {
        return nullptr;
    }
}

std::unique_ptr<funcdefinitionnode> parserclass::parseTopLevelExpr()
{
    if (auto e = parseExpression()){
        auto proto = std::make_unique<protonode>("", std::vector<std::string>());
        return std::make_unique<funcdefinitionnode>(std::move(proto), std::move(e));
    }
    return nullptr;
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
        default:
            std::cout << "Unknown token when expecting expression" << "\n";
            return nullptr;
    }
}


int main()
{
    parserclass parses;
    std::string input;
    getline(std::cin, input);
    std::deque<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;
    std::vector<std::unique_ptr<astnode>> ast;

    while (true) {
        switch (parses.get_token().types) {
            case type::FUNC:
                parses.parseDefinition();
                std::cout << "parsed func definition" << "\n";
                break;
            case type::NUM:
                parses.parseInteger();
                std::cout << "parsed integer" << "\n";
                break;
            case type::VAR:
                parses.parseVariable();
                std::cout << "parsed variable" << "\n";
                break;
        }
    }

}
