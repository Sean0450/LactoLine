#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "Tasks/TaskData.hpp"
#include "Tasks/ChangedData.hpp"

class TasksTable
{
    std::string m_tableName;
    SQLite::Database m_database;

    std::string wrapString(const std::string& text);

public:
    explicit TasksTable(const std::string& databaseName);
    bool isTableExist() const;
    bool addTask(const Tasks::TaskData& taskData);
    void updateTaskData(const ChangedData& data);
    std::vector<Tasks::TaskData> getTasks() const;
};
