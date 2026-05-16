#pragma once

#include <QWidget>
#include <QAbstractListModel>

#include "Resources.hpp"
#include "Tasks/TaskData.hpp"
#include "ChangedData.hpp"

QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QGroupBox)

class TaskWidget;
class TaskGroup;
class CurrentShiftTaskGroup;

class TaskList : public QWidget
{
    Q_OBJECT
    std::vector<Tasks::TaskData> m_tasks;
    const QFont m_baseFont {Resources::baseFont, 16};

    QVBoxLayout* m_mainLayout {nullptr};
    QGroupBox* m_toDoTasks {nullptr};
    TaskGroup* m_toDoTaskGroup {nullptr};
    CurrentShiftTaskGroup* m_currentShifTasktGroup {nullptr};

    void createWidgets();
    QGroupBox* createTasksGroup(const QString& title, const std::vector<TaskWidget*>& widgets, bool areEnabled = true);
private slots:
    void moveTaskFromToDo(const QString& taskName);
    void changeTaskData(const ChangedData& data);
public:
    explicit TaskList(const std::vector<Tasks::TaskData>& tasks, QWidget *parent = nullptr);
};
