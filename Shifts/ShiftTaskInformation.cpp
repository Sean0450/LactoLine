#include "Shifts/ShiftTaskInformation.hpp"


ShiftTaskInformation::ShiftTaskInformation(Date::Date&& shiftDate,
                                           GeneralValues::Name&& productName,
                                           GeneralValues::Amount&& doneForShift,
                                           GeneralValues::Amount&& needToDo):m_shiftDate(shiftDate),
                                                                             m_productName(productName),
                                                                             m_doneForShift(doneForShift),
                                                                             m_needToDo(needToDo)
{
}


std::string ShiftTaskInformation::productName() const
{
    return m_productName;
}

double ShiftTaskInformation::doneForShift() const
{
    return m_doneForShift;
}

double ShiftTaskInformation::needToDo() const
{
    return m_needToDo;
}

std::string ShiftTaskInformation::date() const
{
    return m_shiftDate.getDate();
}
