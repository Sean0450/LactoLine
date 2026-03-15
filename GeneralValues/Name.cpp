#include "Name.hpp"

#include <stdexcept>

namespace GeneralValues 
{
Name::Name(std::string&& name)
{
    if (name.empty())
        throw std::runtime_error("Name can't be empty");
    m_name = std::move(name);
}

std::string Name::name() const
{
    if (m_name.empty())
        throw std::runtime_error("Name is empty");
    return m_name;
}
}
