#ifndef IR_H
#define IR_H

#include<list>
#include<string>

using State = std::string;
using Char = char;

using Direction = char;

/* represents a line of the transfer function */
struct FuncLine {
    FuncLine(const State& oldState,
        const std::list<Char>& oldChar,
        const std::list<Char>& newChar,
        const std::list<Direction>& direction,
        const State& newState
    );

    /* check if the function is well-formed under the tape num 'n' */
    bool isWellFormed(int n) const;

#ifndef NDEBUG
    void print(); //DEBUG
#endif

    const State oldState;
    const std::list<Char> oldChar;
    const std::list<Char> newChar;
    const std::list<Direction> direction;
    const State newState;
};

struct TuringMachine {
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

    /* checking if the machine is well-formed */
    bool isWellFormed() const;

#ifndef NDEBUG
    void print(); //DEBUG
#endif

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