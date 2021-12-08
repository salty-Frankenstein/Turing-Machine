#ifndef UTIL_H
#define UTIL_H

#include<iostream>
#include<cassert>
#include<string>

inline void panic(const std::string& msg) {
    std::cerr << "\033[31mPanic: \033[0m";
    std::cerr << msg << std::endl;
    assert(0);
}

inline void log(const std::string& msg){
    std::cerr << "\033[32mLog: \033[0m";
    std::cerr << msg << std::endl;
}

#endif