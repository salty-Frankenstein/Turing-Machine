#include"parser.h"
#include<string>
using namespace std;

/* 'a'-'z' 'A'-'Z' '0'-'9' and '_' */
const auto nonBlank = satisfy([](char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_';}, "non-blank");

/* printable ascii */
const auto printable = satisfy([](char c) {
    return c >= 32 && c <= 127;}, "printable");

/* all characters */
const auto allChar = satisfy([](char _) {return true;}, "all char");

Parsec<char> isChar(char _c) {
    return satisfy([=](char c) {return c == _c;},
        string("is char '") + _c + "'");
}

Parsec<char> notChar(char _c) {
    return satisfy([=](char c) {return c != _c;},
        string("is not char '") + _c + "'");
}

Parsec<Char> isString(const string& s) {
    auto parser = pure(' ');
    for (char c : s) {
        parser = parser >> isChar(c);
    }
    return parser;
}

const auto parseState = pure(function<State(String)>(tostring)) * many(nonBlank);
const auto parseChar = printable;

Parser::Code Parser::readFile(istream& is) {
    Parser::Code result;

    while (!is.eof()) {
        string s;
        getline(is, s);
        result.push_back(s);
    }
    return result;
}

TuringMachine Parser::parse(const Parser::Code& code) {
}

const Parsec<list<State>> Parser::parseStateSet
= isString("#Q = ") >>
isChar('{') >> many(parseState << isChar(',')) << isChar('}');

bool Parser::isEmpty(const string& s) {
    for (auto c : s) {
        if (c != ' ' && c != '\t' && c != '\n') {
            return false;
        }
    }
    return true;
}

Parser::Code Parser::preprocess(const Parser::Code& code) {
    auto parseLine
        = many(notChar(';')) << isChar(';') << many(allChar)
        | many(notChar(';'));
    Parser::Code result;

    for (auto line : code) {
        auto res = parseLine(line);
        assert(res->isRight());     // this parsec shall parse all strings
        auto newLine = tostring(res->getRight().first);
        if (!isEmpty(newLine)) {
            result.push_back(newLine);
        }
    }
    return result;
}
