#pragma once

#include <cstddef>
#include <iostream>

struct Token
{
	std::string data;
    char type = 'd';
};

class TokenParser
{
private:
    void (*StartCallbackFunc)(const std::string &, TokenParser *) = nullptr;
    void (*DigitCallbackFunc)(const struct Token &, TokenParser *) = nullptr;
    void (*StringCallbackFunc)(const struct Token &, TokenParser *) = nullptr;
    void (*EndCallbackFunc)(const std::string &, TokenParser *) = nullptr;
public:

    unsigned dig_token_num = 0;
    unsigned str_token_num = 0;
    
    TokenParser() = default;

    void SetStartCallback(void (*func) (const std::string &, TokenParser *));
    void SetEndCallback(void (*func) (const std::string &, TokenParser *));
    void SetStringCallback(void (*func) (const struct Token &, TokenParser *));
    void SetDigitCallback(void (*func) (const struct Token &, TokenParser *));
    
    void Parse(const std::string &);
    void token_processing(struct Token &);
};
