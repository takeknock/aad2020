#pragma once
#include <vector>
#include <memory>

#include "Time.h"
#include "SampleDef.h"
#include "Scenario.hpp"

template <typename T>
class Model {
public:
    virtual void allocate(
        const std::vector<Time>& prdTimeline, 
        const std::vector<SampleDef>& prdDefline) = 0;
    
    virtual void init(
        const std::vector<Time>& prdTimeline,
        const std::vector<SampleDef>& prdDefline) = 0;

    virtual size_t simDim() const = 0;
    
    virtual void generatePath(
        const std::vector<double>& gaussVec,
        Scenario<T>& path) = 0;

    virtual std::unique_ptr<Model<T>> clone() const = 0;

    virtual ~Model(){}

    virtual const std::vector<T*>& parameters() = 0;

    virtual const std::vector<std::string>& parameterLabels() const = 0;

    size_t numParams() const {
        return const_cast<Model*>(this)->parameters().size();
    }

};