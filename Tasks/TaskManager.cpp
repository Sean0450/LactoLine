#include "TaskManager.hpp"

#include <algorithm>

namespace Tasks
{
TaskManager::TaskManager()
{
    readProductFromtable();
    readTasksFromTable();
}

void TaskManager::readTasksFromTable()
{
    std::vector<TaskData> tasksData = m_tablesManager.getTaskData();
    for (auto& task: tasksData)
    {
        try
        {
            std::optional<Task> taskCreationResult = Task::fromDataToTask(task, findProductByName(task.productName));
            if (taskCreationResult)
                m_tasks.emplace_back(std::move(taskCreationResult.value()));
        }
        catch(std::exception& exc)
        {
        }
    }
}

void TaskManager::readProductFromtable()
{
    auto data = m_tablesManager.getProductData();
    for (const auto& element: data)
    {
        std::optional<Product> product = Product::fromDataToProduct(element);
        if (product)
        {
            m_products.emplace_back(std::move(product.value()));
        }
    }
}

Product TaskManager::findProductByName(const std::string& productName)
{
    Product result;
    auto iterator = std::ranges::find_if(m_products, [&](const auto& product){return product.name() == productName;});
    if (iterator != std::ranges::end(m_products))
    {
        result = *iterator;
    }
    return result;
}

void TaskManager::addTask(const TaskData& taskData)
{
    std::optional<Task> taskCreationResult = Task::fromDataToTask(taskData, findProductByName(taskData.productName));
    if (taskCreationResult && m_tablesManager.addTask(taskData))
    {
        m_tasks.emplace_back(std::move(taskCreationResult.value()));
    }
}

std::vector<TaskData> TaskManager::getCurrentShiftTaskData() const
{
    std::vector<TaskData> data;
    for (const auto& task: m_tasks)
    {
        data.emplace_back(task.getTaskData());
    }
    return data;
}

std::vector<ProductData> TaskManager::getProductData()
{
    std::vector<ProductData> data;
    for (const auto& product: m_products)
    {
        data.emplace_back(product.getProductData());
    }
    return data;
}

void TaskManager::createProduct(const ProductData& data)
{
    bool isNameCorrect = std::ranges::none_of(m_products, [&](const auto& product){return product.name() == data.productName;});
    std::optional<Product> product = Product::fromDataToProduct(data);
    if (isNameCorrect && product && m_tablesManager.addProductData(data))
        m_products.emplace_back(std::move(product.value()));
}

void TaskManager::taskDataChanged(std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command)
{
    const GUI gui = command->getIdentifier();
    auto iterator = std::ranges::find_if(m_tasks, [gui](const auto& task){return task.getTaskIdentifier() == gui;});
    if (iterator != std::ranges::end(m_tasks))
    {
        TaskData updatedTaskData = (command->getUpdatedTask(*iterator)).getTaskData();
        ShiftData shiftData = createTaskChangeEntry(gui, updatedTaskData.taskName, DateTranslator::getCurrentDate(), updatedTaskData.productToDoAmount, updatedTaskData.doneProduct);
        if (m_tablesManager.updateTaskData(shiftData, std::move(command)))
            *iterator = Task{updatedTaskData, findProductByName(updatedTaskData.productName)};
    }
}

std::vector<std::string> TaskManager::productNames()
{
    std::vector<std::string> names;
    for (const auto& product: m_products)
    {
        names.emplace_back(product.name());
    }
    return names;
}

std::vector<ShiftData> TaskManager::getTasksData(const std::string& data)
{
    std::vector<ShiftData> shiftTasks = m_tablesManager.getShiftInfo(data);
    return shiftTasks;
}

std::vector<std::string> TaskManager::getProductNames()
{
    return productNames();
}

void TaskManager::doneProductAmountChanged(const GUI& gui, double doneAmount, double rawMaterial)
{
    auto iterator = std::ranges::find_if(m_tasks, [gui](const auto& task){return task.getTaskIdentifier() == gui;});
    if (iterator != std::ranges::end(m_tasks))
    {
        iterator->addData(doneAmount, rawMaterial);
        TaskData data = iterator->getTaskData();
        m_tablesManager.doneProductAmountChanged(createTaskChangeEntry(gui, data.taskName, DateTranslator::getCurrentDate(), data.productToDoAmount, doneAmount),
                                                 gui,
                                                 doneAmount,
                                                 rawMaterial);
    }
}

ShiftData TaskManager::createTaskChangeEntry(const GUI& gui,
                                             const std::string& taskName,
                                             const std::string& date,
                                             double needToDo,
                                             double done)
{
    ShiftData shiftData;
    shiftData.setGUI(gui);
    shiftData.taskName = taskName;
    shiftData.date = date;
    shiftData.isComplete = needToDo <= done;
    shiftData.needToDo = needToDo;
    shiftData.doneForShift = done;
    return shiftData;
}
}
