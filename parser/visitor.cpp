#include "parser.hpp"
#include "Visitor.hpp"

void integerliteral::accept(Visitor& visitor) {visitor.visit(std::move(std::make_unique<integerliteral>(*this)));}

void variabledeclaration::accept(Visitor& visitor) {visitor.visit(std::move(std::make_unique<variabledeclaration>(*this)));}

void binaryoperation::accept(Visitor& visitor) {visitor.visit(std::move(std::make_unique<binaryoperation>(*this)));}
