#include "Date.hpp"
#include <format>
#include <regex>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Date 
{
Date::Date(std::string_view date)
{
    if (!std::regex_match(date.begin(), date.end(), s_dateTemplate))
        throw std::runtime_error("It is not a date");

    m_distance.day = std::stoi(date.substr(0, 2).data());
    m_distance.month = std::stoi(date.substr(3, 2).data());
    m_distance.year = std::stoi(date.substr(6, 4).data());
}

bool Date::isLeapYear(int year) const
{
    return year % 4 == 0 && year % 100 != 0;
}

int Date::dayCount(MonthName name, int year) const
{
    int result {0};
    switch(name)
    {
        case MonthName::Jan:
        case MonthName::Mar:
        case MonthName::May:
        case MonthName::Jul:
        case MonthName::Aug:
        case MonthName::Oct:
        case MonthName::Dec:
            result = 31;
            break;
        case MonthName::Apr:
        case MonthName::Jun:
        case MonthName::Sep:
        case MonthName::Nov:
            result = 30;
            break;
        case MonthName::Feb:
            result = 28;
            if (isLeapYear(year))
                ++result;
            break;
        default:
            break;
    }
    return result;
}

DateDistance Date::compareDate(const Date& date)
{
    const auto [ year, month, day] = m_distance;
    const auto [c_year, c_month, c_day] = date.m_distance;
    int yearsDisnace {year - c_year};
    if (yearsDisnace > 0)
    {
        if (month < c_month || (month == c_month && day < c_day))
            --yearsDisnace;
    }
    int monthesDistance {month - c_month};
    if (monthesDistance < 0)
    {
        monthesDistance+= 12;
    }
    if (day < c_day && monthesDistance != 0)
        --monthesDistance;
    int daysDistance {day - c_day};
    if (daysDistance < 0)
    {
        daysDistance = dayCount(static_cast<MonthName>(c_month), c_year) - c_day + day;
    }
    return {yearsDisnace, monthesDistance, daysDistance};
}

std::string Date::getDate() const
{
    const auto [ year, month, day] = m_distance;
    return std::format("{:02d}.{:02d}.{}", day, month, year);
}
}