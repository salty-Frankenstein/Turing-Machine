/* testcases */
#include"either.h"
#include"parsec.h"
#include"parser.h"
#include"interpreter.h"
#include"util.h"
#include"ir.h"
#include<cassert>
#include<functional>
#include<string>
#include<sstream>
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

void print(int x) { cout << x << endl; }
void print(String x) { cout << show(x) << endl; }
void print(char x) { cout << x << endl; }
void print(string x) { cout << x << endl; }
template<typename A>
void print(list<A> x) { for (auto i : x) print(x); }
void print(list<string> x) { for (auto i : x) print(i); }

template<typename A>
void tParse(const string& testName, const Parsec<A> testParser,
    const string& testStr, bool expect,
    function<bool(Res<A>)> assertion = [](Res<A> a) {return true;}) {

    log("test parse " + testName);

    auto res = testParser(testStr);
    if (expect) {
        auto r = res->getRight();
        cout << "result: ";
        print(r.first);
        cout << "rest: " << show(r.second) << endl;
        assert(assertion(r));
    }
    else {
        cout << res->getLeft() << endl;
    }
}

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

    // assert(tostring(r1.first) == "ABC" && tostring(r1.second) == "");
    tParse("parser1 test1", parser1, "(abc)", true,
        function<bool(Res<String>)>([](Res<String> r) {
            return tostring(r.first) == "ABC"
                && tostring(r.second) == ""; }));
    tParse("parser1 test2", parser1, "(ab", false);
    tParse("parser1 test3", parser1, "a(ab)", false);
    tParse("parser1 test4", parser1, "(ab1)", false);

    auto allChar = satisfy([](char _) {return true;});
    auto notComma = satisfy([](char c) {return c != ',';});
    auto isComma = satisfy([](char c) {return c == ',';});
    auto parser2
        = many(notComma) << isComma << many(allChar)
        | many(notComma);
    tParse("parser2 test1", parser2, "asdf,adsf", true);
    tParse("parser2 test2", parser2, "asdfadsf", true);

    string s("abcde");
    auto parser3 = pure(' ');
    for (char c : s) {
        parser3 = parser3 >> satisfy([=](char _c) {return c == _c;});
    }
    tParse("parser3", parser3, s, true);

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
        { {"cmp", {'0', '1'}, {'_', '_'}, {'r', 'l'}, "0"} }
    );

    m.print();
    });

Test testParser = Test("Parser", []() {
    Parser p;
    auto res = p.preprocess({ "abcd \t ", "abc  ;d", " ;cd;e", " ;" });
    assert(res.front() == "abcd"); res.pop_front();
    assert(res.front() == "abc"); res.pop_front();
    assert(res.empty());

    ifstream f("../input/test.tm");
    auto code = p.readFile(f);
    auto res2 = p.preprocess(code);
    for (auto i : res2) {
        cout << i << endl;
    }

    tParse("state set", p.parseStateSet, "#Q = {0,cp,accept2,halt_}", true);
    tParse("input set", p.parseInputSet, "#S = {0,1}", true);
    tParse("tape set", p.parseTapeSet, "#G = {0,1,_,t,r,u,e,f,a,l,s}", true);
    tParse("final state set", p.parseFinalStateSet, "#F = {halt_accept}", true);

    tParse("int test1", p.parseTapeNum, "#N = 123", true);
    tParse("int test2", p.parseTapeNum, "#N = 123 234", true);
    tParse("int test3", p.parseTapeNum, "#N = 123abcde", true);
    tParse("int test4", p.parseTapeNum, "#N = hello123", false);

    p.parseFuncLine("cmp 01 __ rl reject").print();
    // p.parseFuncLine("cmp 01 __ xx reject").print();

    f.close();
    });

Test testTape = Test("Tape", []() {
    Tape t;
    t.print(cout);

    for (auto c : string("hello,world!!!")) {
        t.moveHead('l');
        t.access() = c;
    }
    t.moveHead('r');
    t.moveHead('r');
    t.print(cout);
    for (int i = 1; i <= 11; i++) {
        t.moveHead('r');
    }
    t.print(cout);

    t.access() = '_';
    t.moveHead('l');
    t.access() = '_';
    t.moveHead('l');
    t.print(cout);

    for (int i = 1; i <= 15; i++) {
        t.moveHead('r');
    }
    t.print(cout);

    });

Test testInterpreter = Test("Interpreter", []() {
    Parser p;
    ifstream f2("../input/test2.tm");
    auto code = p.readFile(f2);
    auto res2 = p.preprocess(code);
    p.dump(res2, "../input/dump.tm");
    auto tur = p.parse(res2);

    tur.print();

    Interpreter it(tur);
    // it.execute("01x010k1");
    // log("start execute 1");
    // it.execute("010101");
    log("start execute 2");
    it.execute("0101010");
    });

int main() {
    // testEither();
    // testParsec();
    // testIR();
    testParser();
    testTape();
    testInterpreter();
    return 0;
}