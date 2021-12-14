#ifndef SHELL_H
#define SHELL_H

#include"parser.h"
#include"interpreter.h"
#include<memory>
#include<string>
#include<list>
#include<vector>

class Shell {
public:
    Shell(int argc, char* argv[]);

    void run();

private:
    void printHelp();

    void runTuring(const std::string& filePath, const std::string& input);

    bool isFlag(const std::string& s);
    
    std::vector<std::string> args;    
    std::list<std::string> flags;
    std::string path;
};

#endif