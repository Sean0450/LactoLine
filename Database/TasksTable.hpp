#pragma once

#include <format>

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
                                          "Product TEXT,"
                                          "ReleaseDate TEXT,"
                                          "CreatedProduct REAL,"
                                          "WastedRawMaterials REAL)"};
    static constexpr auto* s_insertTask{"INSERT INTO Tasks VALUES ('{}', '{}', {}, '{}', '{}', {}, {})"};
    static constexpr auto* s_selectActiveTask {"SELECT * FROM Tasks WHERE CreatedProduct < ProductToDo"};
    static constexpr auto* s_updateTaskData {"UPDATE Tasks SET '{}' = {} WHERE GUI = '{}'"};
    static constexpr auto* s_updateDoneProduct {"UPDATE Tasks SET CreatedProduct = CreatedProduct + {}, WastedRawMaterials = WastedRawMaterials + {} WHERE GUI = '{}'"};
    static constexpr auto* s_deleteDoneTask {"DELETE FROM Tasks WHERE GUI = '{}'"};
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
            SQLite::Statement query(m_database, s_selectActiveTask);
            Date::Date currentDate(DateTranslator::getModelCurrentDate());
            while (query.executeStep())
            {
                const std::string gui = query.getColumn(0);
                const std::string releaseDate = query.getColumn(4);
                if (!currentDate.isDateMore(Date::Date(releaseDate)))
                {
                    const std::string taskName = query.getColumn(1);
                    const double productToDo = query.getColumn(2);
                    const std::string product = query.getColumn(3);
                    const double createdProduct = query.getColumn(5);
                    const double wastedRawMaterials = query.getColumn(6);
                    Tasks::TaskData taskData{taskName, product, releaseDate, GeneralValues::PriorityStatus::Low, productToDo, createdProduct, wastedRawMaterials, gui};
                    // На приоритет выставляется заглушка, он все равно пересчитывается позднее
                    result.emplace_back(std::move(taskData));
                }
                else
                {
                    m_database.exec(std::format(s_deleteDoneTask, gui));
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
                                        doneProduct,
                                        wastedRawMaterials,
                                        gui));
        }
        catch(std::exception& exc)
        {
            result = false;
        }
        return result;
    }
};
