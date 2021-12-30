#ifndef UTIL_H
#define UTIL_H

#include<iostream>
#include<cassert>
#include<string>
#include<exception>
#include<algorithm>
#include<list>
#include<vector>

#ifndef NDEBUG
#define PANIC(msg) do{ \
    std::cerr << "\033[31mPanic: \033[0m"; \
    std::cerr << msg << std::endl; \
    assert(0); }while(0);
#else
#define PANIC(msg) do { exit(-1);} while(0);
#endif

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

/* return the digit number of an integer */
inline int digitNum(int i) {
    assert(i >= 0);
    int res = 1;
    while(i >= 10) {
        i /= 10;
        res++;
    }
    return res;
}

/* check if the element is in the given set */
template<typename A>
bool inSet(const A& elem, const std::list<A>& s) {
    return find(s.begin(), s.end(), elem) != s.end();
}

template<typename A>
A& access(std::vector<A>& v, int i) {
    if (i < v.size()) {
        return v[i];
    }
    PANIC("error accessing vector");
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