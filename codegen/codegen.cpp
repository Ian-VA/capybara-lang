#include <cctype>
#include <vector>
#include <string>
#include "parser.hpp"
#include "lexer.hpp"
#include <fstream>
#include "parserdefinitions.hpp"

std::ofstream ostream;
static std::vector<std::string> namedvalues; // yes, global variables are bad
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

void write(std::string input, bool end) {
    ostream << input;

    if (end) {
        ostream << ";";
    }
}

void startCodegen() {
    ostream.open("cap.c");
    write("int main(){\n", false);
}

void endCodegen() {
    write("\n}", false);
    ostream.close();
}

void integerliteral::codegen() {
    write(this->get_value(), false);
}

void variabledeclaration::codegen() {

    if(this->getvariabletype() == "string"){
        std::string out = "char [";
        out.push_back(this->get_value().size());
        write(out + "]" + this->get_identifier() + "=" + this->get_value(), true);
    } else {
        write(this->getvariabletype() + " " + this->get_identifier() + "=" + this->get_value(), true);
    }

}

void callvariable::codegen() {
    if (std::find(namedvalues.begin(), namedvalues.end(), this->get_identifier()) != namedvalues.end()) {
        codegenerror {m_line, "Unknown variable referenced", ""};
    } else {
        write(this->get_identifier(), false);
    }
}

void astnode::codegen() {
    if (isNumber(this->get_value())) {
        integerliteral integer(this->get_value());
        integer.codegen();
    } else {
        callvariable var(this->get_value());
        var.codegen();
    }
}

void binaryoperation::codegen() {
    this->left->codegen(); write(this->operation, false); this->right->codegen();
}


int main()
{
    parserclass parses;
    std::string input;
    getline(std::cin, input);
    std::deque<Token> alltokens = build_all(input); 
    parses.all_tokens = alltokens;
        
    parses.index = 0;
    std::unique_ptr<variabledeclaration> bin = std::move(parses.parseVariable());
    startCodegen();
    bin->codegen();
    endCodegen();
}
