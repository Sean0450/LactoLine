#pragma once

#include <tuple>

#include "FundamentalTypes.hpp"

namespace GeneralValues
{
class Progress
{
    static constexpr double s_minGoalValue {1.0};
    Amount m_currentResult;
    Amount m_goal;
    Amount m_wastedRawMaterials;
    void checkAmount(Amount amountValue) const;
    void checkGoal(Amount newGoal);
public:
    Progress() = default;
    explicit Progress(Amount currentResult, Amount goal, Amount wastedRawMaterials);
    void changeCurrentResult(Amount newProductAmount, Amount wastedRawMaterials);
    void changeGoal(Amount goal);
    std::tuple<Amount, Amount, Amount> getAmountData() const;
    bool isCompleted() const;
};
}
