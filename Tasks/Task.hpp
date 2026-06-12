#pragma once

#include "../GeneralValues/Progress.hpp"
#include "../GeneralValues/Priority.hpp"
#include "../Product/Product.hpp"
#include "TaskData.hpp"

namespace Tasks
{
using namespace GeneralValues;
class Task
{
    GUI m_gui;
    Product m_productToCreate;
    Date::Date m_releaseDate;
    Priority m_taskPriority;
    Name m_taskName;
    Progress m_taskProgress;

    void setName(const Name& name);
public:
    Task()=default;
    Task(const TaskData& data, const Product& product);
    void addData(Amount doneProductAmount, Amount rawMaterials);
    bool isTaskDone() const;
    void changeTaskPriority(PriorityStatus newPriorityStatus);
    void changeReleaseDate(const Date::Date& newReleaseDate, const Date::Date& currentDate);
    void changeProductToDoAmount(Amount newProductAmount);
    void changeTaskName(const Name& newName);
    TaskData getTaskData() const;
    Amount leftToDo() const;
    GUI getTaskIdentifier() const;
};
}
