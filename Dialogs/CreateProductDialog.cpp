#include "CreateProductDialog.hpp"

#include <QLineEdit>
#include <QCompleter>

CreateProductDialog::CreateProductDialog(const QStringList& categories, QWidget *parent)
    : DataCreationDialog{parent},
      m_categories(categories)
{
    setWindowTitle(QStringLiteral("Добавление нового продукта"));

    auto* productName = createLineEdit();
    createLineEditLayout(productName, QStringLiteral("Наименование продукта:"), [&](){return !m_lineEdits[s_productNameIndex]->text().isEmpty();});

    auto* categoryName = createLineEdit();
    m_productNameCompleter = new QCompleter(m_categories, this);
    m_productNameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    categoryName->setCompleter(m_productNameCompleter);
    createLineEditLayout(categoryName, QStringLiteral("Наименование категории:"), [&](){return !m_lineEdits[s_categoryNameIndex]->text().isEmpty();});

    auto* unitRawMaterials = createLineEdit();
    createLineEditLayout(unitRawMaterials, QStringLiteral("Сырье для единицы товара:"), [&](){bool flag {true};
                                                                                              m_lineEdits[s_unitRawMaterialIndex]->text().toDouble(&flag);
                                                                                              return flag;});

    auto* primeCost = createLineEdit();
    createLineEditLayout(primeCost, QStringLiteral("Себестоимость:"), [&](){bool flag {true};
                                                                            double cost = m_lineEdits[s_primeCostIndex]->text().toDouble(&flag);
                                                                            return flag && cost > 1.0;});

    auto* epsilon = createLineEdit();
    createLineEditLayout(epsilon, QStringLiteral("Погрешность производства(%):"), [&](){bool flag {true};
                                                                                        m_lineEdits[s_epsilonIndex]->text().toDouble(&flag);
                                                                                        return flag;});
    createButton(QStringLiteral("Добавить продукт"), [&](){createProduct();
                                                           close();});
}

void CreateProductDialog::createProduct()
{
    const std::string productName = m_lineEdits[s_productNameIndex]->text().toStdString();
    const std::string categoryName = m_lineEdits[s_categoryNameIndex]->text().toStdString();
    const double unitRawMaterials = m_lineEdits[s_unitRawMaterialIndex]->text().toDouble();
    const double primeCost = m_lineEdits[s_primeCostIndex]->text().toDouble();
    const double epsilon = (m_lineEdits[s_epsilonIndex]->text().toDouble() * unitRawMaterials) / 100;
    m_productData = ProductData(productName, categoryName, unitRawMaterials, primeCost, epsilon);
}

std::optional<ProductData> CreateProductDialog::getCreatedData() const
{
    std::optional<ProductData> result;
    if (!m_productData.productName.empty() && m_productData.unitRawMaterials > 0.0)
    {
        result = m_productData;
    }
    return result;
}
