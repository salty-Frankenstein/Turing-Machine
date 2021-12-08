#ifndef PARSER_H
#define PARSER_H

#include"parsec.h"
#include"ir.h"
#include<string>
#include<list>

class Parser {
public:
    /* code is lines of strings */
    using Code = std::list<std::string>;

    Parser();

    /* removing all comments and empty lines */
    Code preprocess(const Code& code);

private:
    bool isEmpty(const std::string& s);
    const Parsec<char> allChar;
    Parsec<char> isChar(char c);
    Parsec<char> notChar(char c);
};



#endif