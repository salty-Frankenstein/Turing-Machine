#include"either.h"
#include"util.h"
#include<cassert>

void testEither() {
    log("start testing either:");
    Either<int, bool> l1 = Left<int, bool>(1),
        l2 = l1,
        r1 = Right<int, bool>(false),
        r2 = r1;
    assert(l1->isLeft() && l2->isLeft());
    assert(!r1->isLeft() && !r2->isLeft());
    assert(!l1->isRight() && !l2->isRight());
    assert(r1->isRight() && r1->isRight());
    assert(l1->getLeft() == l2->getLeft());
    assert(r1->getRight() == r2->getRight());
    std::cout << l1->getLeft() << std::endl;
    // assert(l1->getRight() == l2->getRight());
    // assert(r1->getLeft() == r2->getLeft());
    
    Either<int, Either<int, bool>> 
        l3 = Left<int, Either<int, bool>>(1),
        r3 = Right<int, Either<int, bool>>(r2);
    std::cout << l3->getLeft() << std::endl;
    std::cout << r3->getRight()->getRight() << std::endl;
    log("test either finished.");
}

int main() {
    testEither();
    return 0;
}