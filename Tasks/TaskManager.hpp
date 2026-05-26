#pragma once

#include <vector>
#include <optional>

#include "Task.hpp"
#include "Database/TasksTable.hpp"
#include "Database/ProductTable.hpp"
#include "Commands/DataChangedCommand.hpp"
#include "TaskChangedObserver.hpp"
#include "Product/ProductData.hpp"
#include "Product/ProductChangedObserver.hpp"

namespace Tasks
{
class TaskManager: public TaskChangedObserver,
                   public ProductChangedObserver
{
    static constexpr auto* s_taskTableName {"Tasks.db3"};
    static constexpr auto* s_productTableName {"Product.db3"};

    std::vector<Task> m_tasks;
    std::vector<Product> m_products;
    TasksTable m_taskTable;
    ProductTable m_productTable;

    std::optional<Task> fromDataToTask(const TaskData& data);
    TaskData fromTaskToData(const Task& task) const;
    std::optional<Product> fromDataToProduct(const ProductData& data);
    void readTasksFromTable();
    void readProductFromtable();
    Product findProductByName(const std::string& productName);
public:
    explicit TaskManager();
    void addTask(const TaskData& taskData);
    std::vector<TaskData> getCurrentShiftTaskData() const;
    std::vector<ProductData> getProductData() override;
    void createProduct(const ProductData& data) override;
    void taskDataChanged(std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command) override;
    std::vector<std::string> productNames() override;
};
}
