#pragma once

#include "Amount.hpp"

namespace GeneralValues
{
class Progress
{
    static constexpr double s_minGoalValue {1.0};
    Amount m_currentResult;
    Amount m_goal;
    void checkGoal(Amount goal) const;
public:
    Progress() = default;
    explicit Progress(Amount&& currentResult, Amount&& goal);
    void changeCurrentResult(Amount newResult);
    void changeGoal(Amount goal);
    double completed() const;
};
}
