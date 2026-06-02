#pragma once

#include <QWidget>

#include "Color.hpp"

class LedIndicator : public QWidget
{
    Q_OBJECT
    Color m_color;
protected:
    void paintEvent(QPaintEvent* event) override;
public:
    explicit LedIndicator(QWidget* parent = nullptr);
    void setColor(const Color& color, const QString& toolTipText);
    bool isEmpty() const;
};
