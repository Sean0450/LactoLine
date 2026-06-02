#pragma once

#include "../GeneralValues/Amount.hpp"
#include "../GeneralValues/Progress.hpp"
#include "../GeneralValues/Priority.hpp"
#include "../Product/Product.hpp"
#include "../GeneralValues/Gui.hpp"

namespace Tasks
{
using namespace GeneralValues;
class Task
{
    GUI m_gui;
    Name m_taskName;
    Amount m_productAmount;
    Priority m_taskPriority;
    Product m_productToCreate;
    Date::Date m_releaseDate;
    Progress m_taskProgress;
    Amount m_createdProduct;
    Amount m_wastedRawMaterials;
public:
    Task()=default;
    Task(const GUI& gui,
         Name&& taskName,
         Amount productAmount,
         Priority&& taskPriority,
         Product&& productToCreate,
         Date::Date&& releaseDate);
    void addData(Amount doneProductAmount, Amount rawMaterials);
    bool isTaskDone() const;
    void changeTaskPriority(PriorityStatus newPriorityStatus);
    void changeReleaseDate(const Date::Date& newReleaseDate, const Date::Date& currentDate);
    void changeProductToDoAmount(Amount newProductAmount);
    void changeTaskName(const Name& newName);
    GUI gui() const;
    double leftToDo() const;
    std::string taskName() const;
    std::string productName() const;
    int taskPriority() const;
    std::string releaseDate() const;
    double createdProduct() const;
    double productAmount() const;
    double wastedRawMaterials() const;
};
}
