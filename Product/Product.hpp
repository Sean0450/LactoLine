#pragma once

#include "GeneralValues/FundamentalTypes.hpp"
#include "ProductData.hpp"

using namespace GeneralValues;
class Product
{
    Name m_name;
    Name m_category;
    Amount m_unitRawMaterials;
    double m_primeCost {0.0};
    double m_epsilon {0.0};

    bool isZeroOrNegative(double data) const;
public:
    Product() = default;
    explicit Product(const ProductData& productData);
    bool compareOutput(Amount amount, Amount wastedRawMaterials) const;
    double calcPrimeCost(Amount amount) const;
    ProductData getProductData() const;
    Name name() const;
    void changeUnitRawMaterials(Amount newUnitMaterials);
    void changePrimeCost(double newCost);
    void changeEpsilon(double newEpsilon);
};
