
#include <iostream>

#include "Node.h"
#include "Number.h"
#include "Tape.h"

Tape global_tape;

thread_local Tape* Number::tape = &global_tape;

Number operator *(Number& lhs, Number& rhs) {
    Number result(lhs.node(), rhs.node(), lhs.v() * rhs.v());

    result.lDer() = rhs.v();
    result.rDer() = lhs.v();

    //result.node()->lDer()->partialDerivatives[0] = rhs.v();

    // how to connect to argument's partial derivatives
    //result.node()->lDer()->myAdjoint = lhs.node();

    return result;
}

int main() {
    
    Number a(1.0);
    Number b(2.0);
    auto y = a * b;
    y.aggregateAdjoint();
    auto dydb = y.getDerivative(b);

    std::cout << y.v() << " " << dydb << "\n";

    return 0;
}