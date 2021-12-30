/* the either datatype */
#ifndef EITHER_H
#define EITHER_H

#include"util.h"
#include<memory>

/* private namespace _Either */
namespace _Either
{
    template<typename L, typename R>
    class Either {
    public:
        virtual L getLeft() = 0;

        virtual R getRight() = 0;

        virtual bool isLeft() = 0;

        virtual bool isRight() = 0;

    };

    template<typename L, typename R>
    class Left : public Either<L, R> {
    public:
        Left(const L& t) : val(t) {}

        L getLeft() { return val; }

        R getRight() {
            PANIC("Trying to get Right value from Left");
            return {};
        }

        bool isLeft() { return true; }

        bool isRight() { return !isLeft(); }

    private:
        const L val;
    };

    template<typename L, typename R>
    class Right : public Either<L, R> {
    public:
        Right(const R& t) : val(t) {}

        R getRight() { return val; }

        L getLeft() {
            PANIC("Trying to get Left value from Right");
            return {};
        }

        bool isLeft() { return false; }

        bool isRight() { return !isLeft(); }

    private:
        const R val;
    };
} // namespace _Either   

/* exported methods */

template<typename L, typename R>
using Either = std::shared_ptr<_Either::Either<L, R>>;

template<typename L, typename R>
Either<L, R> Left(const L& v){
    return std::make_shared<_Either::Left<L, R>>(v);
}

template<typename L, typename R>
Either<L, R> Right(const R& v){
    return std::make_shared<_Either::Right<L, R>>(v);
}

#endif