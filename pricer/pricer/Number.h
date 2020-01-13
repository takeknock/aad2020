#pragma once

#include "Tape.h"
#include "Node.h"


class Number
{
public:
    Number(double v);
    Number(Node* n, double v);
    Number(Node* l, Node* r, double v);

    ~Number();

    // accessor
    static thread_local Tape* tape;

    double v() const;

    Node* node();
    double& lDer();
    double& rDer();

    void aggregateAdjoint();
    void aggregateAdjointTo(Node* target);
    double getDerivative(Number& target);

private:
    double value;
    Node* myNode;

    Node* createNode(size_t childNum);

};

inline Number::Number(double v)
    :value(v)
{
}

inline Number::Number(Node* n, double v)
    :value(v)
{
}

inline Number::Number(Node* l, Node* r, double v)
    :value(v)
{
    myNode = createNode(2);
    myNode->lArgAdjPtr() = &l->myAdjoint;
    myNode->rArgAdjPtr() = &r->myAdjoint;
}

Number::~Number()
{
}

inline double Number::v() const
{
    return value;
}

inline Node* Number::node()
{
    return myNode;
}

inline double& Number::lDer()
{
    return node()->lDer;
}

inline double& Number::rDer()
{
    return node()->rDer;
}

inline void Number::aggregateAdjoint()
{
}

inline void Number::aggregateAdjointTo(Node* target)
{
}

inline double Number::getDerivative(Number& target)
{
    this->aggregateAdjointTo(target.node());
}

inline Node* Number::createNode(size_t childNum)
{
    return tape->recordNode(2);
}
