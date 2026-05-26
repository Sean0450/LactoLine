#pragma once

#include "DataCreationDialog.hpp"
#include "Product/ProductData.hpp"

QT_FORWARD_DECLARE_CLASS(QCompleter)

class CreateProductDialog : public DataCreationDialog
{
    Q_OBJECT
    static constexpr int s_productNameIndex {0};
    static constexpr int s_categoryNameIndex {1};
    static constexpr int s_unitRawMaterialIndex {2};
    static constexpr int s_primeCostIndex {3};
    static constexpr int s_epsilonIndex {4};
    const QStringList& m_categories;
    ProductData m_productData;

    QCompleter* m_productNameCompleter {nullptr};

    void createProduct();
public:
    explicit CreateProductDialog(const QStringList& categories, QWidget *parent = nullptr);
    std::optional<ProductData> getCreatedData() const;
};

