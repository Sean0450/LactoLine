#pragma once

#include <vector>
#include <QWidget>

#include "Product/ProductData.hpp"
#include "Resources.hpp"

QT_FORWARD_DECLARE_CLASS(QTreeView)
QT_FORWARD_DECLARE_CLASS(QStandardItemModel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QStandardItem)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)

class ProductChangedObserver;

class ProductTableWidget: public QWidget
{
    Q_OBJECT
    std::vector<ProductData> m_productData;
    QFont m_baseFont {Resources::baseFont, 14};
    QStringList m_columnName {QStringLiteral("Наименование"), QStringLiteral("Сырье на единицу продукции")};
    static constexpr int s_columnCount {2};

    QTreeView* m_treeView {nullptr};
    QStandardItemModel* m_mainModel {nullptr};
    QPushButton* m_addProduct {nullptr};
    ProductChangedObserver* m_observer {nullptr};

    QList<QStandardItem*> createItemFromData(const ProductData& data);
    void createModel();
    QStringList getCategories() const;
    QHBoxLayout* createButton();
private slots:
    void onAddProductClicked();
public:
    explicit ProductTableWidget(ProductChangedObserver* observer, QWidget* parent=nullptr);
};
