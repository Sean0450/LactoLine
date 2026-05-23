#pragma once

#include "TaskGroup.hpp"

QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class CurrentShiftTaskGroup: public TaskGroup
{
    Q_OBJECT

    const QFont m_baseFont {Resources::baseFont, 12};

    QComboBox* m_addTask {nullptr};
    QPushButton* m_applyAdding {nullptr};
    QPushButton* m_createTask {nullptr};
    QVBoxLayout* m_buttonsLayout {nullptr};

    QStringList m_toDoTasks;

    void createAddTaskComboBox();
    void createTaskCreationButton();
    void calcAddTaskWidth();

private slots:
    void onAddTaskIndexChange();
    void onApplyAddingClick();
    void onCreateTaskButtonClick();
public:
    explicit CurrentShiftTaskGroup(const QString& title,
                                   std::vector<Tasks::TaskData>&& data,
                                   TaskChangedObserver* observer,
                                   QWidget* parent = nullptr);
    void addToDoTaskData (QStringList&& data);
    void addTaskNames(const QStringList& names);
    void insertWidget(TaskWidget* widget);
    void addWidget(const Tasks::TaskData& data, TaskChangedObserver* observer) override;
signals:
    void askTask(const QString& taskName);
    void newTaskCreated(const Tasks::TaskData& data, bool isForCurrentShift);
};

