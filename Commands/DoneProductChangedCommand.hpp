#pragma once

#include <format>

#include "TaskDataChangedCommand.hpp"

class DoneProductChangedCommand: public TaskDataChangedCommand
{
public:
    DoneProductChangedCommand(Tasks::TaskData& data):TaskDataChangedCommand(data){}
    std::string createRequest() override {return std::format(s_updateTaskData, "CreatedProduct", m_data.doneProduct, m_data.getIdentifier());}
};

