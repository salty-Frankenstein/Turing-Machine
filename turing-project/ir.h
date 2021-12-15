#ifndef IR_H
#define IR_H

#include<list>
#include<vector>
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
    bool isWellFormed(size_t n) const;

    /* order of the pattern of the function, for sorting */
    bool operator<(const FuncLine& f);

#ifndef NDEBUG
    void print(); //DEBUG
#endif

    const State oldState;
    const std::vector<Char> oldChar;
    const std::vector<Char> newChar;
    const std::vector<Direction> direction;
    const State newState;
};

     /* Spec:
     * > all patterns of every function line is sorted
     */
struct TuringMachine {
    TuringMachine(
        const std::list<State>& stateSet,
        const std::list<Char>& inputSet,
        const std::list<Char>& tapeSet,
        const State& initState,
        const Char& blank,
        const std::list<State>& finalStateSet,
        const size_t& tapeNum,
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
    const size_t tapeNum;
    const std::list<FuncLine> function;
    
private:
    std::list<FuncLine> sort(std::list<FuncLine> function);
};


#endif