#pragma once

#include <QDialog>

#include "Resources.hpp"

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLineEdit)

class TaskDataDialog: public QDialog
{
    Q_OBJECT

    const QFont m_baseFont {Resources::baseFont, 12};
    double m_createdProduct {0.0};

    QLabel* m_description {nullptr};
    QPushButton* m_enterButton {nullptr};
    QLineEdit* m_enterField {nullptr};

    void lockDataSending();

private slots:
    void onProductAmountChanged(const QString& text);
    void onEnterButtonClicked();
public:
    explicit TaskDataDialog(double createdProduct, QWidget* parent = nullptr);
    double getCreatedProduct() const;
};
