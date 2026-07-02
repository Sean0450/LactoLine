#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <format>
#include <vector>

#include "Shifts/ShiftData.hpp"
#include "Date/Date.hpp"

class ShiftsTable
{
    static constexpr auto* s_createTable {"CREATE TABLE IF NOT EXISTS ShiftsInfo ("
                                          "Gui TEXT PRIMARY KEY, "
                                          "Date TEXT , "
                                          "TaskName TEXT, "
                                          "DoneForShift REAL, "
                                          "NeedToDo REAL,"
                                          "IsComplete INTEGER)"}; // Удалить не нужное поле из БД
    static constexpr auto* s_addTaskInfo {"INSERT INTO ShiftsInfo VALUES ('{}', '{}', '{}', {}, {}, {})"};
    static constexpr auto* s_selectShifData {"SELECT * FROM ShiftsInfo WHERE Date = '{}'"};
    static constexpr auto* s_selectConcreteTaskData {"SELECT * FROM ShiftsInfo WHERE Date = '{}' AND TaskName = '{}'"};
    static constexpr auto* s_updateExistingValue {"UPDATE ShiftsInfo SET DoneForShift = DoneForShift + {}, IsComplete = {} WHERE Date = '{}' AND TaskName = '{}'"};
    SQLite::Database m_database;
public:
    explicit ShiftsTable(const std::string& databaseName):m_database(databaseName, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)
    {
        try
        {
            m_database.exec(s_createTable);
        }
        catch (std::exception& e)
        {
        }
    }
    std::vector<ShiftData> getData(const Date::Date& date)
    {
        std::vector<ShiftData> shiftsData;
        try
        {
            SQLite::Statement query(m_database, std::format(s_selectShifData, date.getDate()));
            while(query.executeStep())
            {
                const GeneralValues::GUI gui = query.getColumn(0);
                const std::string shiftDate = query.getColumn(1);
                const std::string taskName = query.getColumn(2);
                const double doneForShift = query.getColumn(3);
                const double needToDo = query.getColumn(4);
                shiftsData.emplace_back(ShiftData{shiftDate, taskName, doneForShift, needToDo, gui});
            }
        }
        catch(std::exception& excpt)
        {
        }
        return shiftsData;
    };

    bool addTaskInfo(const ShiftData& data)
    {
        bool result {true};
        try
        {
            if (!isTaskDataInShift(data))
                m_database.exec(std::format(s_addTaskInfo, data.getIdentifier(), data.date, data.taskName, data.doneForShift, data.needToDo, data.isComplete));
            else
                m_database.exec(std::format(s_updateExistingValue, data.doneForShift, data.isComplete, data.date, data.taskName));
        }
        catch(std::exception& exc)
        {
            result = false;
        }
        return result;
    }

    bool isTaskDataInShift(const ShiftData& data)
    {
        bool result {true};
        try
        {
            SQLite::Statement query(m_database, std::format(s_selectConcreteTaskData, data.date, data.taskName));
            result = query.executeStep();
        }
        catch(std::exception& exc)
        {
            result = false;
        }
        return result;
    }
};
