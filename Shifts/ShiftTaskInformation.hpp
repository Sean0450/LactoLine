#pragma once

#include "GeneralValues/Name.hpp"
#include "GeneralValues/Amount.hpp"
#include "Date/Date.hpp"

class ShiftTaskInformation
{
    Date::Date m_shiftDate;
    GeneralValues::Name m_productName;
    GeneralValues::Amount m_doneForShift;
    GeneralValues::Amount m_needToDo;
public:
    ShiftTaskInformation() = default;
    explicit ShiftTaskInformation(Date::Date&& shiftDate,
                                  GeneralValues::Name&& productName,
                                  GeneralValues::Amount&& doneForShift,
                                  GeneralValues::Amount&& needToDo);
    std::string productName() const;
    double doneForShift() const;
    double needToDo() const;
    std::string date() const;
};
