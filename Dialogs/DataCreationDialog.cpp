#include "DataCreationDialog.hpp"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

DataCreationDialog::DataCreationDialog(QWidget *parent)
    : QDialog{parent}
{
    setModal(true);
    m_mainLayot = new QVBoxLayout(this);
}

void DataCreationDialog::createButton(const QString& buttonName, std::function<void()>&& slot)
{
    if (!m_createButton)
    {
        m_createButton = new QPushButton(buttonName, this);
        m_createButton->setFont(m_baseFont);
        m_createButton->setStyleSheet(QStringLiteral("QPushButton {") + Resources::Styles::baseElementsStyle + "}" + Resources::Styles::pressedButtonStyle);
        connect (m_createButton, &QPushButton::clicked, this, [&, slotFunction = std::move(slot)](){slotFunction();
                                                                                                    close();});
        m_createButton->setEnabled(false);

        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(m_createButton);
        m_mainLayot->addLayout(buttonLayout);
    }
}

void DataCreationDialog::createLineEditLayout(QLineEdit* lineEdit, const QString& labelText, std::function<bool()>&& slot)
{
    auto* layout = new QHBoxLayout();
    connect(lineEdit, &QLineEdit::textChanged, this, [&, slotFunction = std::move(slot), widget = lineEdit](){onLineEditCheck(widget, slotFunction());});

    auto* label = new QLabel(labelText, this);
    layout->setSpacing(5);
    label->setFont(m_baseFont);
    layout->addWidget(label);
    layout->addStretch(1);
    layout->addWidget(lineEdit);

    m_mainLayot->addLayout(layout);
}

QLineEdit* DataCreationDialog::createLineEdit()
{
    auto* lineEdit = new QLineEdit(this);
    lineEdit->setStyleSheet(m_baseStyle);
    lineEdit->setFont(m_baseFont);
    lineEdit->setMinimumWidth(250);
    m_lineEdits.emplace_back(lineEdit);
    return lineEdit;
}

void DataCreationDialog::onLineEditCheck(QLineEdit* lineEdit, bool flag)
{
    if (flag)
        lineEdit->setStyleSheet(m_baseStyle);
    else
        lineEdit->setStyleSheet(Resources::Styles::redBorderLineEdit);
    enableCreateButton();
}

void DataCreationDialog::enableCreateButton()
{
    if (std::ranges::none_of(m_lineEdits, [&](auto* element){return element->text().isEmpty() ||
                                                                    element->styleSheet() == Resources::Styles::redBorderLineEdit;}))
    {
        m_createButton->setEnabled(true);
    }
    else
    {
        m_createButton->setEnabled(false);
    }
}
