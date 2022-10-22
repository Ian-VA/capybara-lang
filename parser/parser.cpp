#include "parser.hpp"
#include "lexer.hpp"
#include <deque>
// todo : a lot
std::unique_ptr<integerliteral> parserclass::parseInteger()
{
    if (get_token().types != type::NUM){
        std::cout << "Expected integer, got " << get_token().value << "\n";
        return NULL;
    }
    
    eat();
    
    return std::make_unique<integerliteral>(get_token());
}

std::unique_ptr<astnode> parserclass::parseVariable()
{
  if (get_token().types == type::VAR){
      eat();
      
      if (get_token().types != type::IDENTIFIER){
            std::cout << "Expected identifier after VAR, got " << get_token().value << "\n";
            return NULL;
      } else {
            std::string identifier = get_token().value;
            eat();
            eat();
            std::string value = get_token().value;
            eat();
            return std::make_unique<variabledeclaration>(std::string("placeholder"), identifier, value);
      }
  }  
}

std::unique_ptr<astnode> parserclass::parseOperation()
{
    if (get_token().types == type::NUM){
        std::unique_ptr<integerliteral> right = std::move(parseInteger());
        
        if (get_token().types != (type::PLUS || type::SUBTRACT || type::TIMES || type::DIVISION)){
            std::cout << "Expected operation, got " << get_token().value << "\n";
            return NULL;
        } else {
            std::string operation = get_token().value;
            eat();
            
            if (get_token().types != type::NUM){
                std::cout << "Expected integer after: " << operation << " got " << get_token().value << " instead" << "\n";
                return NULL;
            } else {
                std::unique_ptr<integerliteral> left = std::move(parseInteger());
                
                return std::make_unique<binaryoperation>(operation, std::move(right), std::move(left));
            }
          
        }
    } 
}

std::unique_ptr<astnode> parseIfStatement()
{
    
}


int main()
{
    parserclass parses;
    std::string input = "var i = 3";
    std::deque<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;

    for (int i = 0; i < alltokens.size(); i++){
        
    }
}
