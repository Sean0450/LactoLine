#pragma once

#include "Tasks/TaskData.hpp"

class TaskDataChangedCommand
{
protected:
    static constexpr auto* s_updateTaskData {"UPDATE Tasks SET '{}' = {} WHERE GUI = '{}'"};
    Tasks::TaskData& m_data;
public:
    TaskDataChangedCommand(Tasks::TaskData& data):m_data(data){}
    Tasks::TaskData getTaskData() const {return m_data;};
    virtual std::string createRequest() = 0;
};
