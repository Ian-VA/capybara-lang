#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "parser.hpp"

class codegenerror
{
    public:
        codegenerror(int m_line, std::string error, std::string note){
            std::cout << "Error encountered at line " << m_line << ": " << error << "\n";
            if (!note.empty()){
                std::cout << "Note: " << note << "\n";
            }
            std::cout << "Aborting.." << "\n";
            exit(1);
        }
};

class Visitor
{
    public:
        int m_line;
        static LLVMContext llvmcontext;
        static IRBuilder<> builder(LLVMContext);
        static std::unique_ptr<Module> llvmmodule;
        static std::map<std::string, Value*> namedvalues;

        
        virtual Value visit (const std::unique_ptr<integerliteral> node);
        virtual Value visit (const std::unique_ptr<binaryoperation> node);
        virtual Value visit (const std::unique_ptr<variabledeclaration> node);
        virtual Value visit (const std::unique_ptr<callvariable> node);
        virtual Value visit (const std::unique_ptr<callfunctionnode> node);
        virtual Value visit (const std::unique_ptr<protonode> node);
        virtual Value visit (const std::unique_ptr<funcdefinitionnode> node);


};

#endif
