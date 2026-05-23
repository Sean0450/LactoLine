#pragma once

#include <QWidget>
#include <QAbstractListModel>

#include "Resources.hpp"
#include "Tasks/TaskData.hpp"

QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QGroupBox)

class TaskWidget;
class TaskGroup;
class CurrentShiftTaskGroup;
class TaskChangedObserver;

class TaskListWidget : public QWidget
{
    Q_OBJECT
    const QFont m_baseFont {Resources::baseFont, 16};

    QVBoxLayout* m_mainLayout {nullptr};
    TaskGroup* m_toDoTaskGroup {nullptr};
    CurrentShiftTaskGroup* m_currentShifTasktGroup {nullptr};
    TaskChangedObserver* m_observer {nullptr};

    void createWidgets(const std::vector<Tasks::TaskData>& tasks, TaskChangedObserver* observer);
    QGroupBox* createTasksGroup(const QString& title, const std::vector<TaskWidget*>& widgets, bool areEnabled = true);
    bool isTaskForCurrentShift(const Tasks::TaskData& data) const;
private slots:
    void moveTaskFromToDo(const QString& taskName);
    void addNewTask(const Tasks::TaskData& data, bool isTaskForCurrentShift);
public:
    explicit TaskListWidget(const std::vector<Tasks::TaskData>& tasks, TaskChangedObserver* observer, QWidget *parent = nullptr);
signals:
    void taskAdded(const Tasks::TaskData& data);
};
