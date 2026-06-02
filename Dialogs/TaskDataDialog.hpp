#pragma once

#include "DataCreationDialog.hpp"


class TaskDataDialog: public DataCreationDialog
{
    Q_OBJECT
    static constexpr int s_amountIndex {0};
    static constexpr int s_rawMaterialsIndex {1};
    double m_createdProduct {0.0};
    double m_wastedRawMaterials {0.0};

public:
    explicit TaskDataDialog(double createdProduct, QWidget* parent = nullptr);
    double getCreatedProduct() const;
    double getWastedRawMaterials() const;
};
