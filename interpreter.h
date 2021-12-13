#ifndef INTERPRETER_H
#define INTERPRETER_H

#include"ir.h"
#include<string>
#include<vector>

class Tape {
public:
    /* representation of a tape, list (index * char) */
    using TapeRep = std::list<std::pair<int, Char>>;

    Tape();

#ifndef NDEBUG
    void print(std::ostream& os);
#endif

    /* get position of head */
    int getHead();

    /* access the cell pointed by head */
    Char& access();

    /* move the head with the given direction */
    void moveHead(Direction d);

    TapeRep showTape();

private:
    /* check if the pointer pointes to a legal position */
    bool check();

    int head;
    
    // represents a tape with two stacks
    std::vector<Char> pos;
    std::vector<Char> neg;
};

class Interpreter {
public:
    Interpreter(const TuringMachine& tm);

    /* execute the given string */
    void execute(const std::string& s);

    /* print the state of the turing machine
     * to a given output stream
     */
    void printState(std::ostream& os);

private:
    /* check if the input is legal,
     * throw an IllegalError exception if not
     */
    void checkLegal(const std::string& s) const;

    void initTape(const std::string& s);

    /* execute single step */
    void singleStep();

    /* perform the transfer function on current state */
    void transfer();

    /* match the current state with a given function line */
    bool match(const FuncLine& fl);

    /* match the tape character
     * with a given character on function line
     */
    bool match(Char c, Char pattern);

    const TuringMachine tm;

    std::vector<Tape> tapes;

    State state;

    int step;

    bool halt;
};

#endif