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
    if (value < 0.0)
        throw std::runtime_error("amount can't be negative");
}

void Amount::changeAmount(double value)
{
    checkAmount(value);
    m_value = value;
}

double Amount::value() const
{
    return m_value;
}
}