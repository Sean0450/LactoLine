#pragma once

#include <format>

#include "BaseTable.hpp"
#include "Commands/DataChangedCommand.hpp"
#include "Product/ProductData.hpp"
#include "Product/Product.hpp"

class ProductTable: public BaseTable<ProductData, DataChangedCommand<ProductData, Product>>
{
    static constexpr auto* s_createTable {"CREATE TABLE IF NOT EXISTS Product ("
                                       "Name TEXT PRIMARY KEY, "
                                       "CategoryName TEXT, "
                                       "UnitRowMaterials REAL,"
                                       "PrimeCost REAL,"
                                       "Epsilon REAL)"};
    static constexpr auto* s_addProduct {"INSERT INTO Product VALUES ('{}', '{}', {}, {}, {})"};
    static constexpr auto* s_updateProductData {"UPDATE Product SET '{}' = {} WHERE Name = '{}'"};
    static constexpr auto* s_selectProducts {"SELECT * FROM Product"};
public:
    explicit ProductTable(const std::string& databaseName): BaseTable(databaseName)
    {
        try
        {
            m_database.exec(s_createTable);
        }
        catch(std::exception& exc)
        {
        }
    }
    bool addData(const ProductData& product) override
    {
        bool result {true};
        try
        {
            m_database.exec(std::format(s_addProduct,
                                        product.productName,
                                        product.categoryName,
                                        product.unitRawMaterials,
                                        product.primeCost,
                                        product.epsilon));
        }
        catch (std::exception& exc)
        {
            result = false;
        }
        return result;
    }
    bool updateData(std::unique_ptr<DataChangedCommand<ProductData, Product>>&& command) override
    {
        bool result {true};
        try
        {
            m_database.exec(std::format(s_updateProductData, command->getChangingFieldName(), command->getChangingField(), command->getIdentifier()));
        }
        catch(std::exception& exc)
        {
            result = false;
        }
        return result;
    }
    std::vector<ProductData> getData() override
    {
        std::vector<ProductData> result;
        try
        {
            SQLite::Statement query(m_database, s_selectProducts);
            while (query.executeStep())
            {
                const std::string name = query.getColumn(0);
                const std::string categoryName = query.getColumn(1);
                const double unitRawMaterials = query.getColumn(2);
                const double primeCost = query.getColumn(3);
                const double epsilon = query.getColumn(4);
                ProductData product {name, categoryName, unitRawMaterials, primeCost, epsilon};
                result.emplace_back(std::move(product));
            }
        }
        catch(std::exception& exc)
        {
        }
        return result;
    }
};
