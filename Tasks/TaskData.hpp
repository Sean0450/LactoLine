#pragma once

#include <string>
#include "GeneralValues/Priority.hpp"

namespace Tasks
{
struct TaskData
{
    std::string taskName;
    std::string productName;
    std::string releaseDate;
    GeneralValues::PriorityStatus status;
    int productToDoAmount {0};
    int doneProduct {0};

    explicit TaskData() = default;
    explicit TaskData(const std::string& taskName,
                      const std::string& productName,
                      const std::string& releaseDate,
                      GeneralValues::PriorityStatus status,
                      int productToDoAmount,
                      int doneProduct,
                      const std::string& identifier):taskName(taskName),
                                                     productName(productName),
                                                     releaseDate(releaseDate),
                                                     status(status),
                                                     productToDoAmount(productToDoAmount),
                                                     doneProduct(doneProduct),
                                                     identifier(identifier)
    {
    }

    std::string getIdentifier() const noexcept {return identifier;}
private:
    std::string identifier;
};
}
