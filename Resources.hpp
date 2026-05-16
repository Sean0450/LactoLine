#pragma once

#include "Color.hpp"

namespace Resources
{
namespace Styles
{
static constexpr auto* baseElementsStyle {"border: 2px solid grey;"
                                         "border-radius: 5px;"
                                         "text-align: center;"
                                         "background-color: white;"};
static constexpr auto* pressedButtonStyle {"QPushButton:pressed {"
                                           "border: 5px solid grey;"
                                           "border-radius: 5px;"
                                           "text-align: center;"
                                           "background-color: grey;}"};
static constexpr auto* dropDownComboBoxStyle {"QComboBox::drop-down {"
                                              "   background-color: white;"
                                              "   border-left: 1px solid gray;"
                                              "   border-top-right-radius: 3px;"
                                              "   border-bottom-right-radius: 3px;"
                                              "}"
                                              "QComboBox::down-arrow {"
                                              "image: url(:/images/images/Arrow.png);"
                                              "width: 12px;"
                                              "height: 12px;"
                                              "}"
                                            };
}
static constexpr auto* baseFont {"Roboto"};
static constexpr Color extraPriority {252, 61, 3, 255};
static constexpr Color highPriority {255, 140, 0, 255};
static constexpr Color mediumPriority {30, 144, 255, 255};
static constexpr Color lowPriority {0, 250, 154, 255};
}
