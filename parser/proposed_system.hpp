#ifndef PARSERFUNCTIONS_HPP
#define PARSERFUNCTIONS_HPP

#include "classes.hpp"

struct Parser 
{
    std::deque<Token> all_tokens;
    int index = 0;
    std::vector<int> values;
    std::vector<std::string> identifiers;

    Token get_token()
    {
        if (index > all_tokens.size()){
            return Token {type::ENDINPUT, "EOF"};
        }
        return all_tokens[index];
    }

    Token peek_token()
    {
        if(index + 1 > all_tokens.size()){
            return Token {type::ENDINPUT, "EOF"};
        }
        return all_tokens[index + 1];
    }

    void eat(){
        all_tokens.pop_front();
    }

    int parseInt()
    {

        if (get_token().types == type::VAR){
            for (int i = 0; i < 3; i++)
            {
                eat();

                if (get_token().types == type::IDENTIFIER){
                    std::string identifier = get_token().value;
                    identifiers.push_back(identifier); 
                    values.push_back(parseSum());
                }
            }
        }
        

        int value1 = parseSum();
        
        return value1;
    }

    int parseSum()
    {
        int pr1 = parseProduct();

        while (get_token().types == type::PLUS){
            eat();
            int pr2 = parseProduct();
            pr1 = pr1 + pr2;
        }

        return pr1;
    }

    int parseProduct()
    {
        int fac1 = parseFactor();
        
        while (get_token().types == type::TIMES){
            eat();
            int fac2 = parseFactor();
            fac1 *= fac2;
        }

        return fac1;
    }

    int parseFactor()
    {
        if (get_token().types == type::NUM){
            int data = std::stoi(get_token().value);
            eat();
            return data;

        } else if (get_token().types == type::LPAREN){
            eat(); // consume (
            int sum = parseSum();
            
            if (get_token().types != type::RPAREN){
                std::cout << "Expected RPAREN, got " << get_token().value << " instead" << "\n";
                return NULL;
            }

            return sum;

        } else if (get_token().types == type::IDENTIFIER){
            return NULL;
        }
    }

};



#endif
