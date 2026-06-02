#include "PackingListWidget.hpp"
#include "Resources.hpp"
#include "Date/DateTranslator.hpp"
#include "GoodWidget.hpp"
#include "PackingListObserver.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QFile>

PackingList::PackingList(PackingListObserver* observer, QWidget* parent):QWidget{parent},
                                                                         m_observer(observer)
{
    auto* globalMainLayout = new QVBoxLayout(this);
    globalMainLayout->setContentsMargins(0, 0, 0, 0);
    globalMainLayout->setSpacing(0);
    m_pageTitle = new QLabel(QStringLiteral("Товарная накладная от ") + QString::fromStdString(DateTranslator::getCurrentDate()), this);
    m_pageTitle->setFont(QFont(Resources::baseFont, 18));
    globalMainLayout->addWidget(m_pageTitle);
    globalMainLayout->addSpacing(15);
    globalMainLayout->setAlignment(m_pageTitle, Qt::AlignHCenter);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    auto* mainWidget = new QWidget();
    scrollArea->setWidget(mainWidget);
    auto* mainWidgetLayout = new QVBoxLayout(mainWidget);
    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    mainWidgetLayout->addLayout(m_mainLayout);

    QStringList productNames;
    if (m_observer)
    {
        std::ranges::for_each(m_observer->getProductNames(), [&](const auto& name){productNames << QString::fromStdString(name);});
    }
    for (int i = 0; i < s_baseFiledsCount; ++i)
    {
        m_goods.emplace_back(new GoodWidget(productNames, i + 1, this));
        connectNewGood();
    }
    globalMainLayout->addWidget(scrollArea);

    auto* buttonLayout = new QHBoxLayout();

    const QString buttonStyle = QStringLiteral("QPushButton {") + Resources::Styles::baseElementsStyle + QStringLiteral("}") + Resources::Styles::pressedButtonStyle;

    const QFont totalFont {Resources::baseFont, 14};
    auto* totalLayout = new QHBoxLayout();
    totalLayout->addStretch(1);
    m_totalButton = new QPushButton(QStringLiteral("Итого"), mainWidget);
    m_totalButton->setFont(totalFont);
    m_totalButton->setStyleSheet(buttonStyle);
    connect(m_totalButton, &QPushButton::clicked, this, &PackingList::onTotalClicked);
    m_totalLabel = new QLabel(QStringLiteral(": 0"), mainWidget);
    m_totalLabel->setFont(totalFont);
    totalLayout->addWidget(m_totalButton);
    totalLayout->setAlignment(m_totalButton, Qt::AlignRight);
    totalLayout->addWidget(m_totalLabel);
    totalLayout->setAlignment(m_totalLabel, Qt::AlignRight);
    mainWidgetLayout->addLayout(totalLayout);
    mainWidgetLayout->addStretch(1);

    m_addField = new QPushButton(this);
    m_addField->setStyleSheet(Resources::Styles::plusButton);
    connect(m_addField, &QPushButton::clicked, this, &PackingList::onAddFieldButtonClicked);
    buttonLayout->addWidget(m_addField);

    m_removeField = new QPushButton(this);
    m_removeField->setStyleSheet(Resources::Styles::deleteButton);
    connect(m_removeField, &QPushButton::clicked, this, &PackingList::onRemoveFieldButtonClicked);
    buttonLayout->addWidget(m_removeField);
    globalMainLayout->addLayout(buttonLayout);
    globalMainLayout->setAlignment(buttonLayout, Qt::AlignRight);

    m_createDocument = new QPushButton(QStringLiteral("Сформировать накладную"), this);
    m_createDocument->setStyleSheet(buttonStyle);
    m_createDocument->setFont(totalFont);
    connect(m_createDocument, &QPushButton::clicked, this, &PackingList::onCreateDocumentClicked);
    globalMainLayout->addWidget(m_createDocument);
    globalMainLayout->setAlignment(m_createDocument, Qt::AlignLeft);
}

void PackingList::connectNewGood()
{
    connect(m_goods.back(), &GoodWidget::selected, this, [&](int num){onGoodSelected(num);});
    connect(m_goods.back(), &GoodWidget::disSelected, this, [&](int num){if (m_selectedGoodNum == num) m_selectedGoodNum = -1;});
    m_mainLayout->addWidget(m_goods.back());
}

void PackingList::onTotalClicked()
{
    double totalSum {0.0};
    for (const auto* good: m_goods)
    {
        totalSum += good->getTotal();
    }
    m_totalLabel->setText(QStringLiteral(": ") + QString::number(totalSum));
}

void PackingList::onCreateDocumentClicked()
{
    onTotalClicked();
    QFile packingList {m_pageTitle->text() + QStringLiteral(".csv")};
    if (packingList.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&packingList);
        out.setGenerateByteOrderMark(true);
        out.setEncoding(QStringConverter::Utf8);
        out << QStringLiteral("№;Наименование;Кол-во;Цена;Стоимость\n");
        for (auto* good: m_goods)
        {
            const QString goodData = good->getFullInfo();
            if (!goodData.isEmpty())
                out << goodData;
        }
        out << QStringLiteral(";;;;Итого;") + m_totalLabel->text().split(":")[1];
        packingList.close();
    }
}

void PackingList::onAddFieldButtonClicked()
{
    QStringList productNames;
    if (m_observer)
    {
        std::ranges::for_each(m_observer->getProductNames(), [&](const auto& name){productNames << QString::fromStdString(name);});
    }
    int goodNum = static_cast<int>(m_goods.size()) + 1;
    m_goods.emplace_back(new GoodWidget(productNames, goodNum, this));
    connectNewGood();
}

void PackingList::onRemoveFieldButtonClicked()
{
    if (m_selectedGoodNum > -1)
    {
        m_goods.at(m_selectedGoodNum - 1)->deleteLater();
        m_goods.erase(m_goods.begin() + m_selectedGoodNum - 1);
        for (int i = m_selectedGoodNum - 1; i < static_cast<int>(m_goods.size()); ++i)
        {
            m_goods.at(i)->setNewNumber(i + 1);
        }
        m_selectedGoodNum = -1;
    }

}

void PackingList::onGoodSelected(int num)
{
    if (m_selectedGoodNum > -1)
    {
        m_goods.at(m_selectedGoodNum - 1)->dropSelection();
    }
    m_selectedGoodNum = num;
}
