#include"ir.h"
#include<iostream>
using namespace std;

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

void TuringMachine::print() {
    printList("State set", stateSet);
    printList("Input set", inputSet);
    printList("Tape set", tapeSet);
    cout << "Initial state: " << initState << endl;
    cout << "Blank character: " << blank << endl;
    printList("Final state set", finalStateSet);
    cout << "Tape num: " << tapeNum << endl;

}