#ifndef PARSER_DEFINITION
#define PARSER_DEFINITION

#include "parser.hpp"
#include "lexer.hpp"
#include <deque>

std::shared_ptr<ifstatement> parserclass::parseIfStatement()
{
    if (get_token().types != type::IF) {
        error {curr_line, "Expected if keyword, found unknown token", ""};
    }

    auto boolean = parseExpression();

    if (auto body = parseExpression()) {
        return std::make_shared<ifstatement>((boolean), (body));
    } else {
        error {curr_line, "Expected body for if statement", ""};
    }
    
}


std::shared_ptr<integerliteral> parserclass::parseInteger()
{
    if (get_token().types != type::NUM){
        error {curr_line, "Expected integer, found unknown token", ""};
    }

    auto result = std::make_shared<integerliteral>(get_token());
    eat();
    return (result);
}


std::shared_ptr<variabledeclaration> parserclass::parseVariable()
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
        return std::make_shared<variabledeclaration>(std::string("placeholder"), value, identifier);
      }
  }
}

std::shared_ptr<callvariable> parserclass::parseCallVariable()
{
    std::string identifier = get_token().value;
    eat(); 
    
    return std::make_shared<callvariable>(identifier);
}

std::shared_ptr<binaryoperation> parserclass::parseOperation()
{
    if (get_token().types == type::NUM){
        std::shared_ptr<astnode> right = (parseInteger()); // eat first num
        
        if (get_token().types != type::PLUS && get_token().types != type::SUBTRACT && get_token().types != type::TIMES && get_token().types != type::DIVISION){
            error {curr_line, "Expected operation, found unknown token", ""};
        } else {
            std::string operation = get_token().value;
            eat(); // eat operation
            
            if (get_token().types != type::NUM){
                error {curr_line, "Expected integer after operation, found unknown token", ""};
                return nullptr;
            } else {
                std::shared_ptr<astnode> left = (parseInteger()); // eat second num
                
                return std::make_shared<binaryoperation>(operation, (right), (left));
            }
          
        }
    } 
}

std::shared_ptr<astnode> parserclass::parseGroupedExpr()
{
    eat(); // (
    auto V = parseExpression();

    if (!V || get_token().value != ")") {
        return nullptr;
    }

    eat();
    return V;
}

std::shared_ptr<callfunctionnode> parserclass::parseIdentifierCall()
{
    std::string identifiername = get_token().value;
    eat(); eat();
    
    std::vector<std::shared_ptr<astnode>> args;

    while (get_token().value != ")") {
        if (auto arg = parseExpression()){
            args.push_back((arg)); // push back arg

            if (get_token().value != ",") {
                error {curr_line, "Expected ',' or ')' in function argument list", ""};
            } else {
                eat(); // eat ,
            }
        } else {
            error {curr_line, "Argument error in function " + identifiername, "Are you sure your arguments are variables?"};
        }
    }

    eat(); // eat ')'
    return std::make_shared<callfunctionnode>(identifiername, (args));

}

std::shared_ptr<protonode> parserclass::parsePrototype()
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
                return std::make_shared<protonode>(name, args);
            }
        }
    }
}

std::shared_ptr<funcdefinitionnode> parserclass::parseDefinition()
{
    eat();
    std::string typeinf = "";
    auto proto = parsePrototype();

    if (get_token().types != type::BEGIN) {
        error {curr_line, "No begin statement for function", "Possible prototyping error"};
    } else {
        eat();

         for (int i = 1; i < all_tokens.size(); i++) {
            if (all_tokens[i].types == type::RETURN) {
                std::cout << "YAY FOUND RETURN";
                if (all_tokens[i+1].types != type::ENDINPUT){
                    switch(all_tokens[i+1].types){
                        case type::IDENTIFIER:
                            switch(all_tokens[i+2].types){
                                case type::PLUS:
                                case type::SUBTRACT:
                                case type::TIMES:
                                case type::DIVISION:
                                    typeinf = "int";
                                case type::COMPARE:
                                case type::GREATERTHAN:
                                case type::GREATERTHANOREQUALTO:
                                case type::NOT:
                                case type::LESSTHAN:
                                case type::LESSTHANOREQUALTO:
                                    typeinf = "bool";
                                default:
                                    if (std::isalpha(all_tokens[i+1].value[0])) {
                                        if (std::isalpha(all_tokens[i+1].value[1])){
                                            typeinf = "char[]";
                                        } else {
                                            typeinf = "char";
                                        }
                                    } else if (isNumber(all_tokens[i+1].value)) {
                                        typeinf = "int";
                                    }
                            }
                        
                    }
                } else {
                    error {curr_line, "Return error in function " + proto->getName(), ""};
                }
            }
        }
 
        if (typeinf.empty()) typeinf = "void";
        
        if (auto e = parseExpression()) {
            return std::make_shared<funcdefinitionnode>((proto), (e));
        } else {
            error {curr_line, "No body found for function definition", ""};
        }
    }
}

std::shared_ptr<funcdefinitionnode> parserclass::parseTopLevelExpr()
{
    if (auto e = parseExpression()){
        auto proto = std::make_shared<protonode>("", std::vector<std::string>());
        return std::make_shared<funcdefinitionnode>((proto), (e));
    }
    return nullptr;
}


std::shared_ptr<astnode> parserclass::primaryParserLoop()
{
    switch (get_token().types)
    {
        case type::IDENTIFIER:
            if (peek_token().value != "(") {
                if (get_token().value != "local") {
                    return parseCallVariable();
                } else {
                    return parseVariable();
                }
            } else {
                return parseIdentifierCall();
            }
            std::cout << "parsed identifier" << "\n";
        case type::NUM:
            if (peek_token().value == "+" || peek_token().value == "-" || peek_token().value == "/" || peek_token().value == "*") {
                std::cout << "parsed binop" << "\n";
                return parseOperation();
            }
            std::cout << "parsed integer" << "\n";
            return parseInteger();
        case type::LPAREN:
            return parseGroupedExpr();
        case type::MAIN:
            encountered_main = true;
        default:
            std::cout << "Unknown token when expecting expression" << "\n";
            return nullptr;
    }
}

#endif
