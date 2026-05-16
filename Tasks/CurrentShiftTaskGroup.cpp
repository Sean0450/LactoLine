#include "CurrentShiftTaskGroup.hpp"
#include "TaskWidget.hpp"

#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

CurrentShiftTaskGroup::CurrentShiftTaskGroup(const QString& title,
                                             std::vector<Tasks::TaskData>&& data,
                                             QWidget* parent): TaskGroup{title, std::move(data), parent}
{
    createAddTaskComboBox();
    createTaskCreationButton();
}

void CurrentShiftTaskGroup::createAddTaskComboBox()
{
    if (!m_addTask)
    {
        auto* addTaskLabel = new QLabel("Добавить задачу: ", this);
        addTaskLabel->setFont(m_baseFont);
        auto* layout = new QHBoxLayout();
        m_addTask = new QComboBox(this);
        m_addTask->setFont(m_baseFont);
        m_addTask->setStyleSheet("QComboBox {" + QString::fromUtf8(Resources::Styles::baseElementsStyle) + "}" +
                                 QString::fromUtf8(Resources::Styles::dropDownComboBoxStyle));
        connect(m_addTask, &QComboBox::currentIndexChanged, this, &CurrentShiftTaskGroup::onAddTaskIndexChange);

        m_applyAdding = new QPushButton("Добавить", this);
        m_applyAdding->setFont(m_baseFont);
        m_applyAdding->setStyleSheet("QPushButton{ " + QString::fromUtf8(Resources::Styles::baseElementsStyle) + "}" +
                                     QString::fromUtf8(Resources::Styles::pressedButtonStyle));
        m_applyAdding->setVisible(false);
        connect(m_applyAdding, &QPushButton::clicked, this, &CurrentShiftTaskGroup::onApplyAddingClick);

        layout->addWidget(addTaskLabel);
        layout->addWidget(m_addTask);
        layout->addWidget(m_applyAdding);
        layout->addStretch(1);
        m_mainLayout->addLayout(layout);
    }
}

void CurrentShiftTaskGroup::createTaskCreationButton()
{
    if (!m_createTask)
    {
        auto* layout = new QHBoxLayout();
        m_createTask = new QPushButton("Сформировать задачу", this);
        m_createTask->setFont(m_baseFont);
        m_createTask->setStyleSheet("QPushButton{ " + QString::fromUtf8(Resources::Styles::baseElementsStyle) + "}" +
                                    QString::fromUtf8(Resources::Styles::pressedButtonStyle));
        layout->addWidget(m_createTask);
        layout->addStretch(1);

        m_mainLayout->addLayout(layout);
    }
}

void CurrentShiftTaskGroup::onAddTaskIndexChange()
{
    m_applyAdding->setVisible(true);
}

void CurrentShiftTaskGroup::onApplyAddingClick()
{
    int currentIndex {m_addTask->currentIndex()};
    if (currentIndex >= 0)
    {
        emit askTask(m_addTask->currentText());
        m_addTask->removeItem(m_addTask->currentIndex());
        m_applyAdding->setVisible(false);
    }
}

void CurrentShiftTaskGroup::addToDoTaskData (QStringList&& data)
{
    m_toDoTasks = std::move(data);
}

void CurrentShiftTaskGroup::addTaskNames(const QStringList& names)
{
    if (m_addTask)
    {
        m_toDoTasks = names;
        m_addTask->addItems(m_toDoTasks);
        m_addTask->setCurrentIndex(-1);
        m_applyAdding->setVisible(false);
    }
}

void CurrentShiftTaskGroup::insertWidget(TaskWidget* widget)
{
    if (!std::ranges::any_of(m_tasks, [widget](auto* element){return element->getTaskName() == widget->getTaskName();}))
    {
        m_tasks.emplace_back(widget);
        connect(m_tasks.back(),
                &TaskWidget::taskDataChanged,
                this,
                [&](const ChangedData& data){emit sendChangedData(data);});
        int position = static_cast<int>(m_tasks.size()) - 1;
        m_mainLayout->insertWidget(position, m_tasks.back());
        int newWidth = m_tasks.back()->getTaskNameWidth();
        if (newWidth > m_maxLabelWidth)
            m_maxLabelWidth = newWidth;
        setSpacing();
    }
}
