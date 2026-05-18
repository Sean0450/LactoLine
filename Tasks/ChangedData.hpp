#pragma once

#include <string>
#include <variant>

#include "GeneralValues/Gui.hpp"
#include "GeneralValues/Priority.hpp"

enum class TaskDataChanged: int
{
    NameChanged = 0,
    StatusChanged = 1,
    ToDoAmountChanged = 2,
    DoneAmountChanged = 3
};

struct ChangedData
{
    std::string identifier;
    std::variant<GeneralValues::GUI, double, GeneralValues::PriorityStatus> data;
    TaskDataChanged flag;
};
