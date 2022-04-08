#include "Parser.hpp"

void TokenParser::Parse(const std::string &str)
{
    
    if (StartCallbackFunc != nullptr) StartCallbackFunc("Start parsing!", this);
    
    struct Token token;
    
    for (auto it=str.begin(); it!=str.end(); ++it) {
        if (isspace(*it)) {
            if (!token.data.empty()) token_processing(token);
            continue;
        }
        token.data += *it;
        if (!std::isdigit(*it)) token.type = 's';
    }
    token_processing(token);
    
    if (StartCallbackFunc != nullptr) EndCallbackFunc("Finish parsing!", this);
}

void TokenParser::token_processing(struct Token &tok) {
    if (tok.type == 's') {
        if (StringCallbackFunc != nullptr) StringCallbackFunc(tok, this);
    } else {
        if (DigitCallbackFunc != nullptr) DigitCallbackFunc(tok, this);
    }
    tok.type = 'd';
    tok.data.clear();
}

void TokenParser::SetDigitCallback(void (*func) (const struct Token &, TokenParser *))
{
    DigitCallbackFunc = func;
}

void TokenParser::SetStringCallback(void (*func) (const struct Token &, TokenParser *))
{
    StringCallbackFunc = func;
}

void TokenParser::SetStartCallback(void (*func) (const std::string &, TokenParser *))
{
    StartCallbackFunc = func;
}

void TokenParser::SetEndCallback(void (*func) (const std::string &, TokenParser *))
{
    EndCallbackFunc = func;
}
