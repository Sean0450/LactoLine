#pragma once

#include "Database/TasksTable.hpp"
#include "Database/ProductTable.hpp"
#include "Database/ShiftsTable.hpp"
#include "Tasks/TaskData.hpp"

class TablesManager
{
    static constexpr auto* s_taskTableName {"Tasks.db3"};
    static constexpr auto* s_productTableName {"Product.db3"};
    static constexpr auto* s_shiftTasksTableName {"ShiftInfo.db3"};

    TasksTable m_taskTable;
    ProductTable m_productTable;
    ShiftsTable m_shiftsTable;
public:
    explicit TablesManager();
    bool addTask(const Tasks::TaskData& taskData);
    std::vector<Tasks::TaskData> getTaskData();
    std::vector<ProductData> getProductData();
    bool addProductData(const ProductData& data);
    bool updateTaskData(const ShiftData& data,
                        std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command);
    bool doneProductAmountChanged(const ShiftData& data,
                                  const GUI& gui,
                                  double doneAmount,
                                  double rawMaterial);
    std::vector<ShiftData> getShiftInfo(const std::string& date);
};
