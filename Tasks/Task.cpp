#include "Task.hpp"
#include "Date/DateTranslator.hpp"

#include <stdexcept>

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

void Task::changeTaskPriority(const Date::Date& currentDate)
{
    if (!isTaskDone())
        m_taskPriority = Priority{m_releaseDate, currentDate};
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

std::optional<Task> Task::fromDataToTask(const TaskData& data, const Product& product)
{
    std::optional<Task> outputTask;
    try
    {
        Task task(data, product);
        task.changeTaskPriority(Date::Date(DateTranslator::getModelCurrentDate()));
        outputTask = std::move(task);
    }
    catch(std::exception& exc)
    {
        outputTask = std::nullopt;
    }
    return outputTask;
}
}
