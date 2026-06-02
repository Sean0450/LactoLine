#include "GoodWidget.hpp"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCompleter>
#include <QMouseEvent>

GoodWidget::GoodWidget(const QStringList& productNames, int number, QWidget *parent)
    : QWidget{parent},
      m_productNames(productNames)
{
    auto* mainLayout = new QHBoxLayout(this);
    m_numberLabel = new QLabel(QString::number(number) + ".", this);
    m_numberLabel->setFont(m_baseFont);
    mainLayout->addWidget(m_numberLabel);
    mainLayout->setStretchFactor(m_numberLabel, 0);
    if (number < 10)
        mainLayout->addSpacing(10);
    else if (number >= 10 && number < 100)
        mainLayout->addSpacing(1);
    else
        mainLayout->addSpacing(1);
    m_goodName = new QLineEdit(this);
    m_goodName->setStyleSheet(Resources::Styles::baseLineEdit);
    m_goodName->setPlaceholderText(QStringLiteral("Наименование продукта"));
    m_goodName->setFont(m_baseFont);
    mainLayout->addWidget(m_goodName);
    mainLayout->setStretchFactor(m_goodName, 21);

    auto* completer = new QCompleter(m_productNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_goodName->setCompleter(completer);

    m_goodAmount = new QLineEdit(this);
    m_goodAmount->setStyleSheet(Resources::Styles::baseLineEdit);
    m_goodAmount->setPlaceholderText(QStringLiteral("Кол-во"));
    m_goodAmount->setFont(m_baseFont);
    connect(m_goodAmount, &QLineEdit::textChanged, this, [&](){onLineEditChanged(m_goodAmount);});
    mainLayout->addWidget(m_goodAmount);
    mainLayout->setStretchFactor(m_goodAmount, 5);

    auto* xLabel = new QLabel(QStringLiteral("X"));
    xLabel->setFont(m_baseFont);
    mainLayout->addWidget(xLabel);
    mainLayout->setStretchFactor(xLabel, 0);

    m_goodCost = new QLineEdit(this);
    m_goodCost->setStyleSheet(Resources::Styles::baseLineEdit);
    m_goodCost->setPlaceholderText(QStringLiteral("Цена"));
    m_goodCost->setFont(m_baseFont);
    connect(m_goodCost, &QLineEdit::textChanged, this, [&](){onLineEditChanged(m_goodCost);});
    mainLayout->addWidget(m_goodCost);
    mainLayout->setStretchFactor(m_goodCost, 5);

    auto* equalLabel = new QLabel(QStringLiteral("="), this);
    equalLabel->setFont(m_baseFont);
    mainLayout->addWidget(equalLabel);
    mainLayout->setStretchFactor(equalLabel, 1);

    m_fullCost = new QLineEdit(this);
    m_fullCost->setFont(m_baseFont);
    m_fullCost->setStyleSheet(Resources::Styles::baseLineEdit);
    m_fullCost->setEnabled(false);
    m_fullCost->setFont(m_baseFont);
    mainLayout->addWidget(m_fullCost);
    mainLayout->setStretchFactor(m_fullCost, 4);
}

void GoodWidget::onLineEditChanged(QLineEdit* lineEdit)
{
    bool flag {true};
    lineEdit->text().toDouble(&flag);
    lineEdit->setTextMargins(25, 0, 0, 0);
    if (flag)
        lineEdit->setStyleSheet(Resources::Styles::baseLineEdit);
    else
        lineEdit->setStyleSheet(Resources::Styles::redBorderLineEdit);
    if (m_goodAmount->styleSheet() != Resources::Styles::redBorderLineEdit &&
        m_goodCost->styleSheet()   != Resources::Styles::redBorderLineEdit &&
        !m_goodAmount->text().isEmpty() &&
        !m_goodCost->text().isEmpty())
    {
        double goodAmount = m_goodAmount->text().toDouble();
        double goodCost = m_goodCost->text().toDouble();
        double total = std::floor(goodAmount * goodCost);
        m_fullCost->setText(QString::number(total));
    }
    else
        m_fullCost->setText({});
}

double GoodWidget::getTotal() const
{
    double total {0.0};
    total = m_fullCost->text().toDouble();
    return total;
}

void GoodWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && styleSheet() != m_selectedStyle)
    {
        m_goodName->setStyleSheet({});
        m_goodName->setEnabled(false);
        m_goodAmount->setStyleSheet({});
        m_goodAmount->setEnabled(false);
        m_goodCost->setStyleSheet({});
        m_goodCost->setEnabled(false);
        m_fullCost->setStyleSheet({});
        setStyleSheet(QStringLiteral("QWidget { background-color: #00BFFF; border: 2px solid #0000FF; border-radius: 5px; }"));
        emit selected(m_numberLabel->text().split(".")[0].toInt());
    }
    else if (event->button() == Qt::LeftButton && styleSheet() == m_selectedStyle)
    {
        dropSelection();
        emit disSelected(m_numberLabel->text().split(".")[0].toInt());
    }
    QWidget::mousePressEvent(event);
}

void GoodWidget::dropSelection()
{
    setStyleSheet({});
    m_goodName->setStyleSheet(Resources::Styles::baseLineEdit);
    m_goodName->setEnabled(true);
    m_goodAmount->setStyleSheet(Resources::Styles::baseLineEdit);
    m_goodAmount->setEnabled(true);
    m_goodCost->setStyleSheet(Resources::Styles::baseLineEdit);
    m_goodCost->setEnabled(true);
    m_fullCost->setStyleSheet(Resources::Styles::baseLineEdit);
}

QString GoodWidget::getFullInfo() const
{
    QString result;
    bool isNameAdded = {!m_goodName->text().isEmpty()};
    bool isAmountAdded {!m_goodAmount->text().isEmpty()};
    bool isCostAdded   {!m_goodCost->text().isEmpty()};
    if (isNameAdded && isAmountAdded && isCostAdded)
    {
        const QString csvSpliter {";"};
        result = m_numberLabel->text() + csvSpliter +
                 m_goodName->text() + csvSpliter +
                 m_goodAmount->text() + csvSpliter +
                 m_goodCost->text() + csvSpliter +
                 m_fullCost->text() + '\n';
    }
    return result;
}

void GoodWidget::setNewNumber(int number)
{
    m_numberLabel->setText(QString::number(number) + ".");
}
