#include"interpreter.h"
#include"util.h"
#include"config.h"
#include<cassert>
#include<algorithm>
#include<sstream>
using namespace std;

Tape::Tape() : head(0), pos({ '_' }) {}

Char& Tape::access() {
    if (head >= 0) {
        return ::access(pos, head);
    }
    return ::access(neg, -head - 1);
}

int Tape::getHead() {
    return head;
}

bool Tape::check() {
    return (head >= 0 && head < pos.size())
        || (head < 0 && (-head - 1) < neg.size());
}

void Tape::moveHead(Direction d) {
    switch (d) {
    case 'l':
        if (head >= 0 && head == pos.size() - 1 && access() == '_') {
            pos.pop_back();
        }
        head--;
        if (!check()) {
            neg.push_back('_');
            assert(check());
        }
        assert(check());
        break;
    case 'r':
        if (head < 0 && -head == neg.size() && access() == '_') {
            neg.pop_back();
        }
        head++;
        if (!check()) {
            pos.push_back('_');
            assert(check());
        }
        assert(check());
        break;
    case '*': break;
    default:
        PANIC("bad direction");
    }
}

Tape::TapeRep Tape::showTape() {
    TapeRep res;
    if (neg.size() == 0) {
        int k = 0;
        while (pos[k] == '_') { k++; }
        for (int i = min(k, head); i < pos.size(); i++) {
            res.push_back({ i, pos[i] });
        }
    }
    else if (pos.size() == 0) {
        int k = 1;
        while (neg[k - 1] == '_') { k++; }
        for (int i = neg.size(); i >= min(abs(head), k); i--) {
            res.push_back({ -i, neg[i - 1] });
        }
    }
    else {
        for (int i = neg.size(); i >= 1; i--) {
            res.push_back({ -i, neg[i - 1] });
        }
        for (int i = 0; i < pos.size(); i++) {
            res.push_back({ i, pos[i] });
        }
    }
    return res;
}

string Tape::showResult() {
    auto s = showTape();
    list<char> res;
    for (auto i : s) {
        res.push_back(i.second);
    }
    while (!res.empty() && res.front() == '_') {
        res.pop_front();
    }
    while (!res.empty() && res.back() == '_') {
        res.pop_back();
    }
    return string(res.begin(), res.end());
}

Interpreter::Interpreter(const TuringMachine& _tm)
    : tm(_tm), tapes(_tm.tapeNum), state(_tm.initState), step(0), halt(false) {}

void Interpreter::printState(ostream& os) {
    os << "Step\t: " << step << endl;
    for (int i = 0; i < tm.tapeNum; i++) {
        auto show = tapes[i].showTape();
        // print index
        os << "Index" << i << "\t: ";
        for (auto p : show) {
            os << abs(p.first) << " ";
        }
        os << endl;

        // print tape
        os << "Tape" << i << "\t: ";
        for (auto p : show) {
            os << p.second << string(digitNum(abs(p.first)), ' ');
        }
        os << endl;

        // print head
        os << "Head" << i << "\t: ";
        for (auto p : show) {
            if (p.first == tapes[i].getHead()) {
                os << '^' << string(digitNum(abs(p.first)), ' ');
            }
            else {
                os << string(digitNum(abs(p.first)) + 1, ' ');
            }
        }
        os << endl;
    }

    os << "State\t: " << state << endl;
}

void Interpreter::execute(const string& s) {
    try {
        checkLegal(s);
        initTape(s);
        if (Mode::getMode() == VERBOSE) {
            cout << "Input: " << s << endl;
            cout << "==================== RUN ====================\n";
            printState(cout);
            while (!halt && !inSet(state, tm.finalStateSet)) {
                cout << "---------------------------------------------\n";
                singleStep();
                printState(cout);
                // int x;
                // cin >> x;
#ifndef NDEBUG 
                system("sleep 1");
#endif
            }
        }
        else {
            while (!halt && !inSet(state, tm.finalStateSet)) {
                singleStep();
            }
            cout << tapes[0].showResult() << endl;
        }

    }
    catch (const IllegalError& e) {
        stringstream err;
        if (Mode::getMode() == VERBOSE) {
            err << "==================== ERR ====================\n"
                << "error: '"
                << s[e.position]
                << "' was not declared in the set of input symbols\n"
                << "Input: " + s + "\n"
                << string(e.position + 7, ' ') << "^\n"
                << "==================== END ====================";
        }
        else {
            err << "illegal input" << endl;
        }
        error(err.str(), 1);
    }
}

void Interpreter::checkLegal(const string& s) const {
    for (int i = 0; i < s.length(); i++) {
        if (!inSet(s[i], tm.inputSet)) {
            throw IllegalError(i);
        }
    }
}

void Interpreter::initTape(const string& s) {
    for (auto c : s) {
        tapes[0].access() = c;
        tapes[0].moveHead('r');
    }
    while (tapes[0].getHead() != 0) {
        tapes[0].moveHead('l');
    }
}

void Interpreter::singleStep() {
    transfer();
    step++;
}

void Interpreter::transfer() {
    for (auto fl : tm.function) {
        if (match(fl)) {
            for (int i = 0; i < tm.tapeNum; i++) {
                // assign new tape characters
                if (fl.newChar[i] != '*') { // wildcast
                    tapes[i].access() = fl.newChar[i];
                }
                // move the heads
                tapes[i].moveHead(fl.direction[i]);
            }
            state = fl.newState;
            return;
        }
    }
    // no rule matches, then halt
    halt = true;
}

bool Interpreter::match(const FuncLine& fl) {
    if (fl.oldState != state) {
        return false;
    }
    for (int i = 0; i < tm.tapeNum; i++) {
        if (!match(tapes[i].access(), fl.oldChar[i])) {
            return false;
        }
    }
    return true;
}

bool Interpreter::match(Char c, Char pattern) {
    if (pattern == '*') {    // wildcard
        return true;
    }
    return c == pattern;
}

#ifndef NDEBUG
void Tape::print(ostream& os) {
    int i = 0;
    auto show = showTape();
    // print index
    os << "Index" << i << "\t: ";
    for (auto p : show) {
        os << abs(p.first) << " ";
    }
    os << endl;

    // print tape
    os << "Tape" << i << "\t: ";
    for (auto p : show) {
        os << p.second << string(digitNum(abs(p.first)), ' ');
    }
    os << endl;

    // print head
    os << "Head" << i << "\t: ";
    for (auto p : show) {
        if (p.first == getHead()) {
            os << '^' << string(digitNum(abs(p.first)), ' ');
        }
        else {
            os << string(digitNum(abs(p.first)) + 1, ' ');
        }
    }
    os << endl;

}
#endif
