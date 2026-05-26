#pragma once

#include <vector>

#include "Product/ProductData.hpp"

struct ProductChangedObserver
{
    virtual std::vector<ProductData> getProductData() = 0;
    virtual void createProduct(const ProductData& data) = 0;
};
