#include <iostream>
#include <string>


std::string evaluate(std::string input)
{
    return input;
}

std::string readln(std::string input)
{
    return input;
}

std::string out(std::string input)
{
    return input;
}

std::string rep(std::string input)
{
    auto read = readln(input);
    auto res = evaluate(read);
    return out(res);
}


int main()
{
    std::string input;
    while (true) {
        std::cout << "capybara> ";
        if (!std::getline(std::cin, input)) {
            break;
        } else {
            std::cout << rep(input) << "\n";
        }
    }
}
