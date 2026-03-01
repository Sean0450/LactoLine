#pragma once

namespace GeneralValues
{
class Amount
{
    double m_value {0.0};
    void checkAmount(double value);
public:
    Amount() = default;
    explicit Amount(double value);
    void changeAmount(double value);
    double value() const;
    auto operator<=>(const Amount& amount) const = default;
};
}