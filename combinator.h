/*  HACK: this header is used in "parser.cpp" *ONLY* 
 * definition of several common parser combinitors
 */
#ifndef COMBINATOR_H
#define COMBINATOR_H

#include"parsec.h"
#include<string>
#include<list>
#include<sstream>

/* 'a'-'z' 'A'-'Z' '0'-'9' and '_' */
const auto nonBlank = satisfy([](char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_';}, "non-blank");

/* printable ascii, expect ' ', ',', ';', '{', '}', '*' */
const auto printable = satisfy([](char c) {
    return c >= 32 && c <= 127
        && c != ' ' && c != ','
        && c != ';' && c != '{'
        && c != '}' && c != '*';}, 
    "printable except ' ', ',', ';', '{', '}', '*' ");

/* all characters */
const auto allChar = satisfy([](char _) {return true;}, "all char");

Parsec<char> isChar(char _c) {
    return satisfy([=](char c) {return c == _c;},
        std::string("is char '") + _c + "'");
}

Parsec<char> notChar(char _c) {
    return satisfy([=](char c) {return c != _c;},
        std::string("is not char '") + _c + "'");
}

Parsec<char> isString(const std::string& s) {
    auto parser = pure(' ');
    for (char c : s) {
        parser = parser >> isChar(c);
    }
    return parser;
}

/* parses /one/ or more occurrences of p, separated
 * by sep. Returns a list of values returned by p.
 * > pure (:) <*> p <*> many (sep >> p)
 */
template<typename A, typename SEP>
Parsec<std::list<A>> sepBy(const Parsec<A> p, const Parsec<SEP> sep) {
    // HACK: cons is defined internel here, 
    // since it will cause a bad function call otherwise
    // IDK why...
    std::function<std::function<std::list<A>(std::list<A>)>(A)> cons =
        [](const A& a) {
        return [=](std::list<A> l) {
            l.push_front(a);
            return l;
        };};
    return pure(cons) * p * many(sep >> p);
}

// TODO: test this
const Parsec<int> parseInt = Parsec<int>(
    [](String s)->ParseRes<int> {
        std::stringstream ss(tostring(s));
        int result;
        std::string rest;
        ss >> result;
        if (ss.eof() || ss.good()) {
            ss >> rest;
            return makeRes<int>({ result, toString(rest) });
        }
        else {
            return makeError<int>("Error when parsing " + show(s) +
                ", an integer expected");
        }
    }
);

#endif