#include "TaskManager.hpp"

namespace Tasks
{
TaskManager::TaskManager(): m_taskTable(s_tableName)
{
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
        Product productToCreate {Name("Milk"), Name("M"), Amount(0.5), 5.2, 0.001}; //TODO: заменить реальными значениями
        Date::Date releaseDate {std::string_view(data.releaseDate)};
        Task task(gui,
                  std::move(taskName),
                  productAmount,
                  std::move(taskPriority),
                  std::move(productToCreate),
                  std::move(releaseDate));
        if (data.doneProduct > 0.0)
        {
            task.addData(Amount(data.doneProduct));
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
    return TaskData{taskName, productName, releaseDate, status, productToDoAmount, doneProduct, gui};
}

void TaskManager::readTasksFromTable()
{
    auto tasksData = m_taskTable.getTasks();
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

void TaskManager::addTask(const TaskData& taskData)
{
    std::optional<Task> taskCreationResult = fromDataToTask(taskData);
    if (taskCreationResult)
    {
        m_tasks.emplace_back(std::move(taskCreationResult.value()));
        m_taskTable.addTask(taskData);
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

void TaskManager::taskDataChanged(std::unique_ptr<TaskDataChangedCommand>&& command)
{
    const GUI gui = command->getTaskData().getIdentifier();
    auto iterator = std::ranges::find_if(m_tasks, [gui](const auto& task){return task.gui() == gui;});
    if (iterator != std::ranges::end(m_tasks))
    {
        Task& task = *iterator;
        const TaskData data = command->getTaskData();
        task.changeTaskName(Name(data.taskName));
        task.changeTaskPriority(data.status);
        task.changeProductToDoAmount(Amount(data.productToDoAmount));
        //task.addData(Amount(data.doneProduct)); // Тогда меняется количество, причем неправильным образом, надо чет подумать
        m_taskTable.updateTaskData(std::move(command));
    }
}
}
