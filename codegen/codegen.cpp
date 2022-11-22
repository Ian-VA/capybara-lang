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

    if (!this->getArgs().empty()){
        if (this->getArgs().size() != 1){
            for (auto i : this->getArgs()) {
                if (i == this->getArgs()[getArgs().size() - 1]){
                    write(i->get_value() + ")", false);
                } else {
                    write(i->get_value() + ",", false);
                }
            }
        } else {
            write(getArgs()[0]->get_value() + ")", false);
        }
    } else {
        write(")", false);
    }
}

const std::string& variabledeclaration::codegen() {

    if(this->getvariabletype() == "string"){
        std::string out = "char [";
        out.push_back(this->get_value().size());
        write(out + "]" + this->get_identifier() + "=" + this->get_value(), false);
    } else {
        write(this->getvariabletype() + " " + this->get_identifier() + "=" + this->get_value(), false);
    }
}

const std::string& protonode::codegen(){
    write(this->getName() + "(", false);
    int j = 0;
    
    if (!this->getArgs().empty()){
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
    } else {
        write(")", false);
    }

}

const std::string& astnode::codegen() {
    switch(this->get_type())
    {
        case astnodetype::functiondeclaration:
        {
            std::shared_ptr<funcdefinitionnode> gen = std::make_shared<funcdefinitionnode>(dynamic_cast<funcdefinitionnode&>(*this)); gen->codegen();
            break;
        }
        case astnodetype::protonodes:
        {
            std::shared_ptr<protonode> gen = std::make_shared<protonode>(dynamic_cast<protonode&>(*this)); gen->codegen();
            break;
        }
        case astnodetype::variable:
        {
            std::shared_ptr<variabledeclaration> gen = std::make_shared<variabledeclaration>(dynamic_cast<variabledeclaration&>(*this)); gen->codegen();
            break;
        }
        case astnodetype::functioncall:
        {
            std::shared_ptr<callfunctionnode> gen = std::make_shared<callfunctionnode>(dynamic_cast<callfunctionnode&>(*this)); gen->codegen();
            break;
        }
        case astnodetype::ifs:
        {
            std::shared_ptr<ifstatement> gen = std::make_shared<ifstatement>(dynamic_cast<ifstatement&>(*this)); gen->codegen();
            break;
        }
        case astnodetype::null:
        {
            write(this->get_value(), false);
            break;
        }
        default:
            write(this->get_value(), false);
            break;
    }    
}

const std::string& funcdefinitionnode::codegen() { 

    if (this->proto->getName() == "main"){
        write("int main(){\n", false);

        if (!this->body.empty() || this->body[0]->get_value() != "end"){
            for(auto s : this->body) {
                if (s->get_value() != "end"){
                    if (s->get_type() == astnodetype::null) {
                        write(s->get_value(), false);
                    } else {
                        s->codegen();
                    }

                    if (s->get_value() != "return"){
                        write(" ", true);
                    } else {
                        write(" ", false);
                    }

                } else {
                    write("\n}", false);
                    write("\n", false);
                }
            }
        }

    } else {
        write(this->getReturnType() + " ", false); this->proto->codegen(); write("{\n", false);

        if (!this->body.empty() || this->body[0]->get_value() != "end"){
            for(auto s : this->body) {
                if (s->get_value() != "end"){
                    if (s->get_type() == astnodetype::null) {
                        write(s->get_value(), false);
                    } else {
                        s->codegen();
                    }


                    if (s->get_value() != "return"){
                        write(" ", true);
                    } else {
                        write(" ", false);
                    }

                } else {
                    write("\n}", false);
                    write("\n", false);
                }
            }
        }
    }
}


void compile(std::string input) 
{
    parserclass parses;
    std::deque<Token> alltokens = build_all(input); 
    parses.all_tokens = alltokens;

    std::deque<std::shared_ptr<astnode>> data = parses.parseAll();

    ostream.open("cap.c");

    for (auto i : data){
        i->codegen();
    }

    ostream.close();

}

void run()
{
    system("cap.exe");
}

int main()
{
    std::string input;
    while (true) {
        std::cout << "capybara> ";
        if (!std::getline(std::cin, input)) {
            break;
        } else {
            if (input.find("compile") != std::string::npos) {
                input.erase(input.find("compile"), 8);
                compile(input);
            } else if (input == "run") {
                run();
            }
        }
    }
}
