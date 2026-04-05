#include "Task.hpp"
#include <stdexcept>
//TODO: создать глобально-уникальный id для каждой задачи
namespace Tasks
{
Task::Task(Name&& taskName, 
           Amount productAmount, 
           Priority&& taskPriority, 
           Product&& productToCreate,
           Date::Date&& releaseDate ):m_taskName(std::move(taskName)),
                                      m_productAmount(productAmount),
                                      m_taskPriority(std::move(taskPriority)),
                                      m_productToCreate(std::move(productToCreate)),
                                      m_releaseDate(std::move(releaseDate)),
                                      m_taskProgress(Amount{0.0}, Amount{m_productAmount.value()})
{
}

bool Task::isTaskDone() const
{
    return m_taskProgress.completed() >= 100.0;
}

void Task::addData(Amount doneProductAmount)
{
    if (!isTaskDone())
    {
        m_createdProduct = Amount{ m_createdProduct.value() + doneProductAmount.value() };
        m_taskProgress.changeCurrentResult(m_createdProduct);
    }
}

void Task::changeTaskPriority(PriorityStatus newPriorityStatus)
{
    if (!isTaskDone())
        m_taskPriority = Priority{newPriorityStatus, m_releaseDate};
}

double Task::leftToDo() const
{
    return m_productAmount.value() - m_createdProduct.value();
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
        m_productAmount = newProductAmount;
        m_taskProgress.changeGoal(m_productAmount);
    }
}

void Task::changeTaskName(const Name& newName)
{
    if (!isTaskDone())
        m_taskName = newName;
}

std::string Task::taskName() const
{
    return m_taskName.name();
}

int Task::taskPriority() const
{
    return static_cast<int>(m_taskPriority.status());
}

std::string Task::releaseDate() const
{
    return m_releaseDate.getDate();
}

double Task::createdProduct() const
{
    return m_createdProduct.value();
}
}