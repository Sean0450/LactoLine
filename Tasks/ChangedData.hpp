#pragma once

#include <string>
#include <variant>

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
    std::variant<std::string, int> data;
    TaskDataChanged flag;
};
