#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>

#include "Product.hpp"

class ProductTable
{
    std::string m_tableName;
    SQLite::Database m_database;

    std::string wrapString(const std::string& text);

public:
    explicit ProductTable(const std::string& databaseName);
    bool addProduct(const Product& product);
    void updatePrimeCost(const std::string& categoryName, const std::string& productName, double newCost);
    std::vector<Product> getProducts() const;
};
