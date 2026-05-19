#pragma once

#include <vector>
#include <optional>

#include "Task.hpp"
#include "Database/TasksTable.hpp"
#include "Commands/TaskDataChangedCommand.hpp"
#include "TaskChangedObserver.hpp"

namespace Tasks
{
class TaskManager: public TaskChangedObserver
{
    static constexpr auto* s_tableName {"Tasks.db3"};

    std::vector<Task> m_tasks;
    TasksTable m_taskTable;

    std::optional<Task> fromDataToTask(const TaskData& data);
    TaskData fromTaskToData(const Task& task) const;
    void readTasksFromTable();
public:
    explicit TaskManager();
    void addTask(const TaskData& taskData);
    std::vector<TaskData> getCurrentShiftTaskData() const;
    void taskDataChanged(std::unique_ptr<TaskDataChangedCommand>&& command) override;
};
}
