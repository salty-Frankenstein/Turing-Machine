/* intermediate representation */
#include"ir.h"
#include"util.h"
#include<iostream>
#include<numeric>
#include<algorithm>
#include<sstream>
using namespace std;

FuncLine::FuncLine(const State& _oldState,
    const std::list<Char>& _oldChar,
    const std::list<Char>& _newChar,
    const std::list<Direction>& _direction,
    const State& _newState
) :
    oldState(_oldState),
    oldChar(_oldChar.begin(), _oldChar.end()),
    newChar(_newChar.begin(), _newChar.end()),
    direction(_direction.begin(), _direction.end()),
    newState(_newState) {
}

bool FuncLine::isWellFormed(size_t n) const {
    // TODO: check if all symbols and states are in the set
    return oldChar.size() == n
        && newChar.size() == n
        && direction.size() == n;
}

bool FuncLine::operator<(const FuncLine& f) {
    return count(oldChar.begin(), oldChar.end(), '*')
        < count(f.oldChar.begin(), f.oldChar.end(), '*');
}

string FuncLine::repr() const {
    stringstream s;
    s << oldState << ' ';
    for(auto c : oldChar) { s << c; }
    s << ' ';
    for(auto c : newChar) { s << c; }
    s << ' ';
    for(auto c : direction) { s << c; }
    s << ' ' << newState;
    return s.str();
}

TuringMachine::TuringMachine(
    const std::list<State>& _stateSet,
    const std::list<Char>& _inputSet,
    const std::list<Char>& _tapeSet,
    const State& _initState,
    const Char& _blank,
    const std::list<State>& _finalStateSet,
    const size_t& _tapeNum,
    const std::list<FuncLine>& _function
) :
    stateSet(_stateSet),
    inputSet(_inputSet),
    tapeSet(_tapeSet),
    initState(_initState),
    blank(_blank),
    finalStateSet(_finalStateSet),
    tapeNum(_tapeNum),
    function(sort(_function)) {
}

void TuringMachine::checkWellFormed() const {
    int n = tapeNum;
    if (!inSet(initState, stateSet)) {
        throw KeyError("the initial state \"" + initState + "\" is not in the state set");
    }
    for (auto s : finalStateSet) {
        if (!inSet(s, stateSet)) {
            throw KeyError("the final state \"" + s + "\" is not in the state set");
        }
    }
    // check function 
    for (auto f : function) {
        if (!f.isWellFormed(n)) {
            throw IllFormedError(
                "function cannot match with the tape number", f);
        }
        try {
            checkState(f.oldState);
            checkState(f.newState);
            for (auto c : f.oldChar) { checkTapeSymbol(c); }
            for (auto c : f.newChar) { checkTapeSymbol(c); }
        }
        catch (const KeyError& e) {
            throw IllFormedError(e.info +" is not declared", f);
        }
    }
}

void TuringMachine::checkState(const State& s) const {
    if(!inSet(s, stateSet)) {
        throw KeyError("state \"" + s + "\"");
    }
}

void TuringMachine::checkTapeSymbol(const Char c) const {
    if(c != '*' && !inSet(c, tapeSet)) {
        throw KeyError(string("symbol \"") + c + "\"");
    }
}

list<FuncLine> TuringMachine::sort(list<FuncLine> function) {
    function.sort();
    return function;
}

#ifndef NDEBUG
template<typename T>
void printList(string name, T l) {
    cout << name + ": {";
    auto last = l.end();
    last--;
    for (auto i = l.begin(); i != last; i++) {
        cout << *i << ", ";
    }
    cout << *last;
    cout << "}" << endl;

}

void TuringMachine::print() {
    printList("State set", stateSet);
    printList("Input set", inputSet);
    printList("Tape set", tapeSet);
    cout << "Initial state: " << initState << endl;
    cout << "Blank character: " << blank << endl;
    printList("Final state set", finalStateSet);
    cout << "Tape num: " << tapeNum << endl;
    // for (auto i : function) {
    //     i.print();
    // }
}

void FuncLine::print() const {
    cout << "Old state: " << oldState << endl;
    printList("Old char", oldChar);
    printList("New char", newChar);
    printList("Direction", direction);
    cout << "New state: " << newState << endl;
}
#endif
