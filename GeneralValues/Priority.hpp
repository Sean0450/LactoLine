#pragma once

#include <utility>

#include "../Date/Date.hpp"

namespace GeneralValues
{
enum class PriorityStatus
{
    Low =    0, 
    Medium = 1, 
    High =   2, 
    Extra =  3
};

class Priority
{
    Date::Date m_releaseDate;
    PriorityStatus m_prioity {PriorityStatus::Low};
    static constexpr Date::DateDistance s_lowPriorityMarker{0, 1, 0};
    static constexpr Date::DateDistance s_mediumPriorityMarker{14, 0, 0};
    static constexpr Date::DateDistance s_highProrityMarker {7, 0, 0};
    static constexpr Date::DateDistance s_extraPriorityMarker {3, 0, 0};
public:
    Priority() = default;
    template <typename ReleaseDate, typename CurrentDate>
    Priority(ReleaseDate&& releaseDate, CurrentDate&& currentDate): m_releaseDate(std::forward<ReleaseDate>(releaseDate))
    {
        changeReleaseDate(m_releaseDate, currentDate);
    }
    void changeReleaseDate(const Date::Date& newReleaseDate, const Date::Date& currentDate);
    void changePriority(PriorityStatus newStatus);
    PriorityStatus status() const;
};
}
