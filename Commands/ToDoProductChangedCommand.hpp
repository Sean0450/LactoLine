#pragma once

#include "TaskDataChangedCommand.hpp"

class ToDoProductChangedCommand: public TaskDataChangedCommand
{
public:
    ToDoProductChangedCommand(Tasks::TaskData& data):TaskDataChangedCommand(data){}
    Tasks::Task getUpdatedTask(Tasks::Task task) const override {task.changeProductToDoAmount(Amount(m_data.productToDoAmount));
        return task;}
    std::string getChangingField() const override {return std::to_string(m_data.productToDoAmount);};
    std::string getChangingFieldName() const override {return {"ProductToDo"};};
};

