#pragma once

#include <QWidget>

#include "Resources.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)

class GoodWidget : public QWidget
{
    Q_OBJECT
    const QFont m_baseFont {Resources::baseFont, 13};
    const QString m_selectedStyle {"QWidget { background-color: #00BFFF; border: 2px solid #0000FF; border-radius: 5px; }"};
    QStringList m_productNames;
    QLineEdit* m_goodName {nullptr};
    QLineEdit* m_goodAmount {nullptr};
    QLineEdit* m_goodCost {nullptr};
    QLineEdit* m_fullCost {nullptr};
    QLabel* m_numberLabel {nullptr};
private slots:
    void onLineEditChanged(QLineEdit* lineEdit);
protected:
    void mousePressEvent(QMouseEvent *event) override;
public:
    explicit GoodWidget(const QStringList& productNames, int number, QWidget *parent = nullptr);
    void dropSelection();
    void setNewNumber(int number);
    double getTotal() const;
    QString getFullInfo() const;
signals:
    void selected(int number);
    void disSelected(int number);
};


