#ifndef PARSER_H
#define PARSER_H

#include"parsec.h"
#include"ir.h"
#include"util.h"
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
    static TuringMachine parse(Code code);

    const static Parsec<std::list<State>> parseStateSet;
    const static Parsec<std::list<Char>> parseInputSet;
    const static Parsec<std::list<Char>> parseTapeSet;
    const static Parsec<std::list<State>> parseFinalStateSet;
    const static Parsec<int> parseTapeNum;
    const static Parsec<FuncLine> parseFuncLine;

private:
    static bool isEmpty(const std::string& s);
};

#endif