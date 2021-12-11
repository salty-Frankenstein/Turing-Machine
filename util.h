#ifndef UTIL_H
#define UTIL_H

#include<iostream>
#include<cassert>
#include<string>
#include<exception>

inline void panic(const std::string& msg) {
#ifndef NDEBUG
    std::cerr << "\033[31mPanic: \033[0m";
    std::cerr << msg << std::endl;
#endif
    assert(0);
}

inline void log(const std::string& msg) {
#ifndef NDEBUG
    std::cerr << "\033[32mLog: \033[0m";
    std::cerr << msg << std::endl;
#endif
}

struct ParseError : public std::exception {
    ParseError(const std::string& s) {}

    const char* what() const throw () {
        return info.c_str();
    }

    std::string info;
};

struct EOF_Error : public std::exception {
    const char* what() const throw () {
        return "eof error";
    }
};

#endif