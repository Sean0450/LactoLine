#include "Date.hpp"
#include <format>
#include <regex>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Date 
{

bool DateDistance::empty() const
{
    return day   == 0 &&
           month == 0 &&
           year  == 0; 

}

bool DateDistance::isDistanceEqualOrMore(const DateDistance& distance) const
{
    bool result {true};
    const auto [c_day, c_month, c_year] = distance;
    if (year == c_year)
    {
        if (month < c_month)
            result = false;
        if (month == c_month)
        {
            if (day < c_day)
                result = false;
        }
    }
    else if (year < c_year)
        result = false;
    
    return result;
}
Date::Date(std::string_view date)
{
    setDate(date);
}

void Date::setDate(std::string_view date)
{
    if (!std::regex_match(date.begin(), date.end(), s_dateTemplate) || !m_distance.empty())
        throw std::runtime_error("It is not a date");

    m_distance.day = std::stoi(date.substr(0, 2).data());
    m_distance.month = std::stoi(date.substr(3, 2).data());
    m_distance.year = std::stoi(date.substr(6, 4).data());
    if (dayCount(static_cast<MonthName>(m_distance.month), m_distance.year) < m_distance.day)
    {
        m_distance = DateDistance();
        throw std::runtime_error("Invalid days count");
    }
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

DateDistance Date::compareDate(const Date& date) const
{
    if (m_distance.empty())
        throw std::runtime_error("Can't compae dates. Current date is empty");
    if (date.m_distance.empty())
        throw std::runtime_error("Can't compare dates. Incomming date is empty");

    int yearsDistance {0};
    int monthesDistance {0};
    int daysDistance {0};
    if (m_distance.isDistanceEqualOrMore(date.m_distance))
    {
        const auto [ day, month, year] = m_distance;
        const auto [c_day, c_month, c_year] = date.m_distance;
        yearsDistance = year - c_year;
        if (yearsDistance > 0)
        {
            if (month < c_month || (month == c_month && day < c_day))
                --yearsDistance;
        }
        monthesDistance = month - c_month;
        if (monthesDistance < 0 || (monthesDistance == 0 && day < c_day))
        {
            monthesDistance+= 12;
        }
        if (day < c_day && monthesDistance != 0)
            --monthesDistance;
        daysDistance = day - c_day;
        if (daysDistance < 0)
        {
            daysDistance = dayCount(static_cast<MonthName>(c_month), c_year) - c_day + day;
        }
    }
    return {.day=daysDistance, .month=monthesDistance, .year=yearsDistance};
}

std::string Date::getDate() const
{
    const auto [day, month, year] = m_distance;
    return std::format("{:02d}.{:02d}.{}", day, month, year);
}
}