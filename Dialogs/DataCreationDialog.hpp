#pragma once

#include <QDialog>

#include "Resources.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

class DataCreationDialog : public QDialog
{
    Q_OBJECT
protected:
    const QFont m_baseFont {Resources::baseFont, 13};
    const QString m_baseStyle {QStringLiteral("QLineEdit {") + QLatin1String(Resources::Styles::baseElementsStyle) + QStringLiteral("}")};
    std::vector<QLineEdit*> m_lineEdits;

    QPushButton* m_createButton {nullptr};
    QVBoxLayout* m_mainLayot {nullptr};

    void enableCreateButton();
    QLineEdit* createLineEdit();
    void createLineEditLayout(QLineEdit* lineEdit, const QString& labelText, std::function<bool()>&& slot);
protected slots:
    void onLineEditCheck(QLineEdit* lineEdit, bool flag);

public:
    explicit DataCreationDialog(QWidget *parent = nullptr);
    void createButton(const QString& buttonName, std::function<void()>&& slot);
};

