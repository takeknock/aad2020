#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Time.h"
#include "SampleDef.h"
#include "Scenario.hpp"

template <typename T>
class Product {
public:
    virtual const std::vector<Time>& timeline() const = 0;
    virtual const std::vector<SampleDef>& defline() const = 0;
    virtual const std::vector<std::string>& payoffLabels() const = 0;
    virtual void payoffs(const Scenario<T>& path, std::vector<T>& payoffs) const = 0;
    virtual std::unique_ptr<Product<T>> clone() const = 0;
    virtual ~Product(){}
};
