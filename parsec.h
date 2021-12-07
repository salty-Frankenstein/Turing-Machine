/* parsec conbinator */
#ifndef PARSEC_H
#define PARSEC_H

#include"either.h"
#include<functional>
#include<list>
#include<string>

namespace _Parsec
{
    /* the internal representation of string */
    using String = std::list<char>;

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

    /* the parsec combinator */
    template<typename T>
    class Parsec {
    public:
        using aT = std::function<ParseRes<T>(String)>;

        Parsec(aT t) : f(t) {}

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

    Parsec<char> satisfy(std::function<bool(char)> f) {
        return Parsec<char>([=](String s)->ParseRes<char> {
            if (s.empty()) {
                return makeError<char>(
                    "");
            }
            auto i = s.begin();
            auto head = *i;
            i++;
            String res = String(i, s.end());
            if (f(head)) {
                return makeRes<char>({ head, res });
            }
            return makeError<char>(
                std::string("Error parsing with ") + head + "\n");
            }
        );
    }
} // namespace _Parsec



#endif