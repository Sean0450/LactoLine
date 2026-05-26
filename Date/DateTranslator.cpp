#include "DateTranslator.hpp"

#include "Date.hpp"

GeneralValues::PriorityStatus DateTranslator::calculatePriority(const std::string& viewDateFormat, const std::string& releaseDate)
{
    using namespace GeneralValues;
    return Priority(Date::Date{releaseDate}, Date::Date{viewDateFormat}).status();
}

bool DateTranslator::isReleaseDateCorrect(const std::string& currentDate, const std::string& releaseDate)
{
    const Date::Date current {currentDate};
    const Date::Date release {releaseDate};
    return release.isDateMore(current);
}

std::string DateTranslator::fromChronoDateFormat(const std::chrono::year_month_day& date)
{
    std::string correctDate;
    constexpr int dateLength {10};
    constexpr auto spliter {'.'};
    correctDate.reserve(dateLength);
    unsigned int day = static_cast<unsigned int>(date.day());
    unsigned int month = static_cast<unsigned int>(date.month());
    int year = static_cast<int>(date.year());
    if (day < 10)
        correctDate += "0" + std::to_string(day);
    else
        correctDate += std::to_string(day);
    correctDate += spliter;
    if (month < 10)
        correctDate += "0" + std::to_string(month);
    else
        correctDate += std::to_string(month);
    correctDate += spliter;
    correctDate += std::to_string(year);
    return correctDate;
}
