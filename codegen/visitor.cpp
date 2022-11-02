#include "parser.hpp"
#include "Visitor.hpp"

Value integerliteral::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<integerliteral>(*this)));}

Value variabledeclaration::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<variabledeclaration>(*this)));}

Value binaryoperation::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<binaryoperation>(*this)));}

Value callvariable::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<callvariable>(*this)));}

Value callfunctionnode::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<callfunctionnode>(*this)));}

Value protonode::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<protonode>(*this)));}

Value funcdefinitionnode::accept(Visitor& visitor) {return visitor.visit(std::move(std::make_unique<funcdefinitionnode>(*this)));}

Value Visitor::visit(const std::unique_ptr<integerliteral> node){
    return ConstantFP::get(llvmcontext, APFloat(std::stof(std::move(node->get_value()))));
}

Value Visitor::visit(const std::unique_ptr<variabledeclaration> node){
    Value V = namedvalues[node->get_identifier()];
    !V ? codegenerror {m_line, "Unknown variable name", ""} : return V;
}

Value Visitor::visit(const std::unique_ptr<binaryoperation> node) {
    Value *L = node->left->accept(*this);
    Value *R = node->right->accept(*this);

    switch (node->get_operation())
    {
        case '+':
            return builder.CreateFAdd(L, R, "addtmp");
        case '-':
            return builder.CreateFSub(L, R, "subtmp");
        case '*':
            return builder.CreateFMul(L, R, "multmp");
        case '<':
            L = builder.CreateFCmpULT(L, R, "cmptmp");
            return Builder.CreateUIToFP(L, Type::getDoubleTy(llvmcontext), "booltmp");
        default:
            codegenerror {m_line, "Invalid binary operator", "This probably happened because I haven't added all the common operators to this compiler yet"};
    }
}

int main()
{

}

