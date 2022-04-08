#include <iostream>
#include <gtest/gtest.h>
#include <fstream>

#include "Parser.hpp"

class TestFoo : public ::testing::Test
{
protected:
    std::ifstream infile;
	void SetUp()
	{
        infile.open("test.txt");
        std::cout << "SetUp" << std::endl;
	}
	void TearDown()
	{
        infile.close();
        std::cout << "TearDown" << std::endl;
	}
};

/* The test should print a message about the start and end
of parsing of each line and statistics on the number of string and digit tokens.
Each token and its type is printed. */

static void print_statistic(const std::string &msg, TokenParser *pars) {
    std::cout << "\n" << msg << "\n";
    std::cout << "Number of digit tokens: " << pars->dig_token_num << "\n";
    std::cout << "Number of string tokens: " << pars->str_token_num << "\n\n";
}
static void dig_token_func(const struct Token &tok, TokenParser *pars) {
    pars->dig_token_num++;
    std::cout << "Digit token: " << tok.data << "\n";
}
static void str_token_func(const struct Token &tok, TokenParser *pars) {
    pars->str_token_num++;
    std::cout << "String token: " << tok.data << "\n";
}
    
TEST_F(TestFoo, test_set)
{
    TokenParser parser;
    parser.SetStartCallback(print_statistic);
    parser.SetEndCallback(print_statistic);
    parser.SetDigitCallback(dig_token_func);
    parser.SetStringCallback(str_token_func);
    
    std::string line;
    while (std::getline(infile, line)) {
        parser.Parse(line);
    }
}

TEST_F(TestFoo, test_no_set)
{
    TokenParser parser;
    std::string line;
    while (std::getline(infile, line)) {
        parser.Parse(line);
    }
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
