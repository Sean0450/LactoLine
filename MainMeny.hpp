#pragma once

#include <QFont>
#include "Resources.hpp"

QT_FORWARD_DECLARE_CLASS(QListWidget)

class MainMenu
{
    static inline constexpr auto* s_taskList {"Список задач"};
    static inline constexpr auto* s_wareHouse {"Склад"};
    static inline constexpr auto* s_productList {"Каталог товаров"};
    static inline constexpr auto* s_shiftInformation {"Статистика по сменам"};
    static inline constexpr int s_minWidth {260};
    const QFont m_baseFont {Resources::baseFont, 16};
    QListWidget* m_menu {nullptr};

    void setVisibleSettings();
    void createItem(const char* text) const;
public:
    explicit MainMenu(QWidget* parent = nullptr);
    void setMinSizes(int height, int width = s_minWidth);
    QListWidget* getListObject();
};
