#include "Priority.hpp"
#include <stdexcept>

namespace GeneralValues
{

Priority::Priority(const Date::Date& releaseDate, const Date::Date& currentDate)
{
    changeReleaseDate(releaseDate, currentDate);
}

Priority::Priority(PriorityStatus status, const Date::Date& releaseDate):m_prioity(status),
                                                                         m_releaseDate(releaseDate)
{
}

void Priority::changeReleaseDate(const Date::Date& newReleaseDate, const Date::Date& currentDate)
{
    Date::DateDistance timeToComplete = newReleaseDate.compareDate(currentDate);
    if (timeToComplete.isDistanceEqualOrMore(s_lowPriorityMarker))
        m_prioity = PriorityStatus::Low;
    else if (timeToComplete.isDistanceEqualOrMore(s_mediumPriorityMarker))
        m_prioity = PriorityStatus::Medium;
    else if (timeToComplete.isDistanceEqualOrMore(s_highProrityMarker))
        m_prioity = PriorityStatus::High;
    else if (timeToComplete.isDistanceEqualOrMore(s_extraPriorityMarker))
        m_prioity = PriorityStatus::Extra;
    else
        throw std::runtime_error("Release date too close");
}

PriorityStatus Priority::status() const
{
    return m_prioity;
}

}