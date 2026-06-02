#pragma once

#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

template <typename DtoObject, typename Command>
class BaseTable
{
protected:
    std::string m_tableName;
    SQLite::Database m_database;

    std::string wrapString(const std::string& text)
    {
        return "'" + text + "'";
    }
public:
    explicit BaseTable(const std::string& databaseName):m_tableName(databaseName), m_database(m_tableName, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE) {}
    virtual bool addData(const DtoObject& data) = 0;
    virtual std::vector<DtoObject> getData() = 0;
    virtual bool updateData(std::unique_ptr<Command>&& command) = 0;
    virtual ~BaseTable() = default;
};
