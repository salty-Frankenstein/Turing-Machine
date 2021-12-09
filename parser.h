#ifndef PARSER_H
#define PARSER_H

#include"parsec.h"
#include"ir.h"
#include<string>
#include<list>
#include<iostream>

class Parser {
public:
    /* code is lines of strings */
    using Code = std::list<std::string>;

    /* read code file from istream  */
    static Code readFile(std::istream& is);

    /* removing all comments and empty lines */
    static Code preprocess(const Code& code);

    /* parse the sourse code into IR */
    static TuringMachine parse(const Code& code);

    const static Parsec<std::list<State>> parseStateSet;
private:
    static bool isEmpty(const std::string& s);
};

#endif