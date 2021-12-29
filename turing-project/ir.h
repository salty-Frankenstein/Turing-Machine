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

    std::string repr() const;

#ifndef NDEBUG
    void print() const; //DEBUG
#endif

    const State oldState;
    const std::vector<Char> oldChar;
    const std::vector<Char> newChar;
    const std::vector<Direction> direction;
    const State newState;
};

     /* Spec:
     * > all patterns of every function line is sorted
     * > all symbols and states in the function have been declared
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

    /* checking if the machine is well-formed, 
     * throw an IllFormedError, or a KeyError if not */
    void checkWellFormed() const;

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
    /* check if the element is in the given set, throw an KeyError if not */
    void checkState(const State& s) const;
    void checkTapeSymbol(const Char) const;
    std::list<FuncLine> sort(std::list<FuncLine> function);
};

struct KeyError : public std::exception {
    KeyError(const std::string& s) : info(s) {}

    const char* what() const throw () {
        return info.c_str();
    }

    std::string info;
};

struct IllFormedError : public std::exception {
    IllFormedError(const std::string& s, const FuncLine& _f) 
     : info(s), f(_f) {}

    const char* what() const throw () {
        return info.c_str();
    }

    const std::string info;
    const FuncLine f;
};

#endif