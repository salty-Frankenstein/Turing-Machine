#include"shell.h"
#include"util.h"
#include"config.h"
#include<iostream>
#include<fstream>
using namespace std;

Shell::Shell(int argc, char* argv[])
    : path(argv[0]) {
    for (int i = 1; i < argc; i++) {
        string s(argv[i]);
        if (isFlag(s)) {
            flags.push_back(s);
        }
        else {
            args.push_back(s);
        }
    }
}

void Shell::run() {
    /* --help */
    if (inSet(string("--help"), flags)
        || inSet(string("-h"), flags)) {
        printHelp();
        return;
    }

    /* --verbose */
    if (inSet(string("--verbose"), flags)
        || inSet(string("-v"), flags)) {
        Mode::mode = VERBOSE;
    }
    else {
        Mode::mode = NORMAL;
    }

    if (args.size() == 2) {
        runTuring(args[0], args[1]);
    }
    else {
        //TODO: error handling
    }
}

void Shell::runTuring(const string& filePath, const string& input) {
    ifstream f(filePath);
    // TODO: error handling
    Parser p;
    auto pcode = p.preprocess(p.readFile(f));
#ifndef NDEBUG
    p.dump(pcode, "input/pdump.tm");
#endif
    Interpreter it(p.parse(pcode));
    it.execute(input);
}
bool Shell::isFlag(const string& s) {
    return s.substr(0, 1) == "-";
}

void Shell::printHelp() {
    cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
}
