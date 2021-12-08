#ifndef PARSER_H
#define PARSER_H

#include"parsec.h"
#include"ir.h"
#include<string>


class Parser {
public:
    /* removing all comments */
    std::string preprocess(const std::string& code);

private:
    // Parsec<char> 
};



#endif