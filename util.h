#ifndef UTIL_H
#define UTIL_H

#include<iostream>
#include<cassert>

inline void panic(const char* msg) {
    std::cerr << "\033[31mPanic: \033[0m";
    std::cerr << msg << std::endl;
    assert(0);
}

inline void log(const char* msg){
    std::cerr << "\033[32mLog: \033[0m";
    std::cerr << msg << std::endl;
}

#endif