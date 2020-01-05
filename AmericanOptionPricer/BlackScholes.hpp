#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Model.hpp"
#include "Time.h"
#include "SampleDef.h"

template <typename T>
class BlackScholes : public Model<T> {
    T spot;
    T rate;
    T dividend;
    T volatility;
    const bool isSpotMeasure;
    std::vector<Time> timeline;
    bool todayOnTimeline;
    const std::vector<SampleDef>* defline;
    std::vector<T> standartDiviations;
    std::vector<T> drifts;
    // exp((r - d) * (T - t)
    std::vector<std::vector<T>> forwardFactors;
    std::vector<T> numeraires;
    std::vector<std::vector<T>> discounts;
    std::vector<std::vector<T>> libors;
    std::vector<T*> parameters;
    std::vector<std::string> parameterLabels;

public:
    template <typename U>
    BlackScholes(
        const U spot,
        const U vol,
        const bool spotMeasure = false,
        const U rate = U(0.0),
        const U div = U(0.0)
        :spot(spot),
        volatility(vol),
        rate(rate),
        dividend(div),
        isSpotMeasure(spotMeasure),
        parameters(4),
        parameterLabels(4)
    {
        parameterLabels[0] = "spot";
        parameterLabels[1] = "vol";
        parameterLabels[2] = "rate";
        parameterLabels[3] = "dividend";
        setParamPointers();

    }

private:
    void setParamPointers() {
        parameters[0] = &spot;
        parameters[1] = &volatility;
        parameters[2] = &rate;
        parameters[3] = &dividend;
    }
public:
    T spot() const {
        return spot;
    }
    
    const T vol() const {
        return volatility;
    }

    const T rate() const {
        return rate;
    }

    const T div() const {
        return dividend;
    }

    const std::vector<T*>& parameters() override {
        return parameters;
    }

    const std::vector<std::string>& parameterLabels() const override {
        return parameterLabels;
    }

    std::unique_ptr<Model<T>> clone() const override {
        auto clone = std::make_unique<BlackScholes<T>>(*this);
        clone->setParamPointers();
        return clone;
    }

    void allocate(
        const std::vector<Time>& productTimeline,
        const std::vector<SampleDef>& defline) override
    {
        timeline.clear();
        timeline.emplace_back(systemTime);
    }
};
