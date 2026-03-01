#include "Progress.hpp"
#include "Amount.hpp"

#include <stdexcept>

namespace GeneralValues
{
Progress::Progress(Amount&& currentResult, Amount&& goal):m_currentResult(std::move(currentResult))
{
    checkGoal(goal);
    m_goal = std::move(goal);
}

void Progress::checkGoal(Amount goal) const
{
    if (goal.value() < s_minGoalValue)
        throw std::runtime_error("Error goal value");
}

void Progress::changeCurrentResult(Amount newResult)
{
    m_currentResult = newResult;
}

void Progress::changeGoal(Amount goal)
{
    checkGoal(goal);
    m_goal = goal;
}

double Progress::completed() const
{
    checkGoal(m_goal);
    double result {m_currentResult.value() / m_goal.value()};
    if (result > 1.0)
        result = 100.0;
    else
        result *= 100.0;
    return result;
}
}
