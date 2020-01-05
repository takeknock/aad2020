#pragma once
#include <vector>

#include "Time.h"
#include "SampleDef.h"

template <typename T>
class Model {
public:
    virtual void allocate(
        const std::vector<Time>& prdTimeline, 
        const vector<SampleDef>& prdDefline) = 0;
    
    virtual void init(
        const std::vector<Time>& prdTimeline,
        const std::vector<SampleDef>& prdDefline) = 0;


};