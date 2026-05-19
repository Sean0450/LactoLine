#pragma once

#include <format>

#include "TaskDataChangedCommand.hpp"

class ToDoProductChangedCommand: public TaskDataChangedCommand
{
public:
    ToDoProductChangedCommand(Tasks::TaskData& data):TaskDataChangedCommand(data){}
    std::string createRequest() override {return std::format(s_updateTaskData, "ProductToDo", m_data.productToDoAmount, m_data.getIdentifier());}
};

