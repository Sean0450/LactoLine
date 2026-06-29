#pragma once

#include <string>
#include <vector>

struct PackingListObserver
{
    virtual std::vector<std::string> getProductNames() = 0;
    virtual ~PackingListObserver() = default;
};
