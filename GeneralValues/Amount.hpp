#pragma once

namespace GeneralValues
{
class Amount
{
    static constexpr double s_minAmount {0.001};
    double m_value {0.0};
    void checkAmount(double value);
public:
    Amount() = default;
    explicit Amount(double value);
    double value() const;
    auto operator<=>(const Amount& amount) const = default;
};
}