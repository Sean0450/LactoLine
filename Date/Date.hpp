#pragma once

#include <string_view>
#include <regex>

namespace Date 
{
enum class MonthName
{
    Jan = 1, 
    Feb = 2, 
    Mar = 3, 
    Apr = 4, 
    May = 5, 
    Jun = 6,
    Jul = 7, 
    Aug = 8, 
    Sep = 9, 
    Oct = 10, 
    Nov = 11, 
    Dec = 12
};

struct DateDistance
{
    int year {0};
    int month {0};
    int day {0};
    bool empty() const;
    bool isDistanceEqualOrMore(const DateDistance& distance) const;
};

class Date
{
    DateDistance m_distance;
    static inline const std::regex s_dateTemplate{R"((0[1-9]|[12][0-9]|3[01])\.([0][1-9]|1[0-2])\.2[01][0-9][0-9])"};
    int dayCount(MonthName name, int year) const;
    bool isLeapYear(int year) const;
public:
    Date() = default;
    explicit Date(std::string_view date);
    void setDate(std::string_view date);
    DateDistance compareDate(const Date& date) const;
    std::string getDate() const;
};
}