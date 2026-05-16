#pragma once

#include <QWidget>

#include "Color.hpp"

class LedIndicator : public QWidget
{
    Q_OBJECT
    static constexpr auto* s_extraPriorityText {"Важнейшая задача"};
    static constexpr auto* s_highPriorityText {"Приоритетная задача"};
    static constexpr auto* s_mediumPriorityText {"Задача средней важности"};
    static constexpr auto* s_lowPriorityText {"Второстепенная задача"};
    Color m_color;
protected:
    void paintEvent(QPaintEvent* event) override;
public:
    explicit LedIndicator(QWidget* parent = nullptr);
    void setColor(const Color& color);
    bool isEmpty() const;
};
