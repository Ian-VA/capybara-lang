#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "parser.hpp"

class Visitor
{
    public:
        virtual bool visit (const std::unique_ptr<integerliteral> node);
        virtual bool visit (const std::unique_ptr<binaryoperation> node);
        virtual bool visit (const std::unique_ptr<variabledeclaration> node);
};



#endif
