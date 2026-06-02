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
    bool result {true};
    Task outputTask;
    try
    {
        const GUI gui = data.getIdentifier();
        Name taskName {std::move(data.taskName)};
        Amount productAmount{data.productToDoAmount};
        Priority taskPriority {data.status, Date::Date(data.releaseDate)};
        Product productToCreate = findProductByName(data.productName);
        Date::Date releaseDate {std::string_view(data.releaseDate)};
        Task task(gui,
                  std::move(taskName),
                  productAmount,
                  std::move(taskPriority),
                  std::move(productToCreate),
                  std::move(releaseDate));
        if (data.doneProduct > 0.0)
        {
            task.addData(Amount(data.doneProduct), Amount(data.wastedRawMaterials));
        }
        outputTask = std::move(task);
    }
    catch(std::exception& exc)
    {
        result = false;
    }
    return result ? std::optional(outputTask) : std::nullopt;
}

TaskData TaskManager::fromTaskToData(const Task& task) const
{
    const GUI gui = task.gui();
    const std::string taskName = task.taskName();
    const std::string productName = task.productName();
    std::string releaseDate = task.releaseDate();
    GeneralValues::PriorityStatus status = static_cast<GeneralValues::PriorityStatus>(task.taskPriority());
    double doneProduct = task.createdProduct();
    double productToDoAmount = task.leftToDo() + doneProduct;
    double wastedRawMaterials = task.wastedRawMaterials();
    return TaskData{taskName, productName, releaseDate, status, productToDoAmount, doneProduct, wastedRawMaterials, gui};
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
        data.emplace_back(fromTaskToData(task));
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
    auto iterator = std::ranges::find_if(m_tasks, [gui](const auto& task){return task.gui() == gui;});
    if (iterator != std::ranges::end(m_tasks))
    {
        auto updatedTask = command->getUpdatedTask(*iterator);
        Date::Date currentDate(DateTranslator::getModelCurrentDate());
        auto shiftData = ShiftData(currentDate.getDate(),
                                   iterator->taskName(),
                                   updatedTask.createdProduct() - iterator->createdProduct(),
                                   iterator->productAmount(),
                                   updatedTask.isTaskDone(),
                                   GeneralValues::Gui::generateGui());
        m_shiftsTable.addTaskInfo(shiftData);
        *iterator = updatedTask;
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
    auto iterator = std::ranges::find_if(m_tasks, [gui](const auto& task){return task.gui() == gui;});
    if (iterator != std::ranges::end(m_tasks))
    {
        double previosDone = iterator->createdProduct();
        iterator->addData(Amount(doneAmount), Amount(rawMaterial));
        auto shiftData = ShiftData(DateTranslator::getModelCurrentDate(),
                                   iterator->taskName(),
                                   iterator->createdProduct() - previosDone,
                                   iterator->productAmount(),
                                   iterator->isTaskDone(),
                                   GeneralValues::Gui::generateGui());
        m_shiftsTable.addTaskInfo(shiftData);
        m_taskTable.doneProductAmountChanged(gui, doneAmount, rawMaterial);
    }
}
}
