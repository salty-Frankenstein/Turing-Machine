#include"parser.h"
#include<string>
#include<sstream>
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

/* parses /one/ or more occurrences of p, separated
 * by sep. Returns a list of values returned by p.
 * > pure (:) <*> p <*> many (sep >> p)
 */
template<typename A, typename SEP>
Parsec<list<A>> sepBy(const Parsec<A> p, const Parsec<SEP> sep) {
    // HACK: cons is defined internel here, 
    // since it will cause a bad function call otherwise
    // IDK why...
    function< function<list<A>(list<A>)>(A)> cons =
        [](const A& a) {
        return [=](list<A> l) {
            l.push_front(a);
            return l;
        };};
    return pure(cons) * p * many(sep >> p);
}

/* parse set with given set identifier
 * and element parser combinator
 */
template<typename A>
Parsec<list<A>> parseSet(const string& id, const Parsec<A> parseElem) {
    return isString(id) >>
        isChar('{') >> sepBy(parseElem, isChar(',')) << isChar('}');
}

const auto parseState = pure(function<string(String)>(tostring)) * many(nonBlank);
const auto parseChar = printable;

const Parsec<list<State>> Parser::parseStateSet = parseSet("#Q = ", parseState);
const Parsec<list<Char>> Parser::parseInputSet = parseSet("#S = ", parseChar);
const Parsec<list<Char>> Parser::parseTapeSet = parseSet("#G = ", parseChar);
const Parsec<list<State>> Parser::parseFinalStateSet = parseSet("#F = ", parseState);

// TODO: test this
const Parsec<int> parseInt = Parsec<int>(
    [](String s)->ParseRes<int> {
        stringstream ss(tostring(s));
        int result;
        string rest;
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

const Parsec<int> Parser::parseTapeNum = isString("#N = ") >> parseInt;

/* apply the given parser combinator
 * and consume the code if succeeded
 * throw an exception otherwise
 */
template<typename A>
Res<A> applyParsec(const Parsec<A> p, Parser::Code& code) {
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

TuringMachine Parser::parse(Parser::Code code) {
    try {
        auto stateSet = applyParsec(parseStateSet, code);
        auto inputSet = applyParsec(parseInputSet, code);
        auto tapeSet = applyParsec(parseTapeSet, code);
        auto initState = applyParsec(isString("#q0 = ") >> parseState, code);
        auto blank = applyParsec(isString("#B = ") >> parseChar, code);
        auto finalStateSet = applyParsec(parseFinalStateSet, code);
        auto tapeNum = applyParsec(parseTapeNum, code);
        
    }
    catch (const EOF_Error& e) {
        //TODO
    }
    catch (const ParseError& e) {
        std::cerr << e.what() << '\n';
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
