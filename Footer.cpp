#include "Footer.hpp"

#include <QDate>
#include <QLabel>
#include <QVBoxLayout>

#include "Resources.hpp"

Footer::Footer(QWidget *parent)
    : QWidget{parent},
    m_currentDate(QDate::currentDate().toString())
{
    auto* layout = new QVBoxLayout(this);
    m_mainLabel = new QLabel(this);
    m_mainLabel->setFont({Resources::baseFont, 14});
    m_mainLabel->setText(m_currentDate);
    layout->addWidget(m_mainLabel, Qt::AlignRight);
    setStyleSheet("border: 1.5px solid black;");
}
