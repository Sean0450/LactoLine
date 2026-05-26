#pragma once

#include <string>

struct ProductData
{
    std::string productName;
    std::string categoryName;
    double unitRawMaterials {0.0};
    double primeCost        {0.0};
    double epsilon          {0.0};
public:
    explicit ProductData() = default;
    explicit ProductData(const std::string& productName,
                         const std::string& categoryName,
                         double unitRawMaterials,
                         double primeCost,
                         double epsilon): productName(productName),
                                          categoryName(categoryName),
                                          unitRawMaterials(unitRawMaterials),
                                          primeCost(primeCost),
                                          epsilon(epsilon)
    {
    }
};

