#pragma once
#include <vector>

#include "SampleDef.h"
#include "Scenario.hpp"

namespace {
    template <typename T>
    inline void allocatePath(const std::vector<SampleDef>& defline, Scenario<T>& path) {
        path.resize(defline.size());
        for (size_t i = 0; i < defline.size(); i++)
        {
            path[i].allocate(defline[i]);
        }
    }

    template <typename T>
    inline void initializePath(Scenario<T>& path) {
        for (auto& scen : path) {
            scen.initialize();
        }
    }
}

