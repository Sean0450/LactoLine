#pragma once

#include <vector>

#include "ShiftData.hpp"

struct ShiftTaskInformationObserver
{
    virtual std::vector<ShiftData> getTasksData(const std::string& data) = 0;
    virtual ~ShiftTaskInformationObserver() = default;
};
