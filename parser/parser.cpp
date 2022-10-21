#include "parser.hpp"
#include "lexer.hpp"
#include <deque>
// todo : a lot
astnode* parserclass::parseInteger()
{
    if (get_token().type != type::INTEGER){
        std::cout << "Expected integer, got " << get_token().value << "\n";
        return NULL;
    }
    
    eat();
    
    return integerliteral {get_token()};
}

astnode* parserclass::parseVariable()
{
  if (get_token().type == type::VAR){
      eat();
      
      if (get_token().type != type::IDENTIFIER){
          std::cout << "Expected identifier after VAR, got " << get_token().value << "\n";
          return NULL;
      } else {
          std::string identifier = get_token().value;
      }
      
      eat();
      std::string value = get_token().value;
      
      
      
      return variable {identifier, value};
      
  }  
}

astnode* parserclass::parseOperation()
{
    if (get_token().type == type::INTEGER){
        integerliteral *right = parseInteger();
        
        if (get_token().type != (type::PLUS || type::SUBTRACT || type::TIMES || type::DIVISION)){
            std::cout << "Expected operation, got " << get_token().value << "\n";
            return NULL;
        } else {
            std::string operation = get_token().value;
            eat();
            
            if (get_token().type != type::INTEGER){
                std::cout << "Expected integer after: " << operation << " got " << get_token().value << " instead" << "\n";
                return NULL;
            } else {
                integerliteral *left = parseInteger();
                
                return binaryoperation {operation, right, left};
            }
          
        }
    } 
}

astnode* parseIfStatement()
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
