#pragma once

#include <vector>

#include "Product.hpp"
#include "Model.hpp"
#include "RNG.h"

#include "Scenario.hpp"

#include "functions.hpp"

namespace {
    inline std::vector<std::vector<double>> doSimulation(
        const Product<double>& prd,
        const Model<double>& mdl,
        const RNG& rng,
        const size_t nPath)
    {
        // why are these copied?
        auto cMdl = mdl.clone();
        auto cRnd = rng.clone();

        const size_t nPayoff = prd.payoffLabels().size();
        std::vector<std::vector<double>> results(nPath, std::vector<double>(nPayoff));


        cMdl->allocate(prd.timeline(), prd.defline());
        cMdl->init(prd.timeline(), prd.defline());

        cRnd->init(cMdl->simDim());
        std::vector<double> gaussVec(cMdl->simDim());

        Scenario<double> path;
        ::allocatePath(prd.defline(), path);
        ::initializePath(path);

        for (size_t i = 0; i < nPath; i++)
        {
            cRnd->nextG(gaussVec);
            cMdl->generatePath(gaussVec, path);
            prd.payoffs(path, results[i]);
        }

        return results;
    }
}
