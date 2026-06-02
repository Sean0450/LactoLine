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
static constexpr auto* redBorderLineEdit {"QLineEdit {"
                                          "border: 2px solid red;"
                                          "border-radius: 5px;"
                                          "text-align: center;"
                                          "background-color: white;}"};
static constexpr auto* baseLineEdit {"QLineEdit {"
                                     "border: 2px solid grey;"
                                     "border-radius: 5px;"
                                     "text-align: center;"
                                     "background-color: white;}"};
static constexpr auto* plusButton {"QPushButton{ border: 2px solid grey;"
                                   "border-radius: 5px;"
                                   "image: url(:/images/images/Plus.png);"
                                   "width: 17px;"
                                   "height: 17px;"
                                   "text-align: center;"
                                   "background-color: white;}"
                                   "QPushButton:pressed {"
                                   "border: 5px solid grey;"
                                   "image: url(:/images/images/Plus.png);"
                                   "width: 17px;"
                                   "height: 17px;"
                                   "border-radius: 5px;"
                                   "text-align: center;"
                                   "background-color: grey;}"};
static constexpr auto* deleteButton {"QPushButton{ border: 2px solid grey;"
                                     "border-radius: 5px;"
                                     "image: url(:/images/images/Delete.png);"
                                     "width: 17px;"
                                     "height: 17px;"
                                     "text-align: center;"
                                     "background-color: white;}"
                                     "QPushButton:pressed {"
                                     "border: 5px solid grey;"
                                     "image: url(:/images/images/Delete.png);"
                                     "width: 17px;"
                                     "height: 17px;"
                                     "border-radius: 5px;"
                                     "text-align: center;"
                                     "background-color: grey;}"};
}
static constexpr auto* baseFont {"Roboto"};
static constexpr auto* dateParseFormat {"dd.MM.yyyy"};
static constexpr Color birghtRed {252, 61, 3, 255};
static constexpr Color brightOrange {255, 140, 0, 255};
static constexpr Color deepBlue {30, 144, 255, 255};
static constexpr Color brightGreen {0, 250, 154, 255};
static constexpr Color classicGreen {0, 128, 0, 255};
}
