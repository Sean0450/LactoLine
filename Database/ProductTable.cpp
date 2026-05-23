#include "ProductTable.hpp"

#include <format>

namespace requests
{
static constexpr auto* createTable {"CREATE TABLE IF NOT EXISTS Product ("
                                    "Name TEXT PRIMARY KEY, "
                                    "CategoryName TEXT, "
                                    "UnitRowMaterials REAL,"
                                    "PrimeCost REAL,"
                                    "Epsilon REAL)"};
static constexpr auto* addProduct {"INSERT INTO Product VALUES ('{}', '{}', {}, {}, {})"};
static constexpr auto* updateProductData {"UPDATE Product SET 'PrimeCost' = {} WHERE Name = '{}' AND CategoryName = '{}'"};
static constexpr auto* selectProducts {"SELECT * FROM Product"};
}

ProductTable::ProductTable(const std::string& databaseName):m_tableName(databaseName), m_database(m_tableName, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)
{
    try
    {
        m_database.exec(requests::createTable);
    }
    catch(std::exception& exc)
    {
    }
}

bool ProductTable::addProduct(const Product& product)
{
    bool result {true};
    try
    {
        m_database.exec(std::format(requests::addProduct,
                                    product.name(),
                                    product.category(),
                                    product.unitRowMaterials(),
                                    product.primeCost(),
                                    product.epsilon()));
    }
    catch (std::exception& exc)
    {
        result = false;
    }
    return result;
}

void ProductTable::updatePrimeCost(const std::string& categoryName, const std::string& productName, double newCost)
{
    try
    {
        m_database.exec(std::format(requests::updateProductData, newCost, productName, categoryName));
    }
    catch(std::exception& exc)
    {
    }
}

std::vector<Product> ProductTable::getProducts() const
{
    std::vector<Product> result;
    try
    {
        SQLite::Statement query(m_database, requests::selectProducts);
        while (query.executeStep())
        {
            const std::string name = query.getColumn(0);
            const std::string categoryName = query.getColumn(1);
            const double unitRawMaterials = query.getColumn(2);
            const double primeCost = query.getColumn(3);
            const double epsilon = query.getColumn(4);
            Product product {Name(name), Name(categoryName), Amount(unitRawMaterials), primeCost, epsilon};
            result.emplace_back(std::move(product));
        }
    }
    catch(std::exception& exc)
    {
    }
    return result;

}
