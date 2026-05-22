#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "Tasks/TaskData.hpp"
#include "Commands/TaskDataChangedCommand.hpp"

class TasksTable
{
    std::string m_tableName;
    SQLite::Database m_database;

    std::string wrapString(const std::string& text);

public:
    explicit TasksTable(const std::string& databaseName);
    bool isTableExist() const;
    bool addTask(const Tasks::TaskData& taskData);
    void updateTaskData(std::unique_ptr<TaskDataChangedCommand>&& command);
    void updateDoneProduct(const GUI& gui, double doneProduct);
    std::vector<Tasks::TaskData> getTasks() const;
};
