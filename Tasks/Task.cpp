#include "Task.hpp"
#include <stdexcept>
//TODO: создать глобально-уникальный id для каждой задачи
namespace Tasks
{
Task::Task(const TaskData& taskData, const Product& product): m_gui(taskData.getIdentifier()),
                                                              m_productToCreate(product),
                                                              m_releaseDate(std::string_view(taskData.releaseDate)),
                                                              m_taskPriority(taskData.status, m_releaseDate)
{
    setName(taskData.taskName);
    m_taskProgress = Progress{taskData.doneProduct, taskData.productToDoAmount, taskData.wastedRawMaterials};
}

void Task::setName(const Name& name)
{
    if (!name.empty())
        m_taskName = name;
    else
        throw std::runtime_error("Имя задачи не может быть пустой строкой");
}

bool Task::isTaskDone() const
{
    return m_taskProgress.isCompleted();
}

void Task::addData(Amount doneProductAmount, Amount rawMaterials)
{
    if (!isTaskDone())
    {
        m_taskProgress.changeCurrentResult(doneProductAmount, rawMaterials);
    }
}

void Task::changeTaskPriority(PriorityStatus newPriorityStatus)
{
    if (!isTaskDone())
        m_taskPriority = Priority{newPriorityStatus, m_releaseDate};
}

Amount Task::leftToDo() const
{
    constexpr int doneProductIndex {0};
    constexpr int productToDoAmountIndex {1};
    auto amountData = m_taskProgress.getAmountData();
    return std::get<productToDoAmountIndex>(amountData) - std::get<doneProductIndex>(amountData);
}

void Task::changeReleaseDate(const Date::Date& newReleaseDate, const Date::Date& currentDate)
{
    if (!isTaskDone())
    {
        if (!newReleaseDate.isDateMore(currentDate))
            throw std::runtime_error("New release date has already passed");
        m_releaseDate = newReleaseDate;
        m_taskPriority = Priority{newReleaseDate, currentDate};
    }
}

void Task::changeProductToDoAmount(Amount newProductAmount)
{
    if (!isTaskDone())
    {
        m_taskProgress.changeGoal(newProductAmount);
    }
}

void Task::changeTaskName(const Name& newName)
{
    if (!isTaskDone())
        m_taskName = newName;
}

TaskData Task::getTaskData() const
{
    constexpr int doneProductIndex {0};
    constexpr int productToDoAmountIndex {1};
    constexpr int wastedRawMaterialsIndex {2};
    std::string releaseDate = m_releaseDate.getDate();
    auto amountData = m_taskProgress.getAmountData();
    Amount doneProduct = std::get<doneProductIndex>(amountData);
    Amount productToDoAmount = std::get<productToDoAmountIndex>(amountData);
    Amount wastedRawMaterials = std::get<wastedRawMaterialsIndex>(amountData);
    return TaskData{m_taskName, m_productToCreate.name(), releaseDate, m_taskPriority.status(), productToDoAmount, doneProduct, wastedRawMaterials, m_gui};
}

GUI Task::getTaskIdentifier() const
{
    return m_gui;
}
}
