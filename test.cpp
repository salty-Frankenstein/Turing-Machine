/* testcases */
#include"either.h"
#include"parsec.h"
#include"parser.h"
#include"util.h"
#include"ir.h"
#include<cassert>
#include<functional>
#include<string>
#include<fstream>

using namespace std;

class Test {
public:
    Test(const string& _name, function<void()> _callback)
        : name(_name), callback(_callback) {
    }

    void operator()() {
        log("Start testing \033[33m" + name + "\033[0m:");
        callback();
        log("Test " + name + " \033[32mpassed\033[0m.");
    }

private:
    const string name;
    const function<void()> callback;
};


Test testEither = Test("Either", []() {
    Either<int, bool> l1 = Left<int, bool>(1),
        l2 = l1,
        r1 = Right<int, bool>(false),
        r2 = r1;
    assert(l1->isLeft() && l2->isLeft());
    assert(!r1->isLeft() && !r2->isLeft());
    assert(!l1->isRight() && !l2->isRight());
    assert(r1->isRight() && r1->isRight());
    assert(l1->getLeft() == l2->getLeft());
    assert(r1->getRight() == r2->getRight());
    cout << l1->getLeft() << endl;
    // assert(l1->getRight() == l2->getRight());
    // assert(r1->getLeft() == r2->getLeft());

    Either<int, Either<int, bool>>
        l3 = Left<int, Either<int, bool>>(1),
        r3 = Right<int, Either<int, bool>>(r2);
    cout << l3->getLeft() << endl;
    cout << r3->getRight()->getRight() << endl;
    });

Test testParsec = Test("Parsec", []() {
    auto alpha = satisfy([](char c) {return isalpha(c);});
    auto leftP = satisfy([](char c) {return c == '(';});
    auto rightP = satisfy([](char c) {return c == ')';});
    auto toUpper = pure(function<char(char)>([](char c) {return c - 'a' + 'A';}));
    auto parser1 = leftP >> many(toUpper * alpha) << rightP;
    auto res1 = parser1("(abc)");
    assert(res1->isRight());
    auto r1 = res1->getRight();
    cout << show(r1.first) + ", " + show(r1.second) << endl;
    cout << endl;
    assert(tostring(r1.first) == "ABC" && tostring(r1.second) == "");

    auto res2 = parser1("(ab");
    assert(res2->isLeft());
    cout << res2->getLeft() << endl;
    cout << endl;

    auto res3 = parser1("a(ab)");
    assert(res3->isLeft());
    cout << res3->getLeft() << endl;
    cout << endl;

    auto res4 = parser1("(ab1)");
    assert(res4->isLeft());
    cout << res4->getLeft() << endl;
    cout << endl;

    auto allChar = satisfy([](char _) {return true;});
    auto notComma = satisfy([](char c) {return c != ',';});
    auto isComma = satisfy([](char c) {return c == ',';});
    auto parser2
        = many(notComma) << isComma << many(allChar)
        | many(notComma);
    auto res5 = parser2("asdf,adsf");
    assert(res5->isRight());
    auto r5 = res5->getRight();
    cout << show(r5.first) + ", " + show(r5.second) << endl;
    cout << endl;

    auto res6 = parser2("asdfadsf");
    assert(res6->isRight());
    auto r6 = res6->getRight();
    cout << show(r6.first) + ", " + show(r6.second) << endl;
    cout << endl;

    string s("abcde");
    auto parser3 = pure(' ');
    for (char c : s) {
        parser3 = parser3 >> satisfy([=](char _c) {return c == _c;});
    }
    auto res7 = parser3("abcde");
    assert(res7->isRight());
    auto r7 = res7->getRight();
    cout << r7.first << ", " + show(r7.second) << endl;
    cout << endl;

    });

Test testIR = Test("IR", []() {
    TuringMachine m(
        { "0", "cmp", "halt_accept" },
        { '0', '1' },
        { '0', '1', '_' },
        { "0" },
        '_',
        { "halt_accept" },
        2,
        { {"cmp", {'0', '1'}, {'_', '_'}, {RIGHT, LEFT}, "0"} }
    );

    m.print();
    });

Test testParser = Test("Parser", []() {
    Parser p;
    auto res = p.preprocess({ "abcd", "abc;d", " ;cd;e", " ;" });
    assert(res.front() == "abcd"); res.pop_front();
    assert(res.front() == "abc"); res.pop_front();
    assert(res.empty());

    ifstream f("input/test.tm");
    auto code = p.readFile(f);
    auto res2 = p.preprocess(code);
    for (auto i : res2) {
        cout << i << endl;
    }

    log("test parse state set");
    auto res3 = p.parseStateSet("#Q = {0,cp,accept2,halt_}");
    // cout << res3->getLeft() << endl;
    assert(res3->isRight());
    auto r3 = res3->getRight();
    for(auto i : r3.first){
        cout << i << endl;
    }
    });

int main() {
    testEither();
    testParsec();
    testIR();
    testParser();
    return 0;
}