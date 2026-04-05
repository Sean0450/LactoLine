#pragma once

#include "../GeneralValues/Amount.hpp"
#include "../GeneralValues/Progress.hpp"
#include "../GeneralValues/Priority.hpp"
#include "../Product.hpp"

namespace Tasks
{
using namespace GeneralValues;
class Task
{
    Name m_taskName;
    Amount m_productAmount;
    Priority m_taskPriority;
    Product m_productToCreate;
    Date::Date m_releaseDate;
    Progress m_taskProgress;
    Amount m_createdProduct;
public:
    Task()=default;
    Task(Name&& taskName, Amount productAmount, Priority&& taskPriority, Product&& productToCreate, Date::Date&& releaseDate);
    void addData(Amount doneProductAmount);
    bool isTaskDone() const;
    void changeTaskPriority(PriorityStatus newPriorityStatus);
    void changeReleaseDate(const Date::Date& newReleaseDate, const Date::Date& currentDate);
    void changeProductToDoAmount(Amount newProductAmount);
    void changeTaskName(const Name& newName);
    double leftToDo() const;
    std::string taskName() const;
    int taskPriority() const;
    std::string releaseDate() const;
    double createdProduct() const;
};
}