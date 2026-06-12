#include "Product.hpp"

#include <stdexcept>
#include <cmath>

Product::Product(Name&& name,
                 Name&& category, 
                 Amount unitRawMaterials,
                 double primeCost, 
                 double epsilon): m_name(std::move(name)),
                                  m_category(std::move(category)),
                                  m_unitRawMaterials(unitRawMaterials)
{
    if (primeCost <= 0.0 || epsilon <= 0.0)
        throw std::runtime_error("Incorrect measurements");
    m_primeCost = primeCost;
    m_epsilon = epsilon;
}

 bool Product::compareOutput(Amount amount, Amount wastedRawMaterials) const
 {
    if (m_unitRawMaterials <= 0.0 || m_epsilon <= 0)
        throw std::runtime_error("Product object is not valid");
    bool result {true};
    Amount totalWeight{amount * m_unitRawMaterials};
    if (std::abs(totalWeight - wastedRawMaterials) > m_epsilon * wastedRawMaterials)
        result = false;
    return result;
 }

 double Product::calcPrimeCost(Amount amount) const
 {
    return std::trunc(amount * m_primeCost * 100) / 100.0;
 }

 std::string Product::name() const
{
    return m_name;
}

std::string Product::category() const
{
    return m_category;
}

double Product::unitRowMaterials() const
{
    return m_unitRawMaterials;
}

void Product::changeUnitRawMaterials(Amount newUnitMaterials)
{
    m_unitRawMaterials = newUnitMaterials;
}

double Product::primeCost() const
{
    return m_primeCost;
}

void Product::changePrimeCost(double newPrimeCost)
{
    if (newPrimeCost <= 0.0)
        throw std::runtime_error("Prime cost can't be zero or negative");
    m_primeCost = newPrimeCost;
}

double Product::epsilon() const
{
    return m_epsilon;
}

void Product::changeEpsilon(double newEpsiolon)
{
    if (newEpsiolon <= 0.0)
        throw std::runtime_error("Epsilon can't be zero or negative");
    m_epsilon = newEpsiolon;
}
