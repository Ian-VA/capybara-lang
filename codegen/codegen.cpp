#include <cctype>
#include <vector>
#include <string>
#include "parser.hpp"
#include "lexer.hpp"
#include <fstream>
#include "parserdefinitions.hpp"

std::ofstream ostream;
static std::vector<std::string> namedvalues = {"return", "end"}; // yes, global variables are bad
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

class warning
{
    public:
        warning(std::string warning, std::string note){
            std::cout << "Warning: " << warning << "\n";
            if (!note.empty()){
                std::cout << "Note: " << note << "\n";
            }
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

const std::string& ifstatement::codegen() {
    write("\nif(", false); this->condition->codegen();
    write("){\n", false);

    for (int i = 0; i < this->body.size()-1; i++){
        std::shared_ptr<astnode> j = this->body[i];
        j->codegen(); write(" ", false);
    }

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
    int j = 0;
    
    for (auto i : this->getArgs()) {       
        if (i == this->getArgs()[getArgs().size() - 1]) {
            if (this->getTypes()[j] == "str") {
                write(std::string("char[") + std::to_string(i.size()) + ']' + i + ")", false);
            } else {
                write(this->getTypes()[j] + " " + i + ")", false);
                break;
            }
        } else {
            if (this->getTypes()[j] == "str") {
                write("char[" + std::to_string(i.size()) + "]" + i + ",", false);
            } else {
                write(this->getTypes()[j] + " " + i + ",", false);
            }
        }
        j++;
    }

}

const std::string& astnode::codegen() {
    write(this->get_value(), false);
}


const std::string& funcdefinitionnode::codegen() { 

    if (this->proto->getName() == "main"){
        write("int main(){\n", false);

        for(auto s : this->body) {
            if (s->get_value() != "end"){
                s->codegen();
                write(" ", false);
            } else {
                write(";\n}", false);
            }
        }

    } else {
        write(this->getReturnType() + " ", false); this->proto->codegen(); write("{\n", false);

        for(auto s : this->body) {
            if (s->get_value() != "end"){
                s->codegen();
                write(" ", false);
            } else {
                write(";\n}", false);
            }
        }

    }
}

const std::string& callvariable::codegen() {
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

    for (auto i : alltokens){
        std::cout << i;
    }
    
    parses.index = 0;
    std::shared_ptr<ifstatement> bin = parses.parseIfStatement();
    ostream.open("cap.c");
    bin->codegen();
    ostream.close();
}
