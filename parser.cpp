#include"parser.h"
#include<string>
using namespace std;

Parser::Parser()
    : allChar(satisfy([](char _) {return true;})) {
}

Parsec<char> Parser::isChar(char _c) {
    return satisfy([=](char c) {return c == _c;});
}

Parsec<char> Parser::notChar(char _c) {
    return satisfy([=](char c) {return c != _c;});
}

Parser::Code Parser::preprocess(const Parser::Code& code) {
    auto parseLine
        = many(notChar(';')) << isChar(';') << many(allChar)
        | many(notChar(';'));
    Parser::Code result;

    for (auto line : code) {
        auto res = parseLine(line);
        assert(res->isRight());     // this parsec shall parse all strings
        result.push_back(tostring(res->getRight().first));
    }
    return result;
}