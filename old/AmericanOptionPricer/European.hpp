#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "Product.hpp"
#include "Time.h"
#include "SampleDef.h"

template <typename T>
class European : public Product<T>
{
    double strike;
    Time exerciseDate;
    Time settlementDate;

    std::vector<Time> timeline;
    std::vector<SampleDef> defline;
    std::vector<std::string> labels;

public:
    European(
        const double strike,
        const Time exerciseDate,
        const Time settlementDate)
        :strike(strike), exerciseDate(exerciseDate), settlementDate(settlementDate), labels(1)
    {
        timeline.emplace_back(exerciseDate);
        defline.resize(1);
        defline[0].numeraire = true;
        defline[0].forwardMats.emplace_back(settlementDate);
        defline[0].discountMats.emplace_back(settlementDate);

        std::ostringstream ost;
        ost.precision(2);

        if (settlementDate == exerciseDate) {
            ost << "call " << strike << " "
                << exerciseDate;
        }
        else {
            ost << "call " << strike << " "
                << exerciseDate << " " << settlementDate;
        }
        labels[0] = ost.str();
    }

    European(
        const double strike,
        const Time exerciseDate)
        :European(strike, exerciseDate, exerciseDate)
    {}

    std::unique_ptr<Product<T>> clone() {
        return std::make_unique<European<T>>(*this);
    }

    const std::vector<Time>& timeline() const override {
        return timeline;
    }

    const std::vector<SampleDef>& defline() const override {
        return defline;
    }

    const std::vector<std::string>& payoffLabels() const override {
        return labels;
    }

    void payoffs(
        const Scenario<T>& path,
        std::vector<T>& payoffs) const override
    {
        payoffs[0] = std::max_align_t(path[0].forwards[0] - strike, 0.0)
            * path[0].discounts[0]
            / path[0].numeraire;
    }
};