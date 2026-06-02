#pragma once

#include <string>
#include <vector>

struct PackingListObserver
{
    virtual void generatePackingListDocumentation(const std::vector<std::string>& goods) = 0;
    virtual std::vector<std::string> getProductNames() = 0;
    virtual ~PackingListObserver() = default;
};
