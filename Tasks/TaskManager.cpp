#include "TaskManager.hpp"

#include <algorithm>

namespace Tasks
{
TaskManager::TaskManager():m_taskTable(s_taskTableName), m_productTable(s_productTableName), m_shiftsTable(s_shiftTasksTableName)
{
    readProductFromtable();
    readTasksFromTable();
}

std::optional<Task> TaskManager::fromDataToTask(const TaskData& data)
{
    std::optional<Task> outputTask = std::nullopt;
    try
    {
        Product productToCreate = findProductByName(data.productName);
        Task task(data, productToCreate);
        task.changeTaskPriority(Date::Date(DateTranslator::getModelCurrentDate()));
        // Необходимо пересчитывать приоритет после считывания задачи из базы - даты сдвигаются. Возможно, стоит в принципе удалить поле "приоритет" из базы, если все равно нужен пересчёт
        outputTask = std::move(task);
    }
    catch(std::exception& exc)
    {
    }
    return outputTask;
}

void TaskManager::readTasksFromTable()
{
    auto tasksData = m_taskTable.getData();
    for (auto& task: tasksData)
    {
        try
        {
            std::optional<Task> taskCreationResult = fromDataToTask(task);
            if (taskCreationResult)
                m_tasks.emplace_back(std::move(taskCreationResult.value()));
        }
        catch(std::exception& exc)
        {
            //Удаляем таску, если хотя бы одно поле модели инвалидировалось
        }
    }
}

void TaskManager::readProductFromtable()
{
    auto data = m_productTable.getData();
    for (const auto& element: data)
    {
        std::optional<Product> product = fromDataToProduct(element);
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
    std::optional<Task> taskCreationResult = fromDataToTask(taskData);
    if (taskCreationResult)
    {
        m_tasks.emplace_back(std::move(taskCreationResult.value()));
        m_taskTable.addData(taskData);
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
        data.emplace_back(product.name(), product.category(), product.unitRowMaterials(), product.primeCost(), product.epsilon());
    }
    return data;
}

std::optional<Product> TaskManager::fromDataToProduct(const ProductData& data)
{
    std::optional<Product> result;
    try
    {
        Name productName(data.productName);
        Name categoryName(data.categoryName);
        Amount unitRawMaterials(data.unitRawMaterials);
        result = Product(std::move(productName), std::move(categoryName), std::move(unitRawMaterials), data.primeCost, data.epsilon);
    }
    catch(std::exception& exc)
    {
        result = std::nullopt;
    }
    return result;
}

void TaskManager::createProduct(const ProductData& data)
{
    bool isNameCorrect = std::ranges::none_of(m_products, [&](const auto& product){return product.name() == data.productName;});
    std::optional<Product> product = fromDataToProduct(data);
    if (isNameCorrect && product)
    {
        m_productTable.addData(data);
        m_products.emplace_back(std::move(product.value()));
    }
}

void TaskManager::taskDataChanged(std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command)
{
    const GUI gui = command->getIdentifier();
    auto iterator = std::ranges::find_if(m_tasks, [gui](const auto& task){return task.getTaskIdentifier() == gui;});
    if (iterator != std::ranges::end(m_tasks))
    {
        auto updatedTask = (command->getUpdatedTask(*iterator)).getTaskData();
        TaskData taskData = iterator->getTaskData();
        Date::Date currentDate(DateTranslator::getModelCurrentDate());
        auto shiftData = ShiftData(currentDate.getDate(),
                                   taskData.taskName,
                                   updatedTask.doneProduct - taskData.doneProduct,
                                   taskData.productToDoAmount,
                                   updatedTask.doneProduct >= updatedTask.productToDoAmount,
                                   GeneralValues::Gui::generateGui());
        m_shiftsTable.addTaskInfo(shiftData);
        *iterator = Task{updatedTask, findProductByName(updatedTask.productName)};
        m_taskTable.updateData(std::move(command));
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
    std::vector<ShiftData> shiftTasks = m_shiftsTable.getData(Date::Date(data));
    return shiftTasks;
}

void TaskManager::generatePackingListDocumentation(const std::vector<std::string>& goods)
{

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
        TaskData taskData = iterator->getTaskData();
        auto shiftData = ShiftData(DateTranslator::getModelCurrentDate(),
                                   taskData.taskName,
                                   doneAmount,
                                   taskData.productToDoAmount,
                                   iterator->isTaskDone(),
                                   GeneralValues::Gui::generateGui());
        m_shiftsTable.addTaskInfo(shiftData);
        m_taskTable.doneProductAmountChanged(gui, doneAmount, rawMaterial);
    }
}
}
