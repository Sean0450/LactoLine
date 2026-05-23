#include "Footer.hpp"

#include <QDate>
#include <QLabel>
#include <QVBoxLayout>

#include "Resources.hpp"

Footer::Footer(QWidget *parent)
    : QWidget{parent},
    m_currentDate(QDateTime::currentDateTime().toString(QLatin1String(Resources::dateParseFormat)))
{
    auto* layout = new QVBoxLayout(this);
    m_mainLabel = new QLabel(QStringLiteral("Смена от ") + m_currentDate, this);
    m_mainLabel->setFont({Resources::baseFont, 14});
    m_mainLabel->setAlignment(Qt::AlignRight);

    layout->addWidget(m_mainLabel);
    setStyleSheet(QStringLiteral("border: 1.5px solid black;"));
}
