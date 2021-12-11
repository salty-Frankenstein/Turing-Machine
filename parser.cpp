#include"parser.h"
#include"combinator.h"
#include<string>
using namespace std;

/* parse set with given set identifier
 * and element parser combinator
 */
template<typename A>
Parsec<list<A>> parseSet(const string& id, const Parsec<A> parseElem) {
    return isString(id) >>
        isChar('{') >> sepBy(parseElem, isChar(',')) << isChar('}');
}

const auto parseState = pure(function<string(String)>(tostring)) * many1(nonBlank);
const auto parseChar = printable;

const Parsec<list<State>> Parser::parseStateSet = parseSet("#Q = ", parseState);
const Parsec<list<Char>> Parser::parseInputSet = parseSet("#S = ", parseChar);
const Parsec<list<Char>> Parser::parseTapeSet = parseSet("#G = ", parseChar);
const Parsec<list<State>> Parser::parseFinalStateSet = parseSet("#F = ", parseState);
const Parsec<int> Parser::parseTapeNum = isString("#N = ") >> parseInt;

const Parsec<Direction> parseDirection = isChar('l') | isChar('r') | isChar('*');

/* apply the given parser combinator
 * and consume the code if succeeded
 * throw an exception otherwise
 */
template<typename A>
Res<A> applyParsecLn(const Parsec<A> p, Parser::Code& code) {
    if (code.empty()) {
        throw EOF_Error();
    }
    auto res = p(code.front());
    if (res->isRight()) {
        code.pop_front();
        return res->getRight();
    }
    throw ParseError(res->getLeft());
}

/* apply the given parser combinator
 * throw an exception if failed
 */
template<typename A>
Res<A> applyParsec(const Parsec<A> p, const String& s) {
    auto res = p(s);
    if (res->isRight()){
        return res->getRight();
    }
    throw ParseError(res->getLeft());
}

FuncLine Parser::parseFuncLine(const string& s) {
    try {
        auto oldState = applyParsec(parseState << isChar(' '), toString(s));
        auto oldChar = applyParsec(many1(parseChar) << isChar(' '), oldState.second);
        auto newChar = applyParsec(many1(parseChar) << isChar(' '), oldChar.second);
        auto direction = applyParsec(many1(parseDirection) << isChar(' '), newChar.second);
        auto newState = applyParsec(parseState, direction.second);
        return { oldState.first, oldChar.first, newChar.first, direction.first, newState.first };
    }
    catch (const ParseError& e) {
        std::cerr << e.what() << '\n';
        exit(-1);
        // TODO: error handling
    }
}

TuringMachine Parser::parse(Parser::Code code) {
    try {
        auto stateSet = applyParsecLn(parseStateSet, code);
        auto inputSet = applyParsecLn(parseInputSet, code);
        auto tapeSet = applyParsecLn(parseTapeSet, code);
        auto initState = applyParsecLn(isString("#q0 = ") >> parseState, code);
        auto blank = applyParsecLn(isString("#B = ") >> parseChar, code);
        auto finalStateSet = applyParsecLn(parseFinalStateSet, code);
        auto tapeNum = applyParsecLn(parseTapeNum, code);
        
    }
    catch (const EOF_Error& e) {
        //TODO
    }
    catch (const ParseError& e) {
        std::cerr << e.what() << '\n';
        // TODO 
    }

}

Parser::Code Parser::readFile(istream& is) {
    Parser::Code result;

    while (!is.eof()) {
        string s;
        getline(is, s);
        result.push_back(s);
    }
    return result;
}

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
