#include "Parser.hpp"

int main()
{
    TokenParser parser;
    std::string line;
    while (std::getline(std::cin, line)) {
        parser.Parse(line);
    }
}
