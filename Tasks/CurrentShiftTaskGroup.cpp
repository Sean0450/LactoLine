#include "CurrentShiftTaskGroup.hpp"
#include "TaskWidget.hpp"
#include "Dialogs/CreateTaskdialog.hpp"

#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

CurrentShiftTaskGroup::CurrentShiftTaskGroup(const QString& title,
                                             std::vector<Tasks::TaskData>&& data,
                                             TaskChangedObserver* observer,
                                             QWidget* parent): TaskGroup{title, std::move(data), observer, parent}
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
        m_addTask->setStyleSheet("QComboBox {" + QLatin1String(Resources::Styles::baseElementsStyle) + "}" +
                                 QLatin1String(Resources::Styles::dropDownComboBoxStyle));
        connect(m_addTask, &QComboBox::currentIndexChanged, this, &CurrentShiftTaskGroup::onAddTaskIndexChange);

        m_applyAdding = new QPushButton("Добавить", this);
        m_applyAdding->setFont(m_baseFont);
        m_applyAdding->setStyleSheet("QPushButton{ " + QLatin1String(Resources::Styles::baseElementsStyle) + "}" +
                                     QLatin1String(Resources::Styles::pressedButtonStyle));
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
        connect(m_createTask, &QPushButton::clicked, this, &CurrentShiftTaskGroup::onCreateTaskButtonClick);
        layout->addWidget(m_createTask);
        layout->addStretch(1);

        m_mainLayout->addLayout(layout);
    }
}

void CurrentShiftTaskGroup::calcAddTaskWidth()
{
    int maxLength {0};
    QFontMetrics fm(m_baseFont);
    for (const auto& name: m_toDoTasks)
    {
        int width = fm.horizontalAdvance(name);
        if (width > maxLength)
        {
            maxLength = width;
        }
    }
    if (maxLength > 0)
    {
        constexpr int spacingPixels {20};
        maxLength += spacingPixels;
        m_addTask->setMinimumWidth(maxLength);
        m_addTask->setMaximumWidth(maxLength);
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
        m_addTask->removeItem(currentIndex);
        m_addTask->setCurrentIndex(-1);
        m_toDoTasks.removeAt(currentIndex);
        calcAddTaskWidth();
        m_applyAdding->setVisible(false);
    }
}

void CurrentShiftTaskGroup::onCreateTaskButtonClick()
{
    QStringList data {"Молоко 1.5л"};
    auto createTaskDialog = CreateTaskDialog(data, this);
    createTaskDialog.exec();
    std::optional<Tasks::TaskData> taskData = createTaskDialog.getTaskData();
    if (taskData)
    {
        emit newTaskCreated(taskData.value(), createTaskDialog.isTaskForCurrentShift());
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
        m_addTask->clear();
        m_addTask->addItems(m_toDoTasks);
        m_addTask->setCurrentIndex(-1);
        calcAddTaskWidth();
        m_applyAdding->setVisible(false);
    }
}

void CurrentShiftTaskGroup::insertWidget(TaskWidget* widget)
{
    if (!std::ranges::any_of(m_tasks, [widget](auto* element){return element->getTaskIdentifier() == widget->getTaskIdentifier();}))
    {
        m_tasks.emplace_back(widget);
        int position = static_cast<int>(m_tasks.size()) - 1;
        m_mainLayout->insertWidget(position, m_tasks.back());
    }
}

void CurrentShiftTaskGroup::addWidget(const Tasks::TaskData& data, TaskChangedObserver* observer)
{
    if (!std::ranges::any_of(m_tasks, [&](auto* element){return element->getTaskIdentifier() == data.getIdentifier();}))
    {
        m_tasks.emplace_back(new TaskWidget(data, observer, this));
        int position = static_cast<int>(m_tasks.size()) - 1;
        m_mainLayout->insertWidget(position, m_tasks.back());
    }
}
