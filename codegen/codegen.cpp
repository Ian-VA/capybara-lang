#include "parser.hpp"
#include <cctype>

static std::unique_ptr<LLVMContext> llvmcontext; // yes global variables bad
static std::unique_ptr<Module> llvmodule;
static std::unique_ptr<IRBuilder<>> builder;
static std::map<std::string, Value*> namedvalues;
int m_line = 0;


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



Value* astnode::codegen()
{
    if (isNumber(this->get_value())) {
        integerliteral integer(this->get_value());
        return integer.codegen();
    } else {
        callvariable var(this->get_value());
        return var.codegen();
    }
}


Value* integerliteral::codegen(){
    return ConstantFP::get(*llvmcontext, APFloat(std::stof(std::move(this->get_value()))));
}

Value* callvariable::codegen(){
    Value* V = namedvalues[this->get_identifier()];
    
    if (!V) {
        codegenerror {m_line, "Unknown variable referenced", ""};
    } else {
        return V;
    }
}

Value* binaryoperation::codegen() {
    Value *L = this->left->codegen();
    Value *R = this->right->codegen();

    switch (this->get_operation())
    {
        case '+':
            return builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return builder->CreateFSub(L, R, "subtmp");
        case '*':
            return builder->CreateFMul(L, R, "multmp");
        case '<':
            L = builder->CreateFCmpULT(L, R, "cmptmp");
            return builder->CreateUIToFP(L, Type::getDoubleTy(*llvmcontext), "booltmp");
        default:
            codegenerror {m_line, "Invalid binary operator", "This probably happened because I haven't added all the common operators to this compiler yet"};
    }
}


int main()
{

}
