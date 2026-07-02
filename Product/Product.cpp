#include "Product.hpp"

#include <stdexcept>
#include <cmath>

Product::Product(const ProductData& data):m_name(data.productName),
                                          m_category(data.categoryName),
                                          m_unitRawMaterials(data.unitRawMaterials)
{
    if (isZeroOrNegative(data.primeCost) || isZeroOrNegative(data.epsilon))
        throw std::runtime_error("Некорректные значения");
    m_primeCost = data.primeCost;
    m_epsilon = data.epsilon;
}

 bool Product::compareOutput(Amount amount, Amount wastedRawMaterials) const
 {
     if (isZeroOrNegative(m_unitRawMaterials))
        throw std::runtime_error("Количество сырья на единицу продукции невалидно");
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

ProductData Product::getProductData() const
{
    return ProductData(m_name, m_category, m_unitRawMaterials, m_primeCost, m_epsilon);
}

Name Product::name() const
{
    return m_name;
}

void Product::changeUnitRawMaterials(Amount newUnitMaterials)
{
    if (isZeroOrNegative(newUnitMaterials) <= 0.0)
        throw std::runtime_error("Количество сырья на еденицу продукции не может быть меньше или равно нулю");
    m_unitRawMaterials = newUnitMaterials;
}

void Product::changePrimeCost(double newPrimeCost)
{
    if (isZeroOrNegative(newPrimeCost) <= 0.0)
        throw std::runtime_error("Себестоимость не может быть меньше равна нулю");
    m_primeCost = newPrimeCost;
}

void Product::changeEpsilon(double newEpsiolon)
{
    if (isZeroOrNegative(newEpsiolon) <= 0.0)
        throw std::runtime_error("Погрешность измерений не может быть меньше равна нулю");
    m_epsilon = newEpsiolon;
}

bool Product::isZeroOrNegative(double data) const
{
    return data <= 0.0;
}

std::optional<Product> Product::fromDataToProduct(const ProductData& data)
{
    std::optional<Product> result;
    try
    {
        result = Product(data);
    }
    catch(std::exception& exc)
    {
        result = std::nullopt;
    }
    return result;
}
