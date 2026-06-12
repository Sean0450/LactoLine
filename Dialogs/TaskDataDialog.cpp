#include "Dialogs/TaskDataDialog.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

TaskDataDialog::TaskDataDialog(QWidget* parent):DataCreationDialog{parent}
{
    setWindowTitle(QStringLiteral("Lactoline"));

    auto* amountEnter = createLineEdit();
    createLineEditLayout(amountEnter, QStringLiteral("Введите количество произведенной продукции:"), [&](){ bool flag {true};
                                                                                                            double result = m_lineEdits[s_amountIndex]->text().toDouble(&flag);
                                                                                                            return flag && result >= 0.0;});

    auto* rawMaterialsEnter = createLineEdit();
    createLineEditLayout(rawMaterialsEnter, QStringLiteral("Введите количество затраченного сырья:"), [&](){bool flag {true};
                                                                                                            double result = m_lineEdits[s_rawMaterialsIndex]->text().toDouble(&flag);
                                                                                                            return flag && result >= 0.0;});
    createButton(QStringLiteral("Добавить"), [&](){m_createdProduct = m_lineEdits[s_amountIndex]->text().toDouble();
                                                   m_wastedRawMaterials = m_lineEdits[s_rawMaterialsIndex]->text().toDouble();});
}


double TaskDataDialog::getCreatedProduct() const
{
    return m_createdProduct;
}

double TaskDataDialog::getWastedRawMaterials() const
{
    return m_wastedRawMaterials;
}
