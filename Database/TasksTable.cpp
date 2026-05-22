#include "TasksTable.hpp"
#include <iostream>
#include <format>
#include <windows.h>

namespace requests
{
static constexpr auto* createTable {"CREATE TABLE IF NOT EXISTS Tasks ("
                                    "GUI TEXT PRIMARY KEY, "
                                    "Name TEXT, "
                                    "ProductToDo REAL,"
                                    "Priority INTEGER,"
                                    "Product TEXT,"
                                    "ReleaseDate TEXT,"
                                    "CreatedProduct REAL)"};
static constexpr auto* insertTask{"INSERT INTO Tasks VALUES ('{}', '{}', {}, {}, '{}', '{}', {})"};
static constexpr auto* selectActiveTask {"SELECT * FROM Tasks WHERE CreatedProduct < ProductToDo"};
static constexpr auto* updateTaskData {"UPDATE Tasks SET '{}' = {} WHERE GUI = '{}'"};
}

TasksTable::TasksTable(const std::string& databaseName):m_tableName(databaseName), m_database(m_tableName, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)
{
    try
    {
     m_database.exec(requests::createTable);
    }
    catch (std::exception& e)
    {
        std::cout << e.what()<< std::endl;
    }
}

std::string TasksTable::wrapString(const std::string& text)
{
    return "'" + text + "'";
}

bool TasksTable::isTableExist() const
{
    return m_database.tableExists(m_tableName);
}

bool TasksTable::addTask(const Tasks::TaskData& taskData)
{
    bool result {true};
    try
    {
        std::string str = std::format(requests::insertTask,
                                      taskData.getIdentifier(),
                                      taskData.taskName,
                                      taskData.productToDoAmount,
                                      static_cast<int>(taskData.status),
                                      taskData.productName,
                                      taskData.releaseDate,
                                      taskData.doneProduct);
        m_database.exec(str);
    }
    catch (std::exception& exception)
    {
        result = false;
    }
    return result;
}

void TasksTable::updateTaskData(std::unique_ptr<TaskDataChangedCommand>&& command)
{
    try
    {
        m_database.exec(std::format(requests::updateTaskData,
                                    command->getChangingFieldName(),
                                    command->getChangingField(),
                                    command->getIdentifier()));
    }
    catch(std::exception& exc)
    {
    }
}

void TasksTable::updateDoneProduct(const GUI& gui, double doneProduct)
{
    try
    {
        m_database.exec(std::format(requests::updateTaskData, "CreatedProduct", doneProduct, gui));
    }
    catch (std::exception& exc)
    {
    }
}

std::vector<Tasks::TaskData> TasksTable::getTasks() const
{
    std::vector<Tasks::TaskData> result;
    try
    {
        SQLite::Statement query(m_database, requests::selectActiveTask);
        while (query.executeStep())
        {
            const std::string gui = query.getColumn(0);
            const std::string taskName = query.getColumn(1);
            const double productToDo = query.getColumn(2);
            const GeneralValues::PriorityStatus priority {static_cast<int>(query.getColumn(3))};
            const std::string product = query.getColumn(4);
            const std::string releaseDate = query.getColumn(5);
            const double createdProduct = query.getColumn(6);
            Tasks::TaskData taskData{taskName, product, releaseDate, priority, productToDo, createdProduct, gui};
            result.emplace_back(std::move(taskData));
        }
    }
    catch(std::exception& exception)
    {

    }
    return result;
}
