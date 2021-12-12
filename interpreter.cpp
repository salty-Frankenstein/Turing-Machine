#include"interpreter.h"
#include"util.h"
#include"config.h"
#include<cassert>
#include<algorithm>
using namespace std;

Interpreter::Interpreter(const TuringMachine& _tm)
    : tm(_tm) {
    assert(tm.isWellFormed());
}

void Interpreter::execute(const string& s) const {
    try {
        checkLegal(s);
    }
    catch (const IllegalError& e) {
        string err;
        if (Mode::getMode() == VERBOSE) {
            err = string("==================== ERR ====================\n")
                + "error: '"
                + s[e.position]
                + "' was not declared in the set of input symbols\n"
                + "Input: " + s + "\n"
                + string(e.position + 7, ' ') + "^\n"
                + "==================== END ====================";
        }
        else {
            err = "illegal input\n";
        }
        error(err, -1);
    }
}

void Interpreter::checkLegal(const string& s) const {
    for (int i = 0; i < s.length(); i++) {
        if (!inSet(s[i], tm.inputSet)) {
            throw IllegalError(i);
        }
    }
}