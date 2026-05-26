#include "ProductTableWidget.hpp"
#include "Dialogs/CreateProductDialog.hpp"
#include "ProductChangedObserver.hpp"

#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QStringList>
#include <QHeaderView>

ProductTableWidget::ProductTableWidget(ProductChangedObserver* observer, QWidget* parent): QWidget(parent),
                                                                                           m_observer(observer)
{
    if (m_observer)
    {
        m_productData = m_observer->getProductData();
    }
    createModel();
    auto* mainLayout = new QVBoxLayout(this);

    constexpr int nameColumnWidth {310};
    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_mainModel);
    auto* header= m_treeView->header();
    header->setFont(m_baseFont);
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_treeView->setColumnWidth(0, nameColumnWidth);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_treeView);
    mainLayout->addLayout(createButton());
}

QHBoxLayout* ProductTableWidget::createButton()
{
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);

    m_addProduct = new QPushButton(QStringLiteral("Добавить продукт"), this);
    const QString style = QStringLiteral("QPushButton {") +
                          QLatin1String(Resources::Styles::baseElementsStyle) +
                          QStringLiteral("}") +
                          QLatin1String(Resources::Styles::pressedButtonStyle);
    m_addProduct->setStyleSheet(style);
    m_addProduct->setFont(m_baseFont);
    connect(m_addProduct, &QPushButton::clicked, this, &ProductTableWidget::onAddProductClicked);
    buttonLayout->addWidget(m_addProduct);

    return buttonLayout;
}

void ProductTableWidget::createModel()
{
    m_mainModel = new QStandardItemModel();
    m_mainModel->setHorizontalHeaderLabels(m_columnName);
    std::vector<std::unique_ptr<QStandardItem>> rootItems;
    for (const auto& product: m_productData)
    {
        const QString categoryName {QString::fromStdString(product.categoryName)};
        auto iterator = std::ranges::find_if(rootItems, [&](auto&& item){return item->text() == product.categoryName;});
        if (iterator == std::ranges::end(rootItems))
        {
            auto rootItem = std::make_unique<QStandardItem>(categoryName);
            rootItem->setFont(m_baseFont);

            rootItem->appendRow(createItemFromData(product));
            rootItems.emplace_back(std::move(rootItem));
        }
        else
            (*iterator)->appendRow(createItemFromData(product));
    }
    for (auto&& item: rootItems)
    {
        m_mainModel->appendRow(item.get());
        item.release();
    }

}

QList<QStandardItem*> ProductTableWidget::createItemFromData(const ProductData& data)
{
    QList<QStandardItem*> childrenItem;
    QString materialName;
    if (data.unitRawMaterials - 1.0 < 0.001)
        materialName = QStringLiteral(" литр");
    else if (data.unitRawMaterials < 1.0 || (data.unitRawMaterials > 1.0 && data.unitRawMaterials < 5.0))
        materialName = QStringLiteral(" литра");
    else
        materialName = QStringLiteral(" литров");
    auto* productName = new QStandardItem(QString::fromStdString(data.productName));
    productName->setFont(m_baseFont);
    auto* primeCost = new QStandardItem(QString::number(data.unitRawMaterials) + materialName);
    primeCost->setFont(m_baseFont);
    childrenItem.append(productName);
    childrenItem.append(primeCost);
    return childrenItem;
}

QStringList ProductTableWidget::getCategories() const
{
    QStringList data;
    for (int i = 0; i < m_mainModel->rowCount(); ++i)
    {
        data << m_mainModel->item(i, 0)->text();
    }
    return data;
}

void ProductTableWidget::onAddProductClicked()
{
    auto categories = getCategories();
    CreateProductDialog dialog = CreateProductDialog(categories, this);
    dialog.exec();
    std::optional<ProductData> newProductData = dialog.getCreatedData();
    if (newProductData && m_observer)
    {
        bool isItemAdded {false};
        for (int i = 0; i < m_mainModel->rowCount(); ++i)
        {
            if (m_mainModel->item(i, 0)->text().toStdString() == newProductData->categoryName)
            {
                m_mainModel->item(i, 0)->appendRow(createItemFromData(newProductData.value()));
                isItemAdded = true;
                break;
            }
        }
        if (!isItemAdded)
        {
            auto* categoryItem = new QStandardItem(QString::fromStdString(newProductData.value().categoryName));
            categoryItem->setFont(m_baseFont);
            categoryItem->appendRow(createItemFromData(newProductData.value()));
            m_mainModel->appendRow(categoryItem);
        }
        m_productData.emplace_back(newProductData.value());
        m_observer->createProduct(newProductData.value());
    }

}
