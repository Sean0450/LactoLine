#include "Amount.hpp"

#include <stdexcept>

namespace GeneralValues 
{
Amount::Amount(double value)
{
    checkAmount(value);
    m_value = value;
}

void Amount::checkAmount(double value)
{
    if (value < s_minAmount)
        throw std::runtime_error("Amount can't be negative");
}

double Amount::value() const
{
    return m_value;
}
}