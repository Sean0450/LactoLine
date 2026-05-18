#include "TaskListWidget.hpp"
#include "TaskGroup.hpp"
#include "TaskWidget.hpp"
#include "CurrentShiftTaskGroup.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QGroupBox>

TaskListWidget::TaskListWidget(const std::vector<Tasks::TaskData>& tasks, QWidget *parent)
    : QWidget{parent},
      m_tasks(tasks)
{
    m_mainLayout = new QVBoxLayout(this);
    createWidgets();
    m_currentShifTasktGroup->addTaskNames(m_toDoTaskGroup->getTaskNames());
}

void TaskListWidget::createWidgets()
{
    std::vector<Tasks::TaskData> currentTasks;
    std::vector<Tasks::TaskData> toDoTasks;

    for (const auto& task: m_tasks)
    {
        if (task.status > GeneralValues::PriorityStatus::Low)
            currentTasks.emplace_back(task);
        else
            toDoTasks.emplace_back(task);
    }

    if (!currentTasks.empty())
    {
        m_currentShifTasktGroup = new CurrentShiftTaskGroup("Задачи на текущую смену", std::move(currentTasks), this);
        connect(m_currentShifTasktGroup,
                &CurrentShiftTaskGroup::askTask,
                this,
                [&](const QString& name){moveTaskFromToDo(name);});
        connect(m_currentShifTasktGroup,
                &TaskGroup::sendChangedData,
                this,
                [&](const ChangedData& data){changeTaskData(data);});

        m_mainLayout->addWidget(m_currentShifTasktGroup);
    }
    if (!toDoTasks.empty())
    {
        m_toDoTaskGroup = new TaskGroup("Задачи на перспективу", std::move(toDoTasks), this);
        m_toDoTaskGroup->setEnabled(false);
        m_mainLayout->addWidget(m_toDoTaskGroup);
        connect(m_toDoTaskGroup,
                &TaskGroup::sendChangedData,
                this,
                [&](const ChangedData& data){changeTaskData(data);});
    }
}

void TaskListWidget::moveTaskFromToDo(const QString& taskName)
{
    auto* widget = m_toDoTaskGroup->getWidget(taskName);
    if (widget)
    {
        widget->setParent(m_currentShifTasktGroup);
        widget->setEnabled(true);
        widget->show();
        m_currentShifTasktGroup->insertWidget(widget);
    }
}

void TaskListWidget::changeTaskData(const ChangedData& data)
{
    auto iterator = std::ranges::find_if(m_tasks, [data](const auto& task)
                                         {return data.identifier == task.getIdentifier();});
    if (iterator != std::ranges::end(m_tasks))
    {
        Tasks::TaskData& task = *iterator;
        switch(data.flag)
        {
        case TaskDataChanged::NameChanged:
            task.taskName = std::get<std::string>(data.data);
            break;
        case TaskDataChanged::StatusChanged:
            task.status = static_cast<GeneralValues::PriorityStatus>(std::get<int>(data.data));
            break;
        case TaskDataChanged::DoneAmountChanged:
            task.doneProduct = std::get<int>(data.data);
            break;
        case TaskDataChanged::ToDoAmountChanged:
            task.productToDoAmount = std::get<int>(data.data);
            break;
        }
    }
}
