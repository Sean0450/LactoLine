#pragma once

#include "Amount.hpp"

namespace GeneralValues
{
class Progress
{
    Amount m_currentResult;
    Amount m_goal;
public:
    Progress() = default;
    explicit Progress(Amount&& currentResult, Amount&& goal);
    void changeCurrentResult(Amount newResult);
    void changeGoal(Amount goal);
    double completed() const;
};
}
