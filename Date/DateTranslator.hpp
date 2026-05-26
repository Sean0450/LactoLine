#pragma once

#include <chrono>

#include "GeneralValues/Priority.hpp"

struct DateTranslator
{
    static GeneralValues::PriorityStatus calculatePriority(const std::string& viewDateFormat, const std::string& releaseDate);
    static bool isReleaseDateCorrect(const std::string& currentDate, const std::string& releaseDate);
    static std::string fromChronoDateFormat(const std::chrono::year_month_day& date);
};
