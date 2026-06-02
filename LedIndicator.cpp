#include "LedIndicator.hpp"

#include <QPainter>

#include "Resources.hpp"

LedIndicator::LedIndicator(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(20, 20);
}

void LedIndicator::setColor(const Color& color, const QString& toolTipText)
{
    if (color == Resources::birghtRed)
    {
        m_color = color;
    }
    else if (color == Resources::brightOrange)
    {
        m_color = color;
    }
    else if (color == Resources::deepBlue )
    {
        m_color = color;
    }
    else if (color == Resources::brightGreen)
    {
        m_color = color;
    }
    else if (color == Resources::classicGreen)
    {
        m_color = color;
    }
    setToolTip(toolTipText);
}

void LedIndicator::paintEvent(QPaintEvent *event)
{
    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(m_color.r, m_color.g, m_color.b, m_color.alpha));
    painter.setPen(Qt::black);

    painter.drawEllipse(rect().adjusted(1, 1, -1, -1));
    QWidget::paintEvent(event);
}

bool LedIndicator::isEmpty() const
{
    return m_color == Color();
}
