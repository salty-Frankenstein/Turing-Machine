#include"parser.h"
#include"combinator.h"
#include"config.h"
#include<string>
#include<fstream>
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
const auto parseChar = printableExNoWild;

const Parsec<list<State>> Parser::parseStateSet = parseSet("#Q = ", parseState);
const Parsec<list<Char>> Parser::parseInputSet = parseSet("#S = ", parseChar);
const Parsec<list<Char>> Parser::parseTapeSet = parseSet("#G = ", parseChar);
const Parsec<list<State>> Parser::parseFinalStateSet = parseSet("#F = ", parseState);
const Parsec<int> Parser::parseTapeNum = isString("#N = ") >> parseInt;

const Parsec<Direction> parseDirection = isChar('l') | isChar('r') | isChar('*');

/* apply the given parser combinator
 * retrue the result and consume the code if succeeded
 * throw an exception otherwise
 */
template<typename A>
A applyParsecLn_(const Parsec<A> p, Parser::Code& code) {
    // TODO: check if the rest of the string is empty
    if (code.empty()) {
        throw EOF_Error();
    }
    auto res = p(code.front());
    if (res->isRight()) {
        code.pop_front();
        return res->getRight().first;
    }
    throw ParseError(res->getLeft());
}

/* apply the given parser combinator
 * throw an exception if failed
 */
template<typename A>
Res<A> applyParsec(const Parsec<A> p, const String& s) {
    auto res = p(s);
    if (res->isRight()) {
        return res->getRight();
    }
    throw ParseError(res->getLeft());
}

FuncLine Parser::parseFuncLine(const string& s) {
    auto oldState = applyParsec(parseState << isChar(' '), toString(s));
    auto oldChar = applyParsec(many1(parseChar) << isChar(' '), oldState.second);
    auto newChar = applyParsec(many1(parseChar) << isChar(' '), oldChar.second);
    auto direction = applyParsec(many1(parseDirection) << isChar(' '), newChar.second);
    auto newState = applyParsec(parseState, direction.second);
    return { oldState.first, oldChar.first, newChar.first, direction.first, newState.first };
}

TuringMachine Parser::parse(Parser::Code code) {
    try {
        auto stateSet = applyParsecLn_(parseStateSet, code);
        auto inputSet = applyParsecLn_(parseInputSet, code);
        auto tapeSet = applyParsecLn_(parseTapeSet, code);
        auto initState = applyParsecLn_(isString("#q0 = ") >> parseState, code);
        auto blank = applyParsecLn_(isString("#B = ") >> parseChar, code);
        auto finalStateSet = applyParsecLn_(parseFinalStateSet, code);
        auto tapeNum = applyParsecLn_(parseTapeNum, code);
        
        if (tapeNum <= 0) {
            throw KeyError("the tape number is less than 1");
        }
        
        list<FuncLine> func;
        while (!code.empty()) {
            func.push_back(parseFuncLine(code.front()));
            code.pop_front();
        }
        TuringMachine res = {
            stateSet, inputSet, tapeSet, initState, blank,
            finalStateSet, static_cast<size_t>(tapeNum), func
        };
        res.checkWellFormed();
        return res;
    }
    catch (const EOF_Error& e) {
        cerr << "syntax error" << endl;
        if(Mode::getMode() == VERBOSE) {
            cerr << "unexpected end of file" << endl;
        }
        exit(1);
    }
    catch (const ParseError& e) {
        cerr << "syntax error" << endl;
        if(Mode::getMode() == VERBOSE) {
            cerr << e.what() << endl;
        }
        exit(1);
    }
    catch (const IllFormedError& e) {
        cerr << "ill-formed error" << endl;
        if(Mode::getMode() == VERBOSE) {
            cerr << e.what() << endl;
            cerr << "in function line: " << e.f.repr() << endl;
        }
        exit(1);
    }
    catch (const KeyError& e) {
        cerr << "ill-formed error" << endl;
        if(Mode::getMode() == VERBOSE) {
            cerr << e.what() << endl;
        }
        exit(1);
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

Parser::Code Parser::preprocess(const Parser::Code& code) {
    auto parseLine
        = many(notChar(';')) << isChar(';') << many(anyChar)
        | many(notChar(';'));
    Parser::Code result;

    for (auto line : code) {
        auto res = parseLine(line);
        assert(res->isRight());     // this parsec shall parse all strings
        auto newLine = tostring(res->getRight().first);

        // remove spaces at the end of line
        while (!newLine.empty() && isSpace(newLine.back())) {
            newLine.pop_back();
        }

        // remove empty lines
        if (!newLine.empty()) {
            result.push_back(newLine);
        }
    }

    auto toOrd = [](const string& s) {
        if (s.length() < 2) {
            return 8;
        }
        if(s[0] != '#') return 7;
        char c = s[1];
        switch (c) {
        case 'Q': return 0;
        case 'S': return 1;
        case 'G': return 2;
        case 'q': return 3;
        case 'B': return 4;
        case 'F': return 5;
        case 'N': return 6;
        default: return 7;
        }
    };

    // sort all lines
    result.sort([&](string& a, string& b) { return toOrd(a) < toOrd(b); });
    return result;
}

#ifndef NDEBUG

void Parser::dump(const Code& code, const std::string& path) {
    ofstream out(path);
    for (auto i : code) {
        out << i << endl;
    }
    out.close();
}

#endif
