/* parsec conbinator */
#ifndef PARSEC_H
#define PARSEC_H

#include"either.h"
#include<functional>
#include<list>
#include<string>

/* the internal representation of string */
using String = std::list<char>;

/* String to std::string */
std::string tostring(const String& s);

/* show String */
std::string show(const String& s);

/* std::string to String */
String toString(const std::string& s);

/* the error message of the parsing process */
using Msg = std::string;

/* type of parsing result */
template<typename T>
using Res = std::pair<T, String>;

template<typename T>
using ParseRes = Either<Msg, Res<T>>;


template<typename T>
ParseRes<T> makeRes(const Res<T>& r) {
    return Right<Msg, Res<T>>(r);
}

template<typename T>
ParseRes<T> makeError(const Msg& m) {
    return Left<Msg, Res<T>>(m);
}

/* get the error message from a left value */
template<typename T>
std::string showError(const ParseRes<T> err) {
    assert(err->isLeft());
    return err->getLeft();
}

/* the parsec combinator */
template<typename T>
class Parsec {
public:
    using aT = std::function<ParseRes<T>(String)>;

    Parsec(aT t) : f(t) {}

    ParseRes<T> operator()(std::string s) const {
        return f(toString(s));
    }

    ParseRes<T> operator()(String s) const {
        return f(s);
    }

    template<typename A, typename B>
    friend Parsec<B> operator>>(Parsec<A> p1, Parsec<B> p2);

    template<typename A, typename B>
    friend Parsec<A> operator<<(Parsec<A> p1, Parsec<B> p2);

    template<typename A, typename B>
    friend Parsec<B> operator*(Parsec<std::function<B(A)>> pf, Parsec<A> p);

private:
    aT f;
};

Parsec<char> satisfy(std::function<bool(char)> f);

template<typename A>
Parsec<A> pure(const A a) {
    return Parsec<A>([=](String s) ->ParseRes<A> {
        return makeRes<A>({ a, s });});
}

template<typename A, typename B>
Parsec<B> operator*(Parsec<std::function<B(A)>> pf, Parsec<A> p) {
    return Parsec<B>([=](String str)->ParseRes<B> {
        auto t1 = pf(str);
        if (t1->isRight()) {
            auto res1 = t1->getRight();
            auto f = res1.first;
            auto str2 = res1.second;
            auto t2 = p(str2);
            if (t2->isRight()) {
                auto res2 = t2->getRight();
                return makeRes<B>({ f(res2.first), res2.second });
            }
            assert(t2->isLeft());//TODO: assertion
            return makeError<B>(showError(t2) + 
                ",\nwhen parsing " + show(str2) + ", trying to parse: " + show(str));
        }
        assert(t1->isLeft());//TODO: assertion
        return makeError<B>(showError(t1) + ",\nwhen parsing " + show(str));
        }
    );
}

template<typename A>
Res<std::list<A>> many_(Parsec<A> p, String str) {
    auto t = p(str);
    if (t->isRight()) {
        auto res1 = t->getRight();
        auto v = res1.first;
        auto s = res1.second;
        auto res2 = many_(p, s);
        auto vs = res2.first;
        auto s_ = res2.second;
        vs.push_front(v);
        return { vs, s_ };
    }
    return { std::list<A>(), str };
}

template<typename A>
Parsec<std::list<A>> many(Parsec<A> p) {
    return Parsec<std::list<A>>([=](String str)->ParseRes<std::list<A>> {
        return makeRes<std::list<A>>(many_(p, str));});
}

template<typename A, typename B>
Parsec<B> operator>>(Parsec<A> p1, Parsec<B> p2) {
    auto p = pure(std::function<std::function<B(B)>(A)>(
        [](A a)->std::function<B(B)> { return [](B b)->B { return b; };}));
    return p * p1 * p2;
}

template<typename A, typename B>
Parsec<A> operator<<(Parsec<A> p1, Parsec<B> p2) {
    auto p = pure(std::function<std::function<A(B)>(A)>(
        [](A a)->std::function<A(B)> { return [=](B b)->A { return a; };}));
    return p * p1 * p2;
}

#endif