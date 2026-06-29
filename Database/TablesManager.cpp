#include "TablesManager.hpp"

TablesManager::TablesManager():m_taskTable(s_taskTableName), m_productTable(s_productTableName), m_shiftsTable(s_shiftTasksTableName)
{
}

bool TablesManager::addTask(const Tasks::TaskData& taskData)
{
    return m_taskTable.addData(taskData);
}

std::vector<Tasks::TaskData> TablesManager::getTaskData()
{
    return m_taskTable.getData();
}

std::vector<ProductData> TablesManager::getProductData()
{
    return m_productTable.getData();
}

bool TablesManager::addProductData(const ProductData& data)
{
    return m_productTable.addData(data);
}

bool TablesManager::updateTaskData(const ShiftData& data,
                                   std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command)
{
    return m_shiftsTable.addTaskInfo(data) && m_taskTable.updateData(std::move(command));
}

bool TablesManager::doneProductAmountChanged(const ShiftData& data,
                                             const GUI& gui,
                                             double doneAmount,
                                             double rawMaterial)
{
    return m_shiftsTable.addTaskInfo(data) && m_taskTable.doneProductAmountChanged(gui, doneAmount, rawMaterial);
}

std::vector<ShiftData> TablesManager::getShiftInfo(const std::string& date)
{
    return m_shiftsTable.getData(Date::Date(date));
}
