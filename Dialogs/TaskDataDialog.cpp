#include "Dialogs/TaskDataDialog.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

TaskDataDialog::TaskDataDialog(double createdProduct, QWidget* parent):DataCreationDialog{parent},
                                                                       m_createdProduct(createdProduct)
{
    setWindowTitle(QStringLiteral("Lactoline"));

    auto* amountEnter = createLineEdit();
    amountEnter->setText(QString::number(createdProduct));
    createLineEditLayout(amountEnter, QStringLiteral("Введите количество произведенной продукции:"), [&](){ bool flag {true};
                                                                                                            double result = m_lineEdits[s_amountIndex]->text().toDouble(&flag);
                                                                                                            return flag && result > m_createdProduct;});

    auto* rawMaterialsEnter = createLineEdit();
    createLineEditLayout(rawMaterialsEnter, QStringLiteral("Введите количество затраченного сырья:"), [&](){bool flag {true};
                                                                                                           m_lineEdits[s_rawMaterialsIndex]->text().toDouble(&flag);
                                                                                                           return flag;});
    createButton(QStringLiteral("Добавить"), [&](){m_createdProduct = m_lineEdits[s_amountIndex]->text().toDouble();});
}


double TaskDataDialog::getCreatedProduct() const
{
    return m_createdProduct;
}

double TaskDataDialog::getWastedRawMaterials() const
{
    return m_wastedRawMaterials;
}
