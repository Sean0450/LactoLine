#pragma once

#include "GeneralValues/Priority.hpp"

struct DateTranslator
{
    static GeneralValues::PriorityStatus calculatePriority(const std::string& viewDateFormat, const std::string& releaseDate);
    static bool isReleaseDateCorrect(const std::string& currentDate, const std::string& releaseDate);
    static bool isCurrentDateMoreOrEqual(const std::string& currentDate, const std::string& dateToCompare);
    static std::string getModelCurrentDate();
    static std::string getCurrentDate();
};
