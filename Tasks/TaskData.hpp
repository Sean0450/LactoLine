#pragma once

#include <string>

#include "GeneralValues/Priority.hpp"
#include "GeneralValues/Gui.hpp"

namespace Tasks
{
struct TaskData
{
    std::string taskName;
    std::string productName;
    std::string releaseDate;
    GeneralValues::PriorityStatus status;
    double productToDoAmount {0.0};
    double doneProduct {0.0};

    explicit TaskData() = default;
    explicit TaskData(const std::string& taskName,
                      const std::string& productName,
                      const std::string& releaseDate,
                      GeneralValues::PriorityStatus status,
                      double productToDoAmount,
                      double doneProduct,
                      const GeneralValues::GUI& identifier):taskName(taskName),
                                                            productName(productName),
                                                            releaseDate(releaseDate),
                                                            status(status),
                                                            productToDoAmount(productToDoAmount),
                                                            doneProduct(doneProduct),
                                                            identifier(identifier)
    {
    }

    GeneralValues::GUI getIdentifier() const noexcept {return identifier;}
private:
    GeneralValues::GUI identifier;
};
}
