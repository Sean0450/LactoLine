#include "Dialogs/TaskDataDialog.hpp"
#include "Resources.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

TaskDataDialog::TaskDataDialog(double createdProduct, QWidget* parent):QDialog{parent},
                                                                       m_createdProduct(createdProduct)
{
    setModal(true);
    setWindowTitle("Lactoline");
    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(5);

    m_description = new QLabel(this);
    m_description->setFont(m_baseFont);
    m_description->setText("Введите количество произведенной продукции:");
    mainLayout->addWidget(m_description);

    m_enterField = new QLineEdit(this);
    m_enterField->setFont(m_baseFont);
    m_enterField->setText(QString::number(createdProduct));
    connect(m_enterField, &QLineEdit::textChanged, this, [&](const QString& text){onProductAmountChanged(text);});
    mainLayout->addWidget(m_enterField);

    m_enterButton = new QPushButton(this);
    m_enterButton->setFont(m_baseFont);
    m_enterButton->setText("Сохранить");
    m_enterButton->setStyleSheet("QPushButton {" + QString::fromUtf8(Resources::Styles::baseElementsStyle) + "}" +
                                 QString::fromUtf8(Resources::Styles::pressedButtonStyle));
    connect(m_enterButton, &QPushButton::clicked, this, &TaskDataDialog::onEnterButtonClicked);
    mainLayout->addWidget(m_enterButton);
}


void TaskDataDialog::lockDataSending()
{
    m_enterButton->setEnabled(false);
    m_enterField->setStyleSheet("QLineEdit { border: 2px solid red; }");
}

void TaskDataDialog::onProductAmountChanged(const QString& text)
{
    bool flag {true};
    double result = text.toDouble(&flag);
    if (flag)
    {
        if (result < m_createdProduct)
        {
            lockDataSending();
        }
        else
        {
            m_enterButton->setEnabled(true);
            m_enterField->setStyleSheet("");
        }
    }
    else
    {
        lockDataSending();
    }
}

void TaskDataDialog::onEnterButtonClicked()
{
    m_createdProduct = m_enterField->text().toDouble();
    close();
}

double TaskDataDialog::getCreatedProduct() const
{
    return m_createdProduct;
}
