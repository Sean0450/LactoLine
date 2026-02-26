#include "Progress.hpp"
#include "Amount.hpp"

#include <stdexcept>

Progress::Progress(Amount && currentResult, Amount&& goal):m_currentResult(std::move(currentResult)),
                                                           m_goal(std::move(goal))
{}

void Progress::changeCurrentResult(Amount newResult)
{
    m_currentResult = newResult;
}

void Progress::changeGoal(Amount goal)
{
    m_goal = goal;
}

double Progress::completed() const
{
    double result {m_currentResult.value() / m_goal.value()};
    if (result > 1.0)
        result = 100.0;
    else
        result *= 100.0;
    return result;
}