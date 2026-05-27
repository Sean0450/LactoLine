#pragma once

#include <vector>
#include <memory>

#include "Commands/DataChangedCommand.hpp"
#include "Tasks/TaskData.hpp"
#include "Tasks/Task.hpp"

struct TaskChangedObserver
{
 virtual void taskDataChanged(std::unique_ptr<DataChangedCommand<Tasks::TaskData, Tasks::Task>>&& command) = 0;
 virtual std::vector<std::string> productNames() = 0;
 virtual ~TaskChangedObserver() = default;
};
