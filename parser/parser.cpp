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
    return *integerliteral {get_token()};
}

astnode* parserclass::parseVariable()
{
  if (tok.type == type::VAR){
      eat();
      
      if (get_token().type != type::IDENTIFIER){
          std::cout << "Expected identifier after VAR, got " << get_token().value << "\n";
          return NULL;
      } else {
          std::string identifier = get_token().value;
      }
      
      eat();
      std::string value = get_token().value;
      
      
      
      return *variable {identifier, value};
      
  }  
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
