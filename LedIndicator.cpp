#include "LedIndicator.hpp"

#include <QPainter>

#include "Resources.hpp"

LedIndicator::LedIndicator(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(20, 20);
}

void LedIndicator::setColor(const Color& color)
{
    if (color == Resources::extraPriority)
    {
        m_color = color;
        setToolTip(QLatin1String(s_extraPriorityText));
    }
    else if (color == Resources::highPriority)
    {
        m_color = color;
        setToolTip(QLatin1String(s_highPriorityText));
    }
    else if (color == Resources::mediumPriority )
    {
        m_color = color;
        setToolTip(QLatin1String(s_mediumPriorityText));
    }
    else if (color == Resources::lowPriority)
    {
        m_color = color;
        setToolTip(QLatin1String(s_lowPriorityText));
    }
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
