#pragma once

class Node
{
public:
    Node();
    ~Node();

    double myAdjoint;
    double* partialDerivatives;

    double* lDer() const;
    double* rDer() const;
    double* lArgAdjPtr();
    double* rArgAdjPtr();

private:
    double* lDerivative;
    double* rDerivative;
    double* lArgAdj;
    double* rArgAdj;
};

Node::Node()
{
}

Node::~Node()
{
}

inline double* Node::lDer() const
{
    return lDerivative;
}

inline double* Node::rDer() const
{
    return rDerivative;
}

inline double* Node::lArgAdjPtr()
{
    return lArgAdj;
}

inline double* Node::rArgAdjPtr()
{
    return rArgAdj;
}

