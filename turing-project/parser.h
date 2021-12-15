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

    /* Spec: 
     * > remove all comments and empty lines 
     * > remove all space characters at the end of each lines
     * > sort all lines to the normal order
     */
    static Code preprocess(const Code& code);

    /* parse the sourse code into IR */
    static TuringMachine parse(Code code);

#ifndef NDEBUG
    /* dump the code to the path */
    static void dump(const Code& code, const std::string& path);
#endif

    const static Parsec<std::list<State>> parseStateSet;
    const static Parsec<std::list<Char>> parseInputSet;
    const static Parsec<std::list<Char>> parseTapeSet;
    const static Parsec<std::list<State>> parseFinalStateSet;
    const static Parsec<int> parseTapeNum;
    static FuncLine parseFuncLine(const std::string& s);

};

#endif