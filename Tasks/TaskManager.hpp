#pragma once

#include <vector>
#include <optional>

#include "Task.hpp"
#include "Commands/DataChangedCommand.hpp"
#include "TaskChangedObserver.hpp"
#include "Product/ProductData.hpp"
#include "Product/ProductChangedObserver.hpp"
#include "Shifts/ShiftTaskInformationObserver.hpp"
#include "PackingList/PackingListObserver.hpp"
#include "Database/TablesManager.hpp"

namespace Tasks
{
class TaskManager: public TaskChangedObserver,
                   public ProductChangedObserver,
                   public ShiftTaskInformationObserver,
                   public PackingListObserver
{
    std::vector<Task> m_tasks;
    std::vector<Product> m_products;
    TablesManager m_tablesManager;
    std::optional<Task> fromDataToTask(const TaskData& data);
    std::optional<Product> fromDataToProduct(const ProductData& data);
    void readTasksFromTable();
    void readProductFromtable();
    Product findProductByName(const std::string& productName);
    ShiftData createTaskChangeEntry(const GUI& gui,
                                    const std::string& taskName,
                                    const std::string& date,
                                    double needToDo,
                                    double done);
public:
    explicit TaskManager();
    void addTask(const TaskData& taskData);
    std::vector<TaskData> getCurrentShiftTaskData() const;
    void doneProductAmountChanged(const GUI& gui, double doneAmount, double rawMaterial) override;
    std::vector<ProductData> getProductData() override;
    void createProduct(const ProductData& data) override;
    void taskDataChanged(std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command) override;
    std::vector<std::string> productNames() override;
    std::vector<ShiftData> getTasksData(const std::string& data) override;
    std::vector<std::string> getProductNames() override;
};
}
