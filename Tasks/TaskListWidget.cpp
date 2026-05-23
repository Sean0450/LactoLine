#include "TaskListWidget.hpp"
#include "TaskGroup.hpp"
#include "TaskWidget.hpp"
#include "CurrentShiftTaskGroup.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QGroupBox>

TaskListWidget::TaskListWidget(const std::vector<Tasks::TaskData>& tasks, TaskChangedObserver* observer, QWidget *parent)
    : QWidget{parent},
      m_observer(observer)
{
    m_mainLayout = new QVBoxLayout(this);
    createWidgets(tasks, m_observer);
    if (m_toDoTaskGroup)
    {
        m_currentShifTasktGroup->addTaskNames(m_toDoTaskGroup->getTaskNames());
    }
}

void TaskListWidget::createWidgets(const std::vector<Tasks::TaskData>& tasks, TaskChangedObserver* observer)
{
    std::vector<Tasks::TaskData> currentTasks;
    std::vector<Tasks::TaskData> toDoTasks;

    for (const auto& task: tasks)
    {
        if (task.status > GeneralValues::PriorityStatus::Low)
            currentTasks.emplace_back(task);
        else
            toDoTasks.emplace_back(task);
    }

    m_currentShifTasktGroup = new CurrentShiftTaskGroup(QStringLiteral("Задачи на текущую смену"), std::move(currentTasks), observer, this);
    connect(m_currentShifTasktGroup,
            &CurrentShiftTaskGroup::askTask,
            this,
            [&](const QString& name){moveTaskFromToDo(name);});
    connect(m_currentShifTasktGroup,
            &CurrentShiftTaskGroup::newTaskCreated,
            this,
            [&](const auto& data){addNewTask(data);});

    m_mainLayout->addWidget(m_currentShifTasktGroup);
    if (!toDoTasks.empty())
    {
        m_toDoTaskGroup = new TaskGroup(QStringLiteral("Задачи на перспективу"), std::move(toDoTasks), observer, this);
        m_toDoTaskGroup->setEnabled(false);
        m_mainLayout->addWidget(m_toDoTaskGroup);
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
        if (!m_toDoTaskGroup->hasTasks())
            m_toDoTaskGroup->setVisible(false);
    }
}

void TaskListWidget::addNewTask(const Tasks::TaskData& data)
{
    if (!m_toDoTaskGroup)
    {
        m_toDoTaskGroup = new TaskGroup(QStringLiteral("Задачи на перспективу"), {data}, m_observer, this);
        m_mainLayout->addWidget(m_toDoTaskGroup);
    }
    else
    {
        m_toDoTaskGroup->setVisible(true);
        m_toDoTaskGroup->addWidget(data, m_observer);
    }
    m_currentShifTasktGroup->addTaskNames(m_toDoTaskGroup->getTaskNames());
    m_toDoTaskGroup->setEnabled(false);
    emit taskAdded(data);
}
