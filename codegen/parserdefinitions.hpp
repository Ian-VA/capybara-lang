// copyright Ian A. 2022, all rights reserved

#ifndef PARSER_DEFINITION
#define PARSER_DEFINITION

#include "parser.hpp"
#include "lexer.hpp"
#include <deque>

std::shared_ptr<astnode> parserclass::parseBoolean()
{
    if (get_token().types != type::NOTEQUAL && get_token().types != type::EQUALSEQUALS) {
        error {curr_line, "expected boolean expression, found unknown token", ""};
    }

    std::string val = get_token().value;
    eat();
    return std::make_shared<boolean>(val);

}

std::shared_ptr<ifstatement> parserclass::parseIfStatement()
{
    std::vector<std::shared_ptr<astnode>> body;
    if (get_token().types != type::IF) {
        error {curr_line, "Expected if keyword, found unknown token", ""};
    }

    eat(); 
    auto boolean = parseExpression();
    eat(); 
    std::cout << boolean->get_value() << "\n";
    while (true)
    {
        if (auto e = parseExpression()) {
            if (e->get_value() != "end"){
                body.push_back(e);
            } else {
                body.push_back(e);
                break;
            }
        }

        if (body.empty()) {
            break;
        }
    }

    return std::make_shared<ifstatement>(boolean, body);
    
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
    std::vector<std::string> keywords;

    if (get_token().types == type::REFERENCE || get_token().types == type::POINTER) {
        keywords.push_back(get_token().value);
        eat();
    }

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
        return std::make_shared<variabledeclaration>(std::string("placeholder"), value, identifier, keywords);
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
        std::shared_ptr<astnode> right = parseInteger(); // eat first num
        
        if (get_token().types != type::PLUS && get_token().types != type::SUBTRACT && get_token().types != type::TIMES && get_token().types != type::DIVISION){
            error {curr_line, "Expected operation, found unknown token", ""};
        } else {
            std::string operation = get_token().value;
            eat(); // eat operation
            
            if (get_token().types != type::NUM){
                error {curr_line, "Expected integer after operation, found unknown token", ""};
                return nullptr;
            } else {
                std::shared_ptr<astnode> left = parseInteger(); // eat second num
                
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
        error {curr_line, "No closing parentheses in groupedexpr", ""};
    }

    eat();
    return std::make_shared<groupedexpre>(V->get_value());
}

std::shared_ptr<callfunctionnode> parserclass::parseIdentifierCall()
{
    std::string identifiername = get_token().value;
    eat(); eat();
    
    std::vector<std::shared_ptr<astnode>> args;

    while (get_token().value != ")") {
        if (auto arg = parseExpression()){
            args.push_back((arg)); // push back arg

            if (get_token().value != "," && get_token().value != ")") {
                error {curr_line, "Expected ',' or ')' in function argument list", ""};
            } else {
                if (get_token().value != ")") eat(); // eat ,
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
    if (get_token().types != type::IDENTIFIER && get_token().types != type::MAIN) {
        error {curr_line, "Expected identifier in function prototype", ""};
    } else {
        std::string name = get_token().value;
        eat();
        if (get_token().value != "("){
            error {curr_line, "Expected '(' in function prototype", ""};
        } else {
            eat();
            std::vector<std::string> args;
            std::vector<std::string> types;
            while (get_token().types == type::CHAR || get_token().types == type::STRING || get_token().types == type::INTEGER || get_token().types == type::BOOLEAN)
            {
                types.push_back(get_token().value);
                eat();
                args.push_back(get_token().value);
                eat();

                if (peek_token().types == type::CHAR || peek_token().types == type::STRING || peek_token().types == type::INTEGER || peek_token().types == type::BOOLEAN) {
                    if (get_token().value != ")") {
                        if (get_token().value != ","){
                            error {curr_line, "Expected ',' seperating function arguments", ""};
                        } else {
                            eat();
                        }
                    } else {
                        break;
                    }
                } 
            }
            if (get_token().value != ")"){
                error {curr_line, "Expected ')' in function prototype", ""};
            } else {
                eat();
                return std::make_shared<protonode>(name, args, types);
            }
        }
    }
}

std::shared_ptr<funcdefinitionnode> parserclass::parseDefinition()
{
    std::string typeinf = "";
    auto proto = parsePrototype();
    std::vector<std::shared_ptr<astnode>> body;

    if (get_token().types != type::BEGIN) {
        error {curr_line, "No begin statement for function", "Possible prototyping error"};
    } else {
        eat();

         for (int i = 0; i < all_tokens.size(); i++) {
            if (all_tokens[i].types == type::RETURN) {
                if (all_tokens[i+1].types != type::ENDINPUT){
                    switch(all_tokens[i+1].types){
                        case type::IDENTIFIER:
                            switch(all_tokens[i+2].types){
                                case type::PLUS:
                                case type::SUBTRACT:
                                case type::TIMES:
                                case type::DIVISION:
                                    typeinf = "int";
                                    break;
                                case type::COMPARE:
                                case type::GREATERTHAN:
                                case type::GREATERTHANOREQUALTO:
                                case type::NOT:
                                case type::LESSTHAN:
                                case type::LESSTHANOREQUALTO:
                                    typeinf = "bool";
                                    break;
                                default:
                                    for (int j = 0; j < proto->getArgs().size(); j++) {
                                        if (all_tokens[i+1].value == proto->getArgs()[j]){
                                            typeinf = proto->getTypes()[j];
                                            break;
                                        }
                                    }

                                    if (!typeinf.empty()) {
                                        break;
                                    } else {
                                        if (std::isalpha(all_tokens[i+1].value[0])) {
                                            if (std::isalpha(all_tokens[i+1].value[1])){
                                                typeinf = "string";
                                            } else {
                                                typeinf = "char";
                                            }
                                        } else if (isNumber(all_tokens[i+1].value)) {
                                            typeinf = "int";
                                        }

                                    }
                            }
                        
                    }
                } else {
                    error {curr_line, "Return error in function " + proto->getName(), ""};
                }
            }
        }
 
        if (typeinf.empty()) typeinf = "void";

        while (true)
        {
            if (auto e = parseExpression()) {
                if (e->get_value() != "end"){
                    body.push_back(e);
                } else {
                    body.push_back(e);
                    break;
                }
            }


            if (body.empty()) {
                break;
            }
        }

        return std::make_shared<funcdefinitionnode>(proto, body, typeinf);            
    }
}

std::shared_ptr<funcdefinitionnode> parserclass::parseTopLevelExpr()
{
    std::vector<std::shared_ptr<astnode>> body;
    for (auto e = parseExpression(); e->get_value() != "end";) {
        body.push_back(e);
    }

    auto proto = std::make_shared<protonode>("", std::vector<std::string>(), std::vector<std::string>());
    return std::make_shared<funcdefinitionnode>((proto), (body), std::string("void"));
}


std::shared_ptr<astnode> parserclass::primaryParserLoop()
{
    switch (get_token().types)
    {
        case type::VAR:
            return parseVariable();
            break;
        case type::IF:
            return parseIfStatement();
            break;
        case type::IDENTIFIER:
            if (peek_token().value != "(") {
                return parseCallVariable();
            } else {
                if (all_tokens[index + 2].value == "int" || all_tokens[index + 2].value == "str" || all_tokens[index + 2].value == "char" || all_tokens[index + 2].value == "bool" || get_token().types == type::MAIN) {
                    return parseDefinition();
                }

                return parseIdentifierCall();
            }
            std::cout << "parsed identifier" << "\n";
            break;
        case type::EQUALSEQUALS:
        case type::NOTEQUAL:
            return parseBoolean();
            break;
        case type::RETURN:  // since callvariables are parsed in codegen by just their identifier, keywords work here too
        case type::END:
            return parseCallVariable();
            break;
        case type::NUM:
            std::cout << "parsed integer" << "\n";
            return parseInteger();
            break;
        case type::LPAREN:
            return parseGroupedExpr();
            break;
        default:
            break;
    
    }       

}

std::deque<std::shared_ptr<astnode>> parserclass::parseAll()
{
    std::deque<std::shared_ptr<astnode>> allnodes;

    while (true)
    {
        std::shared_ptr<astnode> data = parseDefinition();
        allnodes.push_back(data);

        if (get_token().value == "EOF"){
            break;
        }

    }

    return allnodes;
}

#endif