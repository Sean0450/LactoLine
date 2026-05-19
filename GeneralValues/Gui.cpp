#include "Gui.hpp"

#include <random>

namespace GeneralValues
{
std::string Gui::generateGui()
{
    std::string gui;
    gui.reserve(s_guiSize);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distr(1000, 9999);
    for (int i = 0; i < s_sectionsCount; ++i)
    {
        gui += std::to_string(distr(gen));
        if (i != s_sectionsCount - 1)
            gui += '-';
    }
    return gui;
}
}
