#include "Progress.hpp"
#include "FundamentalTypes.hpp"

#include <stdexcept>

namespace GeneralValues
{
Progress::Progress(Amount currentResult, Amount goal, Amount wastedRawMaterials)
{
    checkAmount(currentResult);
    m_currentResult = currentResult;
    checkGoal(goal);
    m_goal = goal;
    checkAmount(wastedRawMaterials);
    m_wastedRawMaterials = wastedRawMaterials;
}

void Progress::checkAmount(Amount amountValue) const
{
    if (amountValue < 0.0)
        throw std::runtime_error("Количество не может быть отрицательным числом");
}

void Progress::checkGoal(Amount newGoal)
{
    if (newGoal < s_minGoalValue)
        throw std::runtime_error("Целевое количество продукции не может быть меньше 1");
}

void Progress::changeCurrentResult(Amount newResult, Amount wastedRawMaterials)
{
    checkAmount(newResult);
    checkAmount(wastedRawMaterials);
    if (newResult + m_currentResult <= m_goal)
    {
        m_currentResult += newResult;
        m_wastedRawMaterials += wastedRawMaterials;
    }
}

void Progress::changeGoal(Amount goal)
{
    checkGoal(goal);
    m_goal = goal;
}

std::tuple<Amount, Amount, Amount> Progress::getAmountData() const
{
    return {m_currentResult, m_goal, m_wastedRawMaterials};
}

bool Progress::isCompleted() const
{
    double result {m_currentResult / m_goal};
    return result >= 1.0;
}
}
