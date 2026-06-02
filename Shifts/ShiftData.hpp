#pragma once

#include "GeneralValues/Gui.hpp"

struct ShiftData
{
    std::string date;
    std::string taskName;
    double doneForShift {0.0};
    double needToDo {0.0};
    bool isComplete;
    explicit ShiftData(const std::string& date, const std::string& taskName, double doneForShift, double needToDo, bool isComplete, const GeneralValues::GUI& gui):date(date),
                                                                                                                                                                   taskName(taskName),
                                                                                                                                                                   doneForShift(doneForShift),
                                                                                                                                                                   needToDo(needToDo),
                                                                                                                                                                   isComplete(isComplete),
                                                                                                                                                                   gui(gui)
    {
    }
    GeneralValues::GUI getIdentifier() const {return gui;}
private:
    GeneralValues::GUI gui;
};
