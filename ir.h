#ifndef IR_H
#define IR_H

#include<list>
#include<string>

using State = std::string;
using Char = char;

enum Direction { LEFT, RIGHT, STAR };

/* represents a line of the transfer function */
struct FuncLine {
    FuncLine(const State& oldState,
        const std::list<Char>& oldChar,
        const std::list<Char>& newChar,
        const std::list<Direction>& direction,
        const State& newState
    );

    const State oldState;
    const std::list<Char> oldChar;
    const std::list<Char> newChar;
    const std::list<Direction> direction;
    const State newState;
};

class TuringMachine {
public:
    TuringMachine(
        const std::list<State>& stateSet,
        const std::list<Char>& inputSet,
        const std::list<Char>& tapeSet,
        const State& initState,
        const Char& blank,
        const std::list<State>& finalStateSet,
        const int& tapeNum,
        const std::list<FuncLine>& function
    );

    void print(); //DEBUG

private:
    const std::list<State> stateSet;
    const std::list<Char> inputSet;
    const std::list<Char> tapeSet;
    const State initState;
    const Char blank;
    const std::list<State> finalStateSet;
    const int tapeNum;
    const std::list<FuncLine> function;
};


#endif