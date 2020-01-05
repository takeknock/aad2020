#pragma once
#include <vector>
#include <memory>

class RNG {
public:
    virtual void init(const size_t simDim) = 0;
    virtual void nextU(std::vector<double>& uVec) = 0;
    virtual void nextG(std::vector<double>& gaussVec) = 0;
    virtual std::unique_ptr<RNG> clone() const = 0;
    virtual ~RNG(){}
    virtual size_t simDim() const = 0;
};