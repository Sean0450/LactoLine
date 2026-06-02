#pragma once

#include <format>
#include <chrono>

#include "BaseTable.hpp"
#include "Tasks/TaskData.hpp"
#include "Tasks/Task.hpp"
#include "Date/DateTranslator.hpp"
#include "Commands/DataChangedCommand.hpp"

class TasksTable: public BaseTable<Tasks::TaskData, DataChangedCommand<Tasks::TaskData, Tasks::Task>>
{
    static constexpr auto* s_createTable {"CREATE TABLE IF NOT EXISTS Tasks ("
                                          "GUI TEXT PRIMARY KEY, "
                                          "Name TEXT, "
                                          "ProductToDo REAL,"
                                          "Priority INTEGER,"
                                          "Product TEXT,"
                                          "ReleaseDate TEXT,"
                                          "WastedRawMaterials REAL,"
                                          "CreatedProduct REAL)"};
    static constexpr auto* s_insertTask{"INSERT INTO Tasks VALUES ('{}', '{}', {}, {}, '{}', '{}', {}, {})"};
    static constexpr auto* s_selectActiveTask {"SELECT * FROM Tasks WHERE CreatedProduct < ProductToDo"};
    static constexpr auto* s_updateTaskData {"UPDATE Tasks SET '{}' = {} WHERE GUI = '{}'"};
    static constexpr auto* s_updateDoneProduct {"UPDATE Tasks SET WastedRawMaterials = {}, CreatedProduct = {} WHERE GUI = '{}'"};
public:
    explicit TasksTable(const std::string& databaseName): BaseTable(databaseName)
    {
        try
        {
            m_database.exec(s_createTable);
        }
        catch (std::exception& e)
        {
        }
    }
    bool addData(const Tasks::TaskData& taskData) override
    {
        bool result {true};
        try
        {
            std::string str = std::format(s_insertTask,
                                          taskData.getIdentifier(),
                                          taskData.taskName,
                                          taskData.productToDoAmount,
                                          static_cast<int>(taskData.status),
                                          taskData.productName,
                                          taskData.releaseDate,
                                          taskData.doneProduct,
                                          taskData.wastedRawMaterials);
            m_database.exec(str);
        }
        catch (std::exception& exception)
        {
            result = false;
        }
        return result;
    }
    bool updateData(std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command) override
    {
        bool result {true};
        try
        {
            m_database.exec(std::format(s_updateTaskData,
                                        command->getChangingFieldName(),
                                        command->getChangingField(),
                                        command->getIdentifier()));
        }
        catch(std::exception& exc)
        {
            result = false;
        }
        return result;
    }
    std::vector<Tasks::TaskData> getData() override
    {
        std::vector<Tasks::TaskData> result;
        try
        {
            using namespace std::chrono;
            SQLite::Statement query(m_database, s_selectActiveTask);
            auto now = std::chrono::system_clock::now();
            year_month_day date{std::chrono::floor<days>(now)};
            Date::Date currentDate(DateTranslator::getModelCurrentDate());
            while (query.executeStep())
            {
                const std::string releaseDate = query.getColumn(5);
                if (!currentDate.isDateMore(Date::Date(releaseDate)))
                {
                    const std::string gui = query.getColumn(0);
                    const std::string taskName = query.getColumn(1);
                    const double productToDo = query.getColumn(2);
                    const GeneralValues::PriorityStatus priority {static_cast<int>(query.getColumn(3))};
                    const std::string product = query.getColumn(4);
                    const double wastedRawMaterials = query.getColumn(6);
                    const double createdProduct = query.getColumn(7);
                    Tasks::TaskData taskData{taskName, product, releaseDate, priority, productToDo, createdProduct, wastedRawMaterials, gui};
                    result.emplace_back(std::move(taskData));
                }
            }
        }
        catch(std::exception& exception)
        {
        }
        return result;
    }

    bool doneProductAmountChanged(const GUI& gui, double doneProduct, double wastedRawMaterials)
    {
        bool result {true};
        try
        {
            m_database.exec(std::format(s_updateDoneProduct,
                                        wastedRawMaterials,
                                        doneProduct,
                                        gui));
        }
        catch(std::exception& exc)
        {
            result = false;
        }
        return result;
    }
};
