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

void encounteredMain() {
    write("int main() {\n", false);
}

void encounteredEnd() {
    write("\n}", false);
}

const std::string& integerliteral::codegen() {
    write(this->get_value(), false);
}

const std::string& callfunctionnode::codegen(){
    write(this->callee + "(", false);
    
    for (int i = 0; i < getArgs().size(); i++) {
        std::shared_ptr<astnode> j = getArgs()[i];
        write(j->codegen() + ",", false);
        
        if (j == (this->getArgs())[getArgs().size() - 2]) {
            write(j->codegen() + ")", false);
        }
    } 
}

const std::string& variabledeclaration::codegen() {

    if(this->getvariabletype() == "string"){
        std::string out = "char [";
        out.push_back(this->get_value().size());
        write(out + "]" + this->get_identifier() + "=" + this->get_value(), true);
    } else {
        write(this->getvariabletype() + " " + this->get_identifier() + "=" + this->get_value(), true);
    }

}

const std::string& protonode::codegen(){
    write(this->getName() + "(", false);
    
    for (auto i : this->getArgs()) {        
        if (i == this->getArgs()[getArgs().size() - 1]) {
            write(i + ")", false);
            break;
        } else {
            write(i + ",", false);
        }
    }

}

const std::string& funcdefinitionnode::codegen() { 
    this->proto->codegen(); write("{\n", false);
    write("return ", false);
    this->body->codegen();
    write(";\n}", false);
}

const std::string& callvariable::codegen() {
    if (std::find(namedvalues.begin(), namedvalues.end(), this->get_value()) != namedvalues.end()) {
        codegenerror {m_line, "Unknown variable referenced", ""};
    } else {
        write(this->get_value(), false);
    }
}

const std::string& astnode::codegen() {
    write(this->get_value(), false);
}

const std::string& binaryoperation::codegen() {
    write(this->get_value(), false);
}

int main()
{
    parserclass parses;
    std::string input;
    getline(std::cin, input);
    std::deque<Token> alltokens = build_all(input); 
    parses.all_tokens = alltokens;
    
    parses.index = 0;
    std::shared_ptr<variabledeclaration> bin = parses.parseVariable();
    ostream.open("cap.c");
    bin->codegen();
    ostream.close();
}
