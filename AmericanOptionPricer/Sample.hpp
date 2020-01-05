#pragma once
#include <vector>
#include "SampleDef.h"

template<typename T>
struct Sample {
    T numeraire;
    std::vector<T> forwards;
    std::vector<T> discounts;
    std::vector<T> libors;

    void allocate(const SampleDef& data) {
        forwards.resize(data.forwardMats.size());
        discounts.resize(data.discountMats.size());
        libors.resize(data.liborDefs.size());
    }

    void initialize() {
        numeraire = T(1.0);
        fill(forwards.begin(), forwards.end(), T(100.0));
        fill(discounts.begin(), discounts.end(), T(1.0));
        fill(libors.begin(), libors.end(), T(0.0));
    }
};
