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

void write(std::string input) {
    ostream.open("cap.c");
    ostream << input;
    ostream.close();
}

void startCodegen() {
    write("int main()\n{\nreturn0\n}");
}

void integerliteral::codegen() {
    write(this->get_value());
}

void callvariable::codegen() {
    if (std::find(namedvalues.begin(), namedvalues.end(), this->get_identifier()) != namedvalues.end()) {
        codegenerror {m_line, "Unknown variable referenced", ""};
    } else {
        write(this->get_identifier());
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
    this->left->codegen(); write(this->operation); this->right->codegen(); write(";");
}


int main()
{
    parserclass parses;
    std::string input;
    getline(std::cin, input);
    std::deque<Token> alltokens = build_all(input);
    parses.all_tokens = alltokens;
    parses.index = 0;
    std::unique_ptr<astnode> bin = std::move(parses.parseInteger());

    bin->codegen();
}
