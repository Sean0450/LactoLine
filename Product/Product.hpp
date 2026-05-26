#pragma once

#include "GeneralValues/Name.hpp"
#include "GeneralValues/Amount.hpp"

using namespace GeneralValues;
class Product
{
    Name m_name;
    Name m_category;
    Amount m_unitRawMaterials;
    double m_primeCost {0.0};
    double m_epsilon {0.0};
public:
    Product() = default;
    explicit Product(Name&& name,
                     Name&& category, 
                     Amount unitRawMaterials,
                     double primeCost, 
                     double epsilon);
    bool compareOutput(Amount amount, Amount wastedRawMaterials) const;
    double calcPrimeCost(Amount amount) const;
    std::string name() const;
    std::string category() const;
    double unitRowMaterials() const;
    void changeUnitRawMaterials(Amount newUnitMaterials);
    double primeCost() const;
    void changePrimeCost(double newCost);
    double epsilon() const;
    void changeEpsilon(double newEpsilon);
};
