#pragma once

#include "Tasks/TaskData.hpp"
#include "Tasks/Task.hpp"

class TaskDataChangedCommand
{
protected:
    Tasks::TaskData& m_data;
public:
    TaskDataChangedCommand(Tasks::TaskData& data):m_data(data){}
    Tasks::TaskData getTaskData() const {return m_data;};
    virtual Tasks::Task getUpdatedTask(Tasks::Task task) const = 0;
    virtual std::string getChangingField() const = 0;
    virtual std::string getChangingFieldName() const = 0;
    GUI getIdentifier() const {return m_data.getIdentifier();}
};
