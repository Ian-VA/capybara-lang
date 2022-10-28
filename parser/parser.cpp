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
    eat(); // eats integer
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
            eat(); eat(); // eats identifier and =
            std::string value = get_token().value;
            eat(); 
            return std::make_unique<variabledeclaration>(std::string("type"), value, identifier);
      }
  } else if (get_token().types == type::IDENTIFIER) { // we then assume this was called by parseidentifiercall, and is a call to a variable
  
      std::string identifier = get_token().value;
      eat(); eat(); // eats 

      return std::make_unique<callvariable>(identifier);
  }

}

std::unique_ptr<astnode> parserclass::parseOperation()
{
    if (get_token().types == type::NUM){
        std::unique_ptr<astnode> right = std::move(parseInteger()); // eat first num
        
        if (get_token().types != type::PLUS && get_token().types != type::SUBTRACT && get_token().types != type::TIMES && get_token().types != type::DIVISION){
            std::cout << "Expected operation, got " << get_token().value << "\n";
            return nullptr;
        } else {
            std::string operation = get_token().value;
            eat(); // eat operation
            
            if (get_token().types != type::NUM){
                std::cout << "Expected integer after: " << operation << " got " << get_token().value << " instead" << "\n";
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
    auto V = parseExpression(); // parses anything in the parentheses

    if (!V || get_token().value != ")") {
        return nullptr;
    }

    eat();
    return V;
}

std::unique_ptr<astnode> parserclass::parseIdentifierCall()
{
    if (peek_token().value != "(") { // assumes this is a variable
        return std::move(parseVariable());
    } 

    std::string identifiername = get_token().value;
    eat(); eat(); // eats identifier, then (
    
    std::vector<std::unique_ptr<astnode>> args;

    while (get_token().value != ")") {
        if (auto arg = parseExpression()){ 
            args.push_back(std::move(arg)); // push back arg

            if (get_token().value != ",") {
                std::cout << "Expected ')' or ',' token in argument list" << "\n";
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

std::unique_ptr<protonode> parserclass::parsePrototype()
{
    if (get_token().types != type::IDENTIFIER) {
        std::cout << "Expected identifier in function protoype" << "\n";
        return nullptr;
    } else {
        std::string name = get_token().value;
        eat(); // eat identifier
        if (get_token().value != "("){
            std::cout << "Expected '(' in function prototype" << "\n";
            return nullptr;
        } else {
            eat(); // eat (
            std::vector<std::string> args;
            while (get_token().types == type::IDENTIFIER) // until hit ) token
            {
                args.push_back(get_token().value);
                eat();
                if (get_token().value != "," && peek_token().types == type::IDENTIFIER) {
                    std::cout << "Expected ',' seperating function " << name << " args" << "\n";
                    return nullptr;
                } else if (get_token().value == ","){
                    eat();
                }
            }
            if (get_token().value != ")"){
                std::cout << "Expected ')' in function prototype" << "\n";
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
    auto proto = parsePrototype(); // parses the protope i.e func(arg1, arg2)

    if (auto e = parseExpression()) { // parses full function body
        return std::make_unique<funcdefinitionnode>(std::move(proto), std::move(e));
    } else {
        return nullptr;
    }
}

std::unique_ptr<funcdefinitionnode> parserclass::parseTopLevelExpr()
{
    if (auto e = parseExpression()){ // allows user to type in arbitrary top level expressions
        auto proto = std::make_unique<protonode>("", std::vector<std::string>());
        return std::make_unique<funcdefinitionnode>(std::move(proto), std::move(e));
    }
}


std::unique_ptr<astnode> parserclass::primaryParserLoop() // parses primary expressions, i.e identifiers, integers
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


int main()
{
    parserclass parses;
    std::string input;
    std::cin >> input;
    std::deque<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;
    std::vector<std::unique_ptr<astnode>> ast;

    while (true) {
        fprintf(stderr, "ready> ");
        switch (parses.get_token().types) {
            case type::IDENTIFIER:
                parses.parseDefinition();
                break;
            default:
                parses.parseTopLevelExpr();
                break;
        }
    }

}
