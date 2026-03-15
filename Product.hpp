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
                     Amount&& unitRawMaterials,
                     double primeCost, 
                     double epsilon);
    bool compareOutput(Amount amount, Amount wastedRawMaterials) const;
    double calcPrimeCost(Amount amount) const;
    Name name() const;
    Name category() const;
    Amount unitRowMaterials() const;
    void changeUnitRawMaterials(Amount newUnitMaterials);
    double primeCost() const;
    void changePrimeCost(double newCost);
    double epsilon() const;
    void changeEpsilon(double newEpsilon);
};