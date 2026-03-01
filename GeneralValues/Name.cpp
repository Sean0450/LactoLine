#include "Name.hpp"

#include <stdexcept>

namespace GeneralValues 
{
Name::Name(std::string&& name)
{
    if (name.empty())
    {
        throw std::runtime_error("Имя должно содержать хотя бы 1 символ");
    }
    m_name = std::move(name);
}

std::string Name::name() const
{
    return m_name;
}
}
