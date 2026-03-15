#pragma once

#include <string>

namespace GeneralValues 
{
class Name
{
    std::string m_name;
public:
    Name() = default;
    explicit Name(std::string&& name);
    bool operator==(const Name& name) const = default;
    std::string name() const;
};
}
