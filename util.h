#ifndef UTIL_H
#define UTIL_H

#include<iostream>
#include<cassert>
#include<string>
#include<exception>
#include<algorithm>
#include<list>

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

inline void error(const std::string& msg, int exitCode) {
    std::cerr << msg;
    exit(exitCode);
}

/* check if the element is in the given set */
template<typename A>
static bool inSet(const A& elem, const std::list<A>& s) {
    return find(s.begin(), s.end(), elem) != s.end();
}

struct ParseError : public std::exception {
    ParseError(const std::string& s) : info(s) {}

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

struct IllegalError : public std::exception {
    IllegalError(int n) : position(n) {}
    const char* what() const throw () {
        return "illegal input string";
    }
    int position;   // the position of the error
};

#endif