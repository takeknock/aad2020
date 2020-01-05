
/*
Written by Antoine Savine in 2018

This code is the strict IP of Antoine Savine

License to use and alter this code for personal and commercial applications
is freely granted to any person or company who purchased a copy of the book

Modern Computational Finance: AAD and Parallel Simulations
Antoine Savine
Wiley, 2018

As long as this comment is preserved at the top of the file
*/

#pragma once

//  Implementation of the mrg32k3a RNG,
//  See chapters 5 and 6

#include "RNG.h"
#include "gaussians.h"

class mrg32k3a : public RNG
{

    //	Seed
    const double	myA, myB;

    //  Dimension
    size_t			myDim;

    //  State
    double			myXn, myXn1, myXn2, myYn, myYn1, myYn2;

    //	Antithetic
    bool			myAnti;
    //	false: generate new, true: negate cached
    vector<double>	myCachedUniforms;
    vector<double>	myCachedGaussians;

    //  Constants
    static constexpr  double	m1 = 4294967087;
    static constexpr  double	m2 = 4294944443;
    static constexpr  double	a12 = 1403580;
    static constexpr  double	a13 = 810728;
    static constexpr  double	a21 = 527612;
    static constexpr  double	a23 = 1370589;
    //	We divide the final uniform 
    //		by m1 + 1 so we never hit 1
    static constexpr  double	m1p1 = 4294967088;

    //  Produce next number and update state
    double nextNumber()
    {
        //  Update X
        //	Recursion
        double x = a12 * myXn1 - a13 * myXn2;
        //	Modulus
        x -= long(x / m1) * m1;
        if (x < 0) x += m1;
        //	Update
        myXn2 = myXn1;
        myXn1 = myXn;
        myXn = x;

        //	Same for Y
        double y = a21 * myYn - a23 * myYn2;
        y -= long(y / m2) * m2;
        if (y < 0) y += m2;
        myYn2 = myYn1;
        myYn1 = myYn;
        myYn = y;

        //  Uniform 
        const double u = x > y
            ? (x - y) / m1p1
            : (x - y + m1) / m1p1;
        return u;
    }

public:

    //  Constructor with seed
    mrg32k3a(const unsigned a = 12345, const unsigned b = 12346) :
        myA(a), myB(b)
    {
        reset();
    }

    //	Reset state to 0 (seed)
    void reset()
    {
        //	Reset state
        myXn = myXn1 = myXn2 = myA;
        myYn = myYn1 = myYn2 = myB;

        //	Anti = false: generate next
        myAnti = false;
    }

    //  Virtual copy constructor
    unique_ptr<RNG> clone() const override
    {
        return make_unique<mrg32k3a>(*this);
    }

    //  Initializer 
    void init(const size_t simDim) override
    {
        myDim = simDim;
        myCachedUniforms.resize(myDim);
        myCachedGaussians.resize(myDim);
    }

    void nextU(vector<double> & uVec) override
    {
        if (myAnti)
        {
            //	Do not generate, negate cached
            transform(
                myCachedUniforms.begin(),
                myCachedUniforms.end(),
                uVec.begin(),
                [](const double d) { return 1.0 - d; });

            //	Generate next
            myAnti = false;
        }
        else
        {
            //	Generate and cache
            generate(
                myCachedUniforms.begin(),
                myCachedUniforms.end(),
                [this]() { return nextNumber(); });

            //	Copy
            copy(
                myCachedUniforms.begin(),
                myCachedUniforms.end(),
                uVec.begin());

            //	Do not generate next
            myAnti = true;
        }
    }

    void nextG(vector<double> & gaussVec) override
    {
        if (myAnti)
        {
            //	Do not generate, negate cached
            //	Note: we reuse the Gaussian numbers,
            //		we save not only generation, but also
            //		Gaussian transaformation
            transform(
                myCachedGaussians.begin(),
                myCachedGaussians.end(),
                gaussVec.begin(),
                [](const double n) { return -n; });

            //	Generate next
            myAnti = false;
        }
        else
        {
            //	Generate and cache
            generate(
                myCachedGaussians.begin(),
                myCachedGaussians.end(),
                [this]() { return invNormalCdf(nextNumber()); });

            //	Copy
            copy(
                myCachedGaussians.begin(),
                myCachedGaussians.end(),
                gaussVec.begin());

            //	Do not generate next
            myAnti = true;
        }
    }
};