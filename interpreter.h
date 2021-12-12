#ifndef INTERPRETER_H
#define INTERPRETER_H

#include"ir.h"
#include<string>

class Interpreter {
public:
    Interpreter(const TuringMachine& tm);

    /* execute the given string */
    void execute(const std::string& s) const;

private:
    /* check if the input is legal,
     * throw an IllegalError exception if not
     */
    void checkLegal(const std::string& s) const;

    const TuringMachine tm;
};

#endif