#pragma once
#include <vector>
#include <string>

#include "Time.h"

// definition of what data must be simulated.
struct SampleDef {
    bool numeraire = true;
    std::vector<Time> forwardMats;
    std::vector<Time> discountMats;

    struct RateDef {
        Time start;
        Time end;
        std::string curve;

        RateDef(const Time s, const Time e, const std::string& c)
            :start(s), end(e), curve(c) {};
    };

    std::vector<RateDef> liborDefs;

};
