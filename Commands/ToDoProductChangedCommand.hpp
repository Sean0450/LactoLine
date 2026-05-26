#pragma once

#include "DataChangedCommand.hpp"
#include "Tasks/TaskData.hpp"
#include <Tasks/Task.hpp>

class ToDoProductChangedCommand: public DataChangedCommand<Tasks::TaskData, Tasks::Task>
{
public:
    ToDoProductChangedCommand(Tasks::TaskData& data):DataChangedCommand(data){}
    Tasks::Task getUpdatedTask(Tasks::Task task) const override {task.changeProductToDoAmount(Amount(m_data.productToDoAmount));
                                                                 return task;}
    std::string getChangingField() const override {return std::to_string(m_data.productToDoAmount);};
    std::string getChangingFieldName() const override {return {"ProductToDo"};};
    std::string getIdentifier() const override {return m_data.getIdentifier();}
};
