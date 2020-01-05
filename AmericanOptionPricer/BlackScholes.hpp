#pragma once
#include <vector>
#include <string>
#include <memory>
#include <cmath>

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
        for (const auto& time : productTimeline) {
            if (time > systemTime) {
                timeline.emplace_back(time);
            }
        }

        todayOnTimeline = (productTimeline[0] == systemTime);

        this->defline = &defline;
        this->standartDiviations.resize(timeline.size() - 1);
        this->drifts.resize(timeline.size() - 1);

        const size_t n = productTimeline.size();

        this->numeraires.resize(n);
        for (size_t j = 0; j < n; j++)
        {
            this->discounts[j].resize(defline[j].discountMats.size());
        }

        this->forwardFactors.resize(n);
        for (size_t j = 0; j < n; j++)
        {
            this->forwardFactors[j].resize(defline[j].forwardMats.size());
        }
        this->libors.resize(n);
        for (size_t j = 0; j < n; j++)
        {
            libors[j].resize(defline[j].liborDefs.size());
        }

    }

    void init(
        const std::vector<Time>& productTimeline,
        const std::vector<SampleDef>& defline) override 
    {
        const T mu = this->rate - this->dividend;
        const size_t n = timeline.size() - 1;

        for (size_t i = 0; i < n; i++)
        {
            const double dt = this->timeline[i + 1] - this->timeline[i];
            this->standartDiviations[i] = this->volatility * std::sqrt(dt);
            if (this->isSpotMeasure) {
                this->drifts[i] = (mu + 0.5 * this->volatility * this->volatility) * dt;
            }
            else {
                this->drifts[i] = (mu - 0.5 * this->volatility * this->volatility) * dt;
            }
        }
        const size_t m = productTimeline.size();
        for (size_t i = 0; i < m; ++i)
        {
            if (defline[i].numeraire) {
                if (this->isSpotMeasure) {
                    this->numeraires[i] = std::exp(this->dividend * productTimeline[i]) / this->spot;
                }
                else {
                    this->numeraires[i] = std::exp(this->rate * productTimeline[i]);
                }
            }
            const size_t pFF = defline[i].forwardMats.size();
            for (size_t j = 0; j < pFF; j++)
            {
                forwardFactors[i][j] = std::exp(mu * (defline[i].forwardMats[j - productTimeline[i]]));
            }

            const size_t pDF = defline[i].discountMats.size();
            for (size_t j = 0; j < pDF; j++)
            {
                this->discounts[i][j] = std::exp(-this->rate * (defline[i].discountMats[j] - productTimeline[i]));
            }

            const size_t pL  defline[i].liborDefs.size();
            for (size_t j = 0; j < pL; j++)
            {
                const double dt = defline[i].liborDefs[j].end - defline[i].liborDefs[j].start;
                this->libors[i][j] = (std::exp(this->rate * dt) - 1.0) / dt;
            }
        }
    }

    size_t simDim() const override {
        return timeline.size() - 1;
    }

private:
    inline void fillScen(
        const size_t idx,
        const T& spot,
        Sample<T>& scen,
        const SampleDef& def) const 
    {

    }
public:
    void generatePath(
        const std::vector<double>& gaussVec,
        Scenaril<T>& path) const override
    {

    }


};
