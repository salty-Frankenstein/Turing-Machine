/* intermediate representation */
#include"ir.h"
#include<iostream>
#include<numeric>
using namespace std;


FuncLine::FuncLine(const State& _oldState,
    const std::list<Char>& _oldChar,
    const std::list<Char>& _newChar,
    const std::list<Direction>& _direction,
    const State& _newState
) :
    oldState(_oldState),
    oldChar(_oldChar),
    newChar(_newChar),
    direction(_direction),
    newState(_newState) {
}

bool FuncLine::isWellFormed(int n) const {
    return oldChar.size() == n 
        && newChar.size() == n
        && direction.size() == n;
}

TuringMachine::TuringMachine(
    const std::list<State>& _stateSet,
    const std::list<Char>& _inputSet,
    const std::list<Char>& _tapeSet,
    const State& _initState,
    const Char& _blank,
    const std::list<State>& _finalStateSet,
    const int& _tapeNum,
    const std::list<FuncLine>& _function
) :
    stateSet(_stateSet),
    inputSet(_inputSet),
    tapeSet(_tapeSet),
    initState(_initState),
    blank(_blank),
    finalStateSet(_finalStateSet),
    tapeNum(_tapeNum),
    function(_function) {
}

bool TuringMachine::isWellFormed() const {
    int n = tapeNum;
    return accumulate(function.begin(), function.end(), true,
    [=](bool a, const FuncLine& b){ 
        return a && b.isWellFormed(n); });
}

#ifndef NDEBUG
template<typename T>
void printList(string name, list<T> l) {
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
    for (auto i : function) {
        i.print();
    }
}

void FuncLine::print() {
    cout << "Old state: " << oldState << endl;
    printList("Old char", oldChar);
    printList("New char", newChar);
    printList("Direction", direction);
    cout << "New state: " << newState << endl;
}
#endif
