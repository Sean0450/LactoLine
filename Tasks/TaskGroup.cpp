#include "TaskGroup.hpp"
#include "TaskWidget.hpp"

#include <QVBoxLayout>

TaskGroup::TaskGroup(const QString& title, std::vector<Tasks::TaskData>&& data, QWidget *parent)
    : QGroupBox{parent}
{
    setFont(m_baseFont);
    setTitle(title);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);

    std::ranges::for_each(data, [&](const auto& task){addWidget(task);});
    setSpacing();
}

void TaskGroup::addWidget(const Tasks::TaskData& data)
{
    if (!std::ranges::any_of(m_tasks, [&](auto* widget){return data.getIdentifier() == widget->getTaskIdentifier();}))
    {
        m_tasks.emplace_back(new TaskWidget(data, this));
        m_mainLayout->addWidget(m_tasks.back());
        connect(m_tasks.back(),
                &TaskWidget::taskDataChanged,
                this,
                [&](const ChangedData& data){emit sendChangedData(data);});
        int newWidth = m_tasks.back()->getTaskNameWidth();
        if (newWidth > m_maxLabelWidth)
            m_maxLabelWidth = newWidth;
    }
}

void TaskGroup::setSpacing()
{
    std::ranges::for_each(m_tasks,
                          [&](auto* widget){widget->addSpacing(m_maxLabelWidth - widget->getTaskNameWidth());});
}

void TaskGroup::setEnabled(bool enabled)
{
    std::ranges::for_each(m_tasks, [enabled](auto* widget){widget->setEnabled(enabled);});
}

QStringList TaskGroup::getTaskNames() const
{
    QStringList data;
    std::ranges::for_each(m_tasks, [&](auto* widget){data << QString::fromStdString(widget->getTaskName());});
    return data;
}

TaskWidget* TaskGroup::getWidget(const QString& taskName)
{
    TaskWidget* widget {nullptr};
    auto iterator = std::ranges::find_if(m_tasks, [&](auto* element){return element->getTaskName() == taskName;});
    if (iterator != std::ranges::end(m_tasks))
    {
        widget = *iterator;
        m_tasks.erase(iterator);
    }
    return widget;
}
