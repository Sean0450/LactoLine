#pragma once

#include <string>

namespace GeneralValues
{
using GUI = std::string;
class Gui
{
    static constexpr int s_guiSize {19};
    static constexpr int s_sectionsCount {4};
public:
    explicit Gui()=default;
    static std::string generateGui();
};
}
